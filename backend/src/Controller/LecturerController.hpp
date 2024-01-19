#ifndef LecturerController_hpp
#define LecturerController_hpp

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "Dto/LecturerDto.hpp"
#include "Lecturer/Lecturer.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)  // <- Begin Codegen

class LecturerController : public oatpp::web::server::api::ApiController {
    /*
     * ENDPOINT_INFO macro adds info about the endpoint to SwaggerUI documentation
     * ENDPOINT macro creates controller's endpoint
     */
   private:
    Lecturer lecturer;

   public:
    LecturerController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper) {
    }
    static std::shared_ptr<LecturerController> createShared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)  // Inject objectMapper component here as default parameter
    ) {
        return std::make_shared<LecturerController>(objectMapper);
    }
    ENDPOINT("POST", "/lecturers", createLecturer,
             BODY_DTO(Object<LecturerDto>, lecturerDto)) {
        return createDtoResponse(Status::CODE_200, lecturer.GetLecturerById(lecturer.CreateLecturer(lecturerDto)));
    }
};

#include OATPP_CODEGEN_END(ApiController)

#endif /* LecturerController_hpp */