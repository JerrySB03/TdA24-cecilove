#ifndef LecturerDto_hpp
#define LecturerDto_hpp

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class ContactInfoDto : public oatpp::DTO {
    DTO_INIT(ContactInfoDto, DTO)
    DTO_FIELD(List<String>, telephone_numbers, "telephone_numbers") = List<String>::createShared();
    DTO_FIELD(List<String>, emails, "emails") = List<String>::createShared();
};
class LecturarTagDto : public oatpp::DTO {
    DTO_INIT(LecturarTagDto, DTO)
    DTO_FIELD(String, name);
    DTO_FIELD(String, uuid);
};

class LecturerDto : public oatpp::DTO {
    DTO_INIT(LecturerDto, DTO)

    DTO_FIELD(String, uuid);
    DTO_FIELD(String, title_before, "title_before");
    DTO_FIELD(String, first_name, "first_name");
    DTO_FIELD(String, middle_name, "middle_name");
    DTO_FIELD(String, last_name, "last_name");
    DTO_FIELD(String, title_after, "title_after");
    DTO_FIELD(String, picture_url, "picture_url");
    DTO_FIELD(String, location, "location");
    DTO_FIELD(String, claim, "claim");
    DTO_FIELD(String, bio, "bio");
    DTO_FIELD(List<Object<LecturarTagDto>>, tags, "tags") = List<Object<LecturarTagDto>>::createShared();
    DTO_FIELD(Int32, price_per_hour, "price_per_hour");
    DTO_FIELD(Object<ContactInfoDto>, contact, "contact") = ContactInfoDto::createShared();
};

#include OATPP_CODEGEN_END(DTO)

#endif /* LecturerDto_hpp */