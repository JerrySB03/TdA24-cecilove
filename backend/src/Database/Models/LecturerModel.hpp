#ifndef LecturerModel_hpp
#define LecturerModel_hpp

#include <oatpp-postgresql/Types.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class LecturerModel : public oatpp::DTO {
    DTO_INIT(LecturerModel, oatpp::DTO);

    DTO_FIELD_INFO(uuid) { info->description = "Universally unique identifier of the lecturer"; }
    DTO_FIELD_INFO(title_before) { info->description = "Title before the name of the lecturer"; }
    DTO_FIELD_INFO(first_name) { info->description = "First name of the lecturer"; }
    DTO_FIELD_INFO(middle_name) { info->description = "Middle name of the lecturer"; }
    DTO_FIELD_INFO(last_name) { info->description = "Last name of the lecturer"; }
    DTO_FIELD_INFO(title_after) { info->description = "Title after the name of the lecturer"; }
    DTO_FIELD_INFO(picture_url) { info->description = "URL of the lecturer's picture"; }
    DTO_FIELD_INFO(location) { info->description = "Location of the lecturer"; }
    DTO_FIELD_INFO(claim) { info->description = "Claim of the lecturer"; }
    DTO_FIELD_INFO(bio) { info->description = "Biography of the lecturer"; }
    DTO_FIELD_INFO(tag_names) { info->description = "Tags of the lecturer"; }
    DTO_FIELD_INFO(tag_uuids) { info->description = "UUIDs of the tags of the lecturer"; }
    DTO_FIELD_INFO(price_per_hour) { info->description = "Price per hour of the lecturer"; }
    DTO_FIELD_INFO(contact_phones) { info->description = "Contact phones of the lecturer"; }
    DTO_FIELD_INFO(contact_emails) { info->description = "Contact emails of the lecturer"; }
    DTO_FIELD(oatpp::postgresql::Uuid, uuid);
    DTO_FIELD(String, title_before) = "";
    DTO_FIELD(String, first_name);
    DTO_FIELD(String, middle_name) = "";
    DTO_FIELD(String, last_name);
    DTO_FIELD(String, title_after) = "";
    DTO_FIELD(String, picture_url);
    DTO_FIELD(String, location);
    DTO_FIELD(String, claim);
    DTO_FIELD(String, bio);
    DTO_FIELD(Vector<String>::ObjectWrapper, tag_names) = Vector<String>::createShared();
    DTO_FIELD(Vector<oatpp::postgresql::Uuid>::ObjectWrapper, tag_uuids) = Vector<oatpp::postgresql::Uuid>::createShared();
    DTO_FIELD(Int32, price_per_hour);
    DTO_FIELD(Vector<String>::ObjectWrapper, contact_phones) = Vector<String>::createShared();
    DTO_FIELD(Vector<String>::ObjectWrapper, contact_emails) = Vector<String>::createShared();
};

#include OATPP_CODEGEN_END(DTO)

#endif /* LecturerModel_hpp */