#include "auth_service.h"
#include "crypto.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace std::chrono_literals;

AuthService::AuthService()
: redis_("tcp://127.0.0.1:6379")
, verifier_(jwt::verify()) {
    verifier_.with_issuer("auth_service").with_type("refresh").allow_algorithm(jwt::algorithm::hs256{authSecret_});
    pullConfig();

    CROW_ROUTE(app_, "/auth/refresh_token").methods("POST"_method)([this](const crow::request& req){
        return getRefreshToken(crow::json::load(req.body));
    });

    CROW_ROUTE(app_, "/auth/access_token").methods("POST"_method)([this](const crow::request& req){
        return getAccessToken(crow::json::load(req.body));
    });
}

void AuthService::run() {
    app_.port(8081).multithreaded().run();
}

void AuthService::pullConfig() {
    boost::property_tree::ptree pt;
    boost::property_tree::read_ini("../config/config.ini", pt);
    authSecret_ = pt.get<std::string>("auth.secret");
    passwords_["tg_bot"] = pt.get<std::string>("tg_bot.password");
    tgBotToken_ = pt.get<std::string>("tg_bot.token");
}

crow::response AuthService::getRefreshToken(const crow::json::rvalue& req) {
    crow::json::wvalue response;
    std::string redisKey;

    try {
        if (!req) {
            throw std::runtime_error("Invalid payload");
        }

        if (req.count("id") != 0) {
            tgAuth(req);
            redisKey = req["id"].s();
        }
        else if (req.count("name") != 0 && req.count("password") != 0) {
            passwordAuth(req);
            redisKey = req["name"].s();
        }
        else {
            throw std::runtime_error("Invalid authorization method");
        }
    }
    catch (const std::exception& e) {
        response["error"] = e.what();
        return {400, response};
    }
    
    auto token = jwt::create()
        .set_issuer("auth_service")
        .set_type("refresh")
        .set_payload_claim("key", jwt::claim(redisKey))
        .sign(jwt::algorithm::hs256{authSecret_});

    redis_.setex(redisKey, 300h, token);
    response["refresh_token"] = std::move(token);
    return {200, response};
}

void AuthService::tgAuth(const crow::json::rvalue& req) {
    if (time(nullptr) - static_cast<int64_t>(req["auth_date"]) > 3600) {
        throw std::runtime_error("Too old data");
    }

    std::string checkString;
    
    for (const auto& key : req.keys()) {
        checkString += std::format("{}={}\n", key, static_cast<std::string>(req[key].s()));
    }

    checkString.pop_back();

    if (Crypto::hmacSha256(checkString, Crypto::sha256(tgBotToken_)) != req["hash"]) {
        throw std::runtime_error("Invalid data for tg authorization");
    }
}

void AuthService::passwordAuth(const crow::json::rvalue& req) {
    const auto name = req["name"].s();
    const auto password = req["password"].s();

    if (!passwords_.contains(name) || passwords_[name] != password) {
        throw std::runtime_error("Invalid data for password authorization");
    }
}

crow::response AuthService::getAccessToken(const crow::json::rvalue& req) {
    crow::json::wvalue response;
    std::string key;

    try {
        if (!req || req.count("refresh_token") == 0) {
            throw std::runtime_error("Invalid payload");
        }

        const auto refreshToken = req["refresh_token"].s();
        const auto decodedToken = jwt::decode(refreshToken);
        verifier_.verify(decodedToken);
        const auto tokenPayload = decodedToken.get_payload_json();
        
        if (!tokenPayload.contains("key")) {
            throw std::runtime_error("Invalid refresh token");
        }

        key = tokenPayload.at("key").to_str();

        if (!redis_.exists(key) || redis_.get(key) != refreshToken) {
            throw std::runtime_error("Wrong refresh token");
        }
    }
    catch (const std::exception& e) {
        response["error"] = e.what();
        return {400, response};
    }

    response["access_token"] = jwt::create()
        .set_issuer("auth_service")
        .set_type("access")
        .set_payload_claim("key", jwt::claim(std::move(key)))
        .set_expires_at(std::chrono::system_clock::now() + 15min)
        .sign(jwt::algorithm::hs256(authSecret_));
    
    return {200, response};
}
