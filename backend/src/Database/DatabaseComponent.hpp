#ifndef DatabaseComponent_hpp
#define DatabaseComponent_hpp

#include "Config/Config.hpp"
#include "DefaultDb.hpp"

class DatabaseComponent {
   public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::sqlite::ConnectionPool>, connectionPool)
    ([] {
        OATPP_COMPONENT(std::shared_ptr<Config>, config);
        auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(config->GetDatabaseConnectionString());
        if(connectionProvider->get() == nullptr){
            OATPP_LOGE("Database", "Couldn't connect");
            exit(1);
        }
        return oatpp::sqlite::ConnectionPool::createShared(connectionProvider,
                                                               config->GetDatabaseMaxConnections(),  // max connections
                                                               std::chrono::seconds(5) /* Connection time to live*/);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::sqlite::Executor>, databaseExecutor)
    ([] {
        /* get connection pool component */
        OATPP_COMPONENT(std::shared_ptr<oatpp::sqlite::ConnectionPool>, connectionPool);

        /* Create database-specific Executor */
        return std::make_shared<oatpp::sqlite::Executor>(connectionPool);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<DefaultDb>, defaultDb)
    ([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::sqlite::Executor>, databaseExecutor);
        // Get DB executor component and create a database client
        return std::make_shared<DefaultDb>(databaseExecutor);
    }());
};

#endif /* DatabaseComponent_hpp */