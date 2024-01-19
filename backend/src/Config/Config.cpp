#include "Config.hpp"

Config::Config(oatpp::base::CommandLineArguments cmdArgs) {
    this->m_cmdArgs = cmdArgs;
    auto configFile = std::string(cmdArgs.getNamedArgumentValue("--config") ? cmdArgs.getNamedArgumentValue("--config") : "");
    if (configFile != "") {
        if (configFile[0] == '/') {
            this->confFile.open(configFile, std::ios::out | std::ios::in);
        } else {
            OATPP_LOGE("Config", "Either the config file is invalid or the path is not absolute");
            exit(EXIT_FAILURE);
        }
    } else {
        this->confFile.open(DEFAULT_CONFIG_FOLDER "/config.json", std::ios::out | std::ios::in);
        if(this->confFile.fail()){
            this->confFile.open(DEFAULT_CONFIG_FOLDER "/config.json.example", std::ios::out | std::ios::in);
        }
    }
    if (UpdateConfig() == nullptr) {  // If the config is invalid, exit
        OATPP_LOGE("Config", "Invalid config file");
        exit(EXIT_FAILURE);
    }
}
Config::~Config() {
    this->confFile.close();
}
oatpp::Object<ConfigDTO> Config::UpdateConfig() {
    std::stringstream configStream;
    configStream << this->confFile.rdbuf();

    oatpp::Object<ConfigDTO> newConfig;
    try {
        auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        newConfig = objectMapper->readFromString<oatpp::Object<ConfigDTO>>(configStream.str());
    } catch (std::exception& e) {
        OATPP_LOGE("Config", "Encountered error while parsing config file: %s", e.what());
        return nullptr;
    }
    if (newConfig == nullptr) {
        OATPP_LOGE("Config", "Config file is empty");
        return nullptr;
    }
    if (newConfig->baseFolder == nullptr) {
        newConfig->baseFolder = (std::string)std::filesystem::current_path() + "/";
    }
    if (!std::filesystem::is_directory(newConfig->baseFolder->c_str())) {
        OATPP_LOGW("Config", "Default folder is not a directory, this will cause issues if relative paths are used");
    }
    if (newConfig->jwt == nullptr) {
        OATPP_LOGE("Config", "JWT config is not set");
        return nullptr;
    }
        if (newConfig->jwt->secret == nullptr) {
        OATPP_LOGV("Config", "JWT secret is not set, generating a new one");
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "!@#$%^&*()_+-=[]{};':\",./<>?\\|`~";
        unsigned char secret[DEFAULT_SECRET_LENGTH];
        RAND_bytes(secret, DEFAULT_SECRET_LENGTH);
        for (int i = 0; i < DEFAULT_SECRET_LENGTH; i++) {
            secret[i] = alphanum[secret[i] % 94];
        }
        newConfig->jwt->secret = oatpp::String((const char*)secret, DEFAULT_SECRET_LENGTH);

        // Save the new secret to the config file
        auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        objectMapper->getSerializer()->getConfig()->useBeautifier = true;
        objectMapper->getSerializer()->getConfig()->includeNullFields = false;
        std::string configString = objectMapper->writeToString(newConfig);
        this->confFile.seekp(0);
        this->confFile << configString;
        this->confFile.flush();
    }
    if (newConfig->jwt->issuer == nullptr) {
        OATPP_LOGE("Config", "JWT issuer is not set");
        return nullptr;
    }
    if (newConfig->jwt->expiry == nullptr) {
        OATPP_LOGE("Config", "JWT expiry is not set");
        return nullptr;
    }
    if (newConfig->database == nullptr) {
        OATPP_LOGE("Config", "Database config is not set");
        return nullptr;
    }
    if (newConfig->database->connectionString == nullptr) {
        OATPP_LOGE("Config", "Database connection string is not set");
        return nullptr;
    }
    if (newConfig->logs == nullptr) {
        OATPP_LOGE("Config", "Log config is not set");
        return nullptr;
    }
    if (newConfig->logs->logFilePath == nullptr) {
        OATPP_LOGE("Config", "Log file path is not set");
        return nullptr;
    }

    /*
     * Port
     */

    if (newConfig->port != nullptr) {
        bool changed = false;
        char* portChar = (char*)m_cmdArgs.getNamedArgumentValue("--port");
        if (portChar != nullptr) {
            v_int32 port32 = oatpp::utils::conversion::strToInt32(portChar);
            if (port32 > 0 && port32 < 65535) {
                port = (v_uint16)port32;
                changed = true;
            }
        }
        if (*newConfig->port > 0 && *newConfig->port < 65535 && !changed) {
            this->port = *newConfig->port;
        }
    }

    /*
     * Database
     */

    auto envDatabaseConnectionString = std::getenv("DATABASE_URL");  // For docker configuration
    if (envDatabaseConnectionString == nullptr) {
        this->databaseConnectionString = newConfig->database->connectionString;
    } else {
        this->databaseConnectionString = envDatabaseConnectionString;
    }
    std::regex databaseConnectionStringRegex("^postgresql:\\/\\/([^:]+):([^@]+)@([^:]+):(\\d+)\\/([^?]+)(\\?.+)?$");
    if (!std::regex_search(newConfig->database->connectionString->c_str(), databaseConnectionStringRegex)) {
        OATPP_LOGE("Config", "Invalid database connection string %s", newConfig->database->connectionString->c_str());
        return nullptr;
    }

    if (newConfig->database->migrationPath == nullptr) {
        this->databaseMigrationsPath = DEFAULT_MIGRATIONS_FOLDER;
    } else {
        this->databaseMigrationsPath = newConfig->database->migrationPath->c_str();
    }
    OATPP_LOGD("Config", "Using database migration path %s", this->databaseMigrationsPath.c_str());
    this->databaseMaxConnections = newConfig->database->maxConnections ? *newConfig->database->maxConnections : 1024;  // Default value

    /*
     * JWT
     */

    this->jwtSecret = newConfig->jwt->secret->c_str();
    this->jwtIssuer = newConfig->jwt->issuer->c_str();
    this->jwtExpiry = newConfig->jwt->expiry;

    /*
     * Logging
     */

    this->logFilePath = newConfig->logs->logFilePath->c_str();
    v_uint8 logLevel = 0;
    if (newConfig->logs->error != nullptr) {
        if (*newConfig->logs->error) {
            logLevel |= 1 << oatpp::base::Logger::PRIORITY_E;
        }
    } else {
        logLevel |= 1 << oatpp::base::Logger::PRIORITY_E;
    }
    if (newConfig->logs->warning != nullptr) {
        if (*newConfig->logs->warning) {
            logLevel |= 1 << oatpp::base::Logger::PRIORITY_W;
        }
    } else {
        logLevel |= 1 << oatpp::base::Logger::PRIORITY_W;
    }
    if (newConfig->logs->info != nullptr) {
        if (*newConfig->logs->info) {
            logLevel |= 1 << oatpp::base::Logger::PRIORITY_I;
        }
    }
    if (newConfig->logs->debug != nullptr) {
        if (*newConfig->logs->debug) {
            logLevel |= 1 << oatpp::base::Logger::PRIORITY_D;
        }
    }
    if (newConfig->logs->verbose != nullptr) {
        if (*newConfig->logs->verbose) {
            logLevel |= 1 << oatpp::base::Logger::PRIORITY_V;
        }
    }
    this->logLevel = logLevel;

    this->config = newConfig;

    return newConfig;
}
std::string Config::GetDatabaseConnectionString() {
    return this->databaseConnectionString;
}
v_uint16 Config::GetDatabaseMaxConnections() {
    return this->databaseMaxConnections;
}
std::string Config::GetJwtSecret() {
    return this->jwtSecret;
}
std::string Config::GetJwtIssuer() {
    return this->jwtIssuer;
}
v_uint64 Config::GetJwtExpiry() {
    return this->jwtExpiry;
}
v_uint16 Config::GetPort() {
    return this->port;
}
std::string Config::GetDatabaseMigrationsPath() {
    return this->databaseMigrationsPath;
}
std::string Config::GetLogFilePath() {
    return this->logFilePath;
}
v_uint8 Config::GetLogFileLevel() {
    return this->logLevel;
}
void Config::SetTestConfig() {
    this->logLevel = 0xFF;
    this->logFilePath = "/dev/null";
    this->jwtExpiry = 3600;
    this->jwtIssuer = "Test";
    this->jwtSecret = "TestSecret";
}
bool Config::StopServer() {
    return this->m_cmdArgs.hasArgument("--stop");
}