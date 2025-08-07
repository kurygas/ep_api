#pragma once

#define CROW_JSON_USE_MAP

#include <crow.h>
#include <sw/redis++/redis++.h>

class RedisConnectionOptions : public sw::redis::ConnectionOptions {
public:
    RedisConnectionOptions();
};

class AuthService {
public:
    AuthService();
    void run();

private:
    void pullConfig();

    crow::response getRefreshToken(const crow::json::rvalue& req);
    void tgAuth(const crow::json::rvalue& req);
    void passwordAuth(const crow::json::rvalue& req);

    crow::response getAccessToken(const crow::json::rvalue& req);

    crow::SimpleApp app_;
    std::string authSecret_;
    std::unordered_map<std::string, std::string> passwords_;
    std::string tgBotToken_;
    sw::redis::Redis redis_;
};
