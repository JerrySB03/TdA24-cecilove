#ifndef DTOs_hpp
#define DTOs_hpp

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class HelloDTO : public oatpp::DTO {
    DTO_INIT(HelloDTO, DTO)

    DTO_FIELD(String, secret);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */