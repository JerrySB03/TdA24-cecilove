#ifndef Lecturer_hpp
#define Lecturer_hpp

#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp-postgresql/Types.hpp>

#include "Database/LecturerDb.hpp"
#include "Database/Models/LecturerModel.hpp"
#include "Dto/LecturerDto.hpp"

class Lecturer {
   private:
    OATPP_COMPONENT(std::shared_ptr<LecturerDb>, lecturerDb);
    using Status = oatpp::web::protocol::http::Status;

   public:
    Lecturer() = default;
    ~Lecturer() = default;
    oatpp::postgresql::Uuid CreateLecturer(const oatpp::Object<LecturerDto> &lecturerDto);
    oatpp::Object<LecturerDto> GetLecturerById(const oatpp::postgresql::Uuid &id);
};

#endif /* Lecturer_hpp */