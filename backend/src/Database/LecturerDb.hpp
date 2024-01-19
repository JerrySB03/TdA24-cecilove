#ifndef LecturerDB_hpp
#define LecturerDB_hpp

#include <oatpp-postgresql/orm.hpp>

#include "Config/Config.hpp"
#include "Models/LecturerModel.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient)

class LecturerDb : public oatpp::orm::DbClient {
   public:
    LecturerDb(const std::shared_ptr<oatpp::orm::Executor> &executor)
        : oatpp::orm::DbClient(executor) {
        OATPP_COMPONENT(std::shared_ptr<Config>, config);
        oatpp::orm::SchemaMigration migration(executor, "Lecturer");
        migration.addFile(1, config->GetDatabaseMigrationsPath() + "/lecturer/001_init.sql");
        migration.migrate();

        auto version = executor->getSchemaVersion();
        OATPP_LOGD("LecturerDb", "Migration - OK. Version=%ld.", version);
    }

    QUERY(createLecturer, 
        "SELECT add_lecturer("
        ":lecturer.title_before, "
        ":lecturer.first_name, "
        ":lecturer.middle_name, "
        ":lecturer.last_name, "
        ":lecturer.title_after, "
        ":lecturer.picture_url, "
        ":lecturer.location, "
        ":lecturer.claim, "
        ":lecturer.bio, "
        ":lecturer.tag_names, "
        ":lecturer.price_per_hour, "    
        ":lecturer.contact_phones, "
        ":lecturer.contact_emails "
        ");",
        PARAM(oatpp::Object<LecturerModel>, lecturer))
    
//     CREATE TABLE IF NOT EXISTS Lecturers (
//     uuid UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
//     title_before VARCHAR(10),
//     first_name VARCHAR(50) NOT NULL,
//     middle_name VARCHAR(50),
//     last_name VARCHAR(50) NOT NULL,
//     title_after VARCHAR(10),
//     picture_url VARCHAR(255),
//     location VARCHAR(255),
//     claim VARCHAR(255),
//     bio TEXT,
//     tags UUID [] REFERENCES LecturerTags(uuid),
//     price_per_hour INTEGER,
//     contact_phones VARCHAR(255) [],
//     contact_emails VARCHAR(255) []
// );
// CREATE TABLE IF NOT EXISTS LecturerTags(
//     uuid UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
//     name VARCHAR(50) NOT NULL UNIQUE
// );
    // Select lecturer data by id with tag uuids and names
    QUERY(getLecturerById,
        "SELECT "
        "lecturers.uuid, "
        "lecturers.title_before, "
        "lecturers.first_name, "
        "lecturers.middle_name, "
        "lecturers.last_name, "
        "lecturers.title_after, "
        "lecturers.picture_url, "
        "lecturers.location, "
        "lecturers.claim, "
        "lecturers.bio, "
        "lecturers.tags AS tag_uuids, "
        "(SELECT array_agg(LecturerTags.name) FROM LecturerTags WHERE LecturerTags.uuid = ANY(lecturers.tags)) AS tag_names, "
        "lecturers.price_per_hour, "
        "lecturers.contact_phones, "
        "lecturers.contact_emails "
        "FROM lecturers "
        "WHERE lecturers.uuid = :id;",
        PARAM(oatpp::postgresql::Uuid, id))
};

#include OATPP_CODEGEN_END(DbClient)

#endif /* LecturerDB_hpp */