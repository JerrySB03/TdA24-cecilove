#ifndef DefaultController_hpp
#define DefaultController_hpp

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "Dto/DefaultDto.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)  // <- Begin Codegen

class DefaultController : public oatpp::web::server::api::ApiController {
    /*
     * ENDPOINT_INFO macro adds info about the endpoint to SwaggerUI documentation
     * ENDPOINT macro creates controller's endpoint
     */
   public:
    DefaultController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper) {
    }
    static std::shared_ptr<DefaultController> createShared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)  // Inject objectMapper component here as default parameter
    ) {
        return std::make_shared<DefaultController>(objectMapper);
    }
    ENDPOINT_INFO(apiRoot) {
        info->summary = "Test API endpoint";

        info->addResponse<Object<HelloDTO>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/", apiRoot) {
        auto dto = HelloDTO::createShared();
        dto->secret = "The cake is a lie";
        return createDtoResponse(Status::CODE_200, dto);
    }
};

#include OATPP_CODEGEN_END(ApiController)

#endif /* DefaultController_hpp */