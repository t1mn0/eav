#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "../include/Result/Result.hpp"

// these are only simulating examples, which omit many functional subtleties.
// it demonstrate the using of fpp libraries in "everyday" code.

using json = nlohmann::json;

struct Config {
    std::string host;
    int port;
    bool ssl;
};

fpp::Result<Config, fpp::StringError> parseConfig(const json& j) {
    try {
        if (!j.contains("host") || !j["host"].is_string()) {
            return fpp::Result<Config, fpp::StringError>::Err(fpp::StringError("Missing or invalid 'host' field"));
        }
        if (!j.contains("port") || !j["port"].is_number()) {
            return fpp::Result<Config, fpp::StringError>::Err(fpp::StringError("Missing or invalid 'port' field"));
        }

        Config cfg {
            .host = j["host"],
            .port = j["port"],
            .ssl = j.value("ssl", false)
        };

        if (cfg.port <= 0 || cfg.port > 65535) {
            return fpp::Result<Config, fpp::StringError>::Err(fpp::StringError("Port must be in range 1-65535"));
        }

        return fpp::Result<Config, fpp::StringError>::Ok(cfg);
    } catch (const std::exception& e) {
        return fpp::Result<Config, fpp::StringError>::Err(fpp::StringError(std::string("JSON parsing error: ") + e.what()));
    }
}

int main() {
    json badConfig = { {"host", "example.com"}, {"port", 655354} };
    json goodConfig = { {"host", "example.com"}, {"port", 8080} };

    auto result1 = parseConfig(badConfig);
    auto result2 = parseConfig(goodConfig);

    if (result1.is_err()) {
        std::cout << "Config loaded: {error: '" << result1.unwrap_err().err_message() << "'}\n";
    }

    if (result2.is_ok()) {
        auto cfg = result2.unwrap_val();
        std::cout << "Config loaded: {host: " << cfg.host << ", port: " << cfg.port << "}\n";
    }

    // I used the unwrap_err() and unwrap_val() methods 
    // because in this case I am sure that the exception will not be thrown. 
    // But it is better to write the unwrap_to_opt_val() methods, 
    // check the received Option<T> object for has_value() and work with it already.
    
    return 0;
}