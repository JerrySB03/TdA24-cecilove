#include "Lecturer.hpp"

oatpp::postgresql::Uuid Lecturer::CreateLecturer(const oatpp::Object<LecturerDto> &lecturerDto) {
    auto insertedModel = LecturerModel::createShared();
    insertedModel->title_before = lecturerDto->title_before;
    insertedModel->first_name = lecturerDto->first_name;
    insertedModel->middle_name = lecturerDto->middle_name;
    insertedModel->last_name = lecturerDto->last_name;
    insertedModel->title_after = lecturerDto->title_after;
    insertedModel->picture_url = lecturerDto->picture_url;
    insertedModel->location = lecturerDto->location;    
    insertedModel->claim = lecturerDto->claim;
    insertedModel->bio = lecturerDto->bio;
    for (auto &tag : *lecturerDto->tags) {
        insertedModel->tag_names->push_back(tag->name);
    }
    insertedModel->price_per_hour = lecturerDto->price_per_hour;
    for (auto &phone : *lecturerDto->contact->telephone_numbers) {
        insertedModel->contact_phones->push_back(phone);
    }
    for (auto &email : *lecturerDto->contact->emails) {
        insertedModel->contact_emails->push_back(email);
    }

    auto result = lecturerDb->createLecturer(insertedModel);
    OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage()->c_str());
    
    OATPP_ASSERT_HTTP(result->hasMoreToFetch(), Status::CODE_500, "Unknown error");
    auto resultData = result->fetch<oatpp::Vector<oatpp::Vector<oatpp::postgresql::Uuid>>>();
    OATPP_ASSERT_HTTP(resultData->size() == 1, Status::CODE_500, "Unknown error");
    OATPP_ASSERT_HTTP(resultData[0]->size() == 1, Status::CODE_500, "Unknown error");
    return resultData[0][0];
}
oatpp::Object<LecturerDto> Lecturer::GetLecturerById(const oatpp::postgresql::Uuid &id){
    OATPP_LOGD("Lecturer", "Id: %s", id->toString()->c_str());
    auto result = lecturerDb->getLecturerById(id);
    OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage()->c_str());
    OATPP_ASSERT_HTTP(result->hasMoreToFetch(), Status::CODE_500, "Unknown error");
    auto resultData = result->fetch<oatpp::Vector<oatpp::Object<LecturerModel>>>();
    OATPP_ASSERT_HTTP(resultData->size() != 0, Status::CODE_500, "User not found");
    auto lecturer = resultData[0];
    auto lecturerDto = LecturerDto::createShared();
    lecturerDto->uuid = lecturer->uuid->toString();
    lecturerDto->title_before = lecturer->title_before;
    lecturerDto->first_name = lecturer->first_name;
    lecturerDto->middle_name = lecturer->middle_name;
    lecturerDto->last_name = lecturer->last_name;
    lecturerDto->title_after = lecturer->title_after;
    lecturerDto->picture_url = lecturer->picture_url;
    lecturerDto->location = lecturer->location;
    lecturerDto->claim = lecturer->claim;
    lecturerDto->bio = lecturer->bio;
    OATPP_ASSERT_HTTP(lecturer->tag_names->size() == lecturer->tag_uuids->size(), Status::CODE_500, "Unknown error");
    for(size_t i = 0; i < lecturer->tag_names->size(); i++){
        auto tag = LecturarTagDto::createShared();
        tag->name = lecturer->tag_names->at(i);
        tag->uuid = lecturer->tag_uuids->at(i)->toString();
        lecturerDto->tags->push_back(tag);
    }
    lecturerDto->price_per_hour = lecturer->price_per_hour;
    for (auto &phone : *lecturer->contact_phones.get()) {
        lecturerDto->contact->telephone_numbers->push_back(phone);
    }
    for (auto &email : *lecturer->contact_emails.get()) {
        lecturerDto->contact->emails->push_back(email);
    }
    return lecturerDto;
}