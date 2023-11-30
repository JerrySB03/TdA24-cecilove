#ifndef AppComponent_hpp
#define AppComponent_hpp

#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/core/utils/ConversionUtils.hpp>
#include <oatpp/network/ConnectionPool.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/web/server/interceptor/AllowCorsGlobal.hpp>

#include "Config/Config.hpp"
#include "Database/DatabaseComponent.hpp"
#include "ErrorHandler/ErrorHandler.hpp"
#include "SwaggerComponent.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
   private:
    oatpp::base::CommandLineArguments m_cmdArgs;

   public:
    // Get command line arguments
    AppComponent(const oatpp::base::CommandLineArguments &cmdArgs)
        : m_cmdArgs(cmdArgs) {
    }
    AppComponent() = default;
    OATPP_CREATE_COMPONENT(std::shared_ptr<Config>, config)
    ([this] {
        return std::make_shared<Config>(this->m_cmdArgs);
    }());
    /**
     * Swagger component
     */
    SwaggerComponent swaggerComponent;
    /**
     * Database component
     */
    DatabaseComponent databaseComponent;

    /**
     * Create ObjectMapper component to serialize/deserialize DTOs in Controller's API
     */

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)
    ([] {
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    objectMapper->getSerializer()->getConfig()->includeNullFields = false;
    objectMapper->getSerializer()->getConfig()->alwaysIncludeNullCollectionElements = true;
    objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
    return objectMapper; }());

    /**
     *  Create ConnectionProvider component which listens on the port
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)
    ([] {
        OATPP_COMPONENT(std::shared_ptr<Config>, config);
        return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", config->GetPort(), oatpp::network::Address::IP_4}, true);
    }());

    /**
     *  Create Router component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)
    ([] { return oatpp::web::server::HttpRouter::createShared(); }());

    /**
     *  Create ConnectionHandler component which uses Router component to route requests
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)
    ([] {
     OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);           // get Router component
     OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper); // get ObjectMapper component

     auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
     connectionHandler->setErrorHandler(std::make_shared<ErrorHandler>(objectMapper));

     connectionHandler->addResponseInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowCorsGlobal>());

     return connectionHandler; }());
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionPool>, connectionPool)
    ([] {
        OATPP_COMPONENT(std::shared_ptr<Config>, config);
        auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({"127.0.0.1", config->GetPort(), oatpp::network::Address::IP_4});
        auto connectionPool = oatpp::network::ClientConnectionPool::createShared(
            connectionProvider /* connection provider */,
            10 /* max connections */,
            std::chrono::seconds(5) /* max lifetime of idle connection */
        );
        return connectionPool;
    }());
};

#endif /* AppComponent_hpp */                                                                                                                                                                                                