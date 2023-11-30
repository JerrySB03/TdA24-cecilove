#ifndef DefaultDB_hpp
#define DefaultDB_hpp

#include <oatpp-sqlite/orm.hpp>

#include "Config/Config.hpp"
#include "Models/DefaultModel.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient)

class DefaultDb : public oatpp::orm::DbClient {
   public:
    DefaultDb(const std::shared_ptr<oatpp::orm::Executor> &executor)
        : oatpp::orm::DbClient(executor) {
        OATPP_COMPONENT(std::shared_ptr<Config>, config);
        oatpp::orm::SchemaMigration migration(executor, "Default");
        migration.addFile(1, config->GetDatabaseMigrationsPath() + "/default/001_init.sql");
        migration.migrate();

        auto version = executor->getSchemaVersion();
        OATPP_LOGD("DefaultDb", "Migration - OK. Version=%ld.", version);
    }
};

#include OATPP_CODEGEN_END(DbClient)

#endif /* DefaultDB_hpp */