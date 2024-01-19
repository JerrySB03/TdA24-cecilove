#ifndef DatabaseComponent_hpp
#define DatabaseComponent_hpp

#include "Config/Config.hpp"
#include "LecturerDb.hpp"

class DatabaseComponent {
   public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::postgresql::ConnectionPool>, connectionPool)
    ([] {
        OATPP_COMPONENT(std::shared_ptr<Config>, config);
        auto connectionProvider = std::make_shared<oatpp::postgresql::ConnectionProvider>(config->GetDatabaseConnectionString());
        // Test and retry the database connection
        for (int i = 0; i < 12; i++) {
            OATPP_LOGV("Database", "Trying to connect to database...");
            try {
                if (connectionProvider->get() != nullptr) {
                    OATPP_LOGV("Database", "Connected to database");
                    break;
                } else {
                    OATPP_LOGV("Database", "Database connection failed");
                }
            } catch (const std::exception& e) {
                OATPP_LOGE("Database", "Failed to connect to database:\n%s", e.what());
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        return oatpp::postgresql::ConnectionPool::createShared(connectionProvider,
                                                               config->GetDatabaseMaxConnections(),  // max connections
                                                               std::chrono::seconds(5) /* Connection time to live*/);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::postgresql::Executor>, databaseExecutor)
    ([] {
        /* get connection pool component */
        OATPP_COMPONENT(std::shared_ptr<oatpp::postgresql::ConnectionPool>, connectionPool);

        /* Create database-specific Executor */
        return std::make_shared<oatpp::postgresql::Executor>(connectionPool);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<LecturerDb>, lecturerDb)
    ([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::postgresql::Executor>, databaseExecutor);
        // Get DB executor component and create a database client
        return std::make_shared<LecturerDb>(databaseExecutor);
    }());
};

#endif /* DatabaseComponent_hpp */