#ifndef Config_hpp
#define Config_hpp

#include <filesystem>
#include <fstream>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/core/utils/ConversionUtils.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <regex>
#include <sstream>
#include <cstdlib>

#include "Dto/ConfigDto.hpp"

class Config {
   private:
    oatpp::base::CommandLineArguments m_cmdArgs = oatpp::base::CommandLineArguments(0, nullptr);
    std::string databaseConnectionString;  // Connection string to the database
    v_uint16 databaseMaxConnections;       // Max connections to the database
    std::string databaseMigrationsPath;
    std::string jwtSecret;  // Secret for JWT
    std::string jwtIssuer;  // Issuer for JWT
    v_uint64 jwtExpiry;     // Expiry for JWT in seconds
    v_uint16 port = 8000;   // Port for the server

    std::string logFilePath;  // Path to the log file
    v_uint8 logLevel;         // Level of logging to the log file each bit represents a level
    std::fstream confFile;
    oatpp::Object<ConfigDTO> config;

   public:
    Config(oatpp::base::CommandLineArguments cmdArgs);
    ~Config();

    oatpp::Object<ConfigDTO> UpdateConfig();
    std::string GetDatabaseConnectionString();
    v_uint16 GetDatabaseMaxConnections();
    std::string GetDatabaseMigrationsPath();
    std::string GetJwtSecret();
    std::string GetJwtIssuer();
    v_uint64 GetJwtExpiry();
    v_uint16 GetPort();
    std::string GetLogFilePath();
    v_uint8 GetLogFileLevel();
    void SetTestConfig();
    bool StopServer();
};

#endif /* Config_hpp */