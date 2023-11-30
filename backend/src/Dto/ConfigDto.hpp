#ifndef ConfigDTO_hpp
#define ConfigDTO_hpp

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class DatabaseConfigDTO : public oatpp::DTO {
    DTO_INIT(DatabaseConfigDTO, DTO)

    DTO_FIELD(String, connectionString, "connectionString");
    DTO_FIELD(UInt16, maxConnections, "maxConnections");
    DTO_FIELD(String, migrationPath, "migrationPath");
};
class LogConfigDTO : public oatpp::DTO {
    DTO_INIT(LogConfigDTO, DTO)

    DTO_FIELD(String, logFilePath, "filePath");
    DTO_FIELD(Boolean, debug, "E");
    DTO_FIELD(Boolean, warning, "W");
    DTO_FIELD(Boolean, info, "I");
    DTO_FIELD(Boolean, error, "D");
    DTO_FIELD(Boolean, verbose, "V");
};
class JWTConfigDTO : public oatpp::DTO {
    DTO_INIT(JWTConfigDTO, DTO)

    DTO_FIELD(String, secret, "secret");
    DTO_FIELD(String, issuer, "issuer");
    DTO_FIELD(UInt64, expiry, "expiresIn");
};
class ConfigDTO : public oatpp::DTO {
    DTO_INIT(ConfigDTO, DTO)

    DTO_FIELD(String, baseFolder, "folder");
    DTO_FIELD(UInt16, port, "port");
    DTO_FIELD(Boolean, cli, "cli");
    DTO_FIELD(Object<DatabaseConfigDTO>, database, "database");
    DTO_FIELD(Object<LogConfigDTO>, logs, "logs");
    DTO_FIELD(Object<JWTConfigDTO>, jwt, "jwt");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* ConfigDTO_hpp */