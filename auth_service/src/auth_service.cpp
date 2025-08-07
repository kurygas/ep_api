#include "auth_service.h"
#include "crypto.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <jwt-cpp/jwt.h>

using namespace std::chrono_literals;

RedisConnectionOptions::RedisConnectionOptions() {
    host = "redis";
    port = 6379;
}

AuthService::AuthService()
: redis_(RedisConnectionOptions()) {
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
    boost::property_tree::read_ini("config.ini", pt);
    authSecret_ = pt.get<std::string>("auth.secret");
    passwords_["tg_bot"] = pt.get<std::string>("tg_bot.password");
    tgBotToken_ = pt.get<std::string>("tg_bot.token");
}

crow::response AuthService::getRefreshToken(const crow::json::rvalue& req) {
    crow::json::wvalue response;

    try {
        if (!req) {
            throw std::runtime_error("Invalid payload");
        }

        std::string authDataType;
        std::string authData;

        if (req.count("id") != 0) {
            tgAuth(req);
            authDataType = "tg_id";
            authData = req["id"].s();
        }
        else if (req.count("name") != 0 && req.count("password") != 0) {
            passwordAuth(req);
            authDataType = "admin_name";
            authData = req["name"].s();
        }
        else {
            throw std::runtime_error("Invalid authorization method");
        }

        const auto refreshToken = jwt::create()
            .set_issuer("auth_service")
            .set_type("refresh")
            .set_payload_claim(authDataType, jwt::claim(authData))
            .sign(jwt::algorithm::hs256{authSecret_});

        redis_.setex(authData, 300h, refreshToken);
        response["refresh_token"] = refreshToken;
        return {200, response};
    }
    catch (const std::exception& e) {
        response.clear();
        response["error"] = e.what();
        return {400, response};
    }
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

    try {
        if (!req || req.count("refresh_token") == 0) {
            throw std::runtime_error("Invalid payload");
        }

        const auto refreshToken = req["refresh_token"].s();
        const auto decodedToken = jwt::decode(refreshToken);
        const auto tokenPayload = decodedToken.get_payload_json();
        std::string authDataType;

        if (tokenPayload.contains("tg_id")) {
            authDataType = "tg_id";
        }
        else if (tokenPayload.contains("admin_name")) {
            authDataType = "admin_name";
        }
        else {
            throw std::runtime_error("Invalid refresh token");
        }

        auto authData = tokenPayload.at(authDataType).to_str();

        jwt::verify()
            .with_issuer("auth_service")
            .with_type("refresh")
            .with_claim(authDataType, jwt::claim(authData))
            .allow_algorithm(jwt::algorithm::hs256{authSecret_})
            .verify(decodedToken);

        if (!redis_.exists(authData) || redis_.get(authData) != refreshToken) {
            throw std::runtime_error("Wrong refresh token");
        }

        response["access_token"] = jwt::create()
            .set_issuer("auth_service")
            .set_type("access")
            .set_payload_claim(authDataType, jwt::claim(std::move(authData)))
            .set_expires_at(std::chrono::system_clock::now() + 15min)
            .sign(jwt::algorithm::hs256{authSecret_});
    
        return {200, response};
    }
    catch (const std::exception& e) {
        response.clear();
        response["error"] = e.what();
        return {400, response};
    }
}
