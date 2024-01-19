CREATE EXTENSION IF NOT EXISTS "uuid-ossp";
CREATE TABLE IF NOT EXISTS Lecturers (
    uuid UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    title_before VARCHAR(10),
    first_name VARCHAR(50) NOT NULL,
    middle_name VARCHAR(50),
    last_name VARCHAR(50) NOT NULL,
    title_after VARCHAR(10),
    picture_url VARCHAR(255),
    location VARCHAR(255),
    claim VARCHAR(255),
    bio TEXT,
    tags UUID [],
    price_per_hour INTEGER,
    contact_phones VARCHAR(255) [],
    contact_emails VARCHAR(255) []
);
CREATE TABLE IF NOT EXISTS LecturerTags(
    uuid UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(50) NOT NULL UNIQUE
);
CREATE OR REPLACE FUNCTION add_lecturer(
        IN title_before VARCHAR(10),
        IN first_name VARCHAR(50),
        IN middle_name VARCHAR(50),
        IN last_name VARCHAR(50),
        IN title_after VARCHAR(10),
        IN picture_url VARCHAR(255),
        IN location VARCHAR(255),
        IN claim VARCHAR(255),
        IN bio TEXT,
        IN tags VARCHAR(50) [],
        IN price_per_hour INTEGER,
        IN contact_phones VARCHAR(255) [],
        IN contact_emails VARCHAR(255) []
    ) RETURNS uuid LANGUAGE plpgsql AS $$
DECLARE lecturer_uuid uuid;
BEGIN -- Check that the 
-- Insert tag names and select lecturer tag uuids
INSERT INTO LecturerTags (name)
SELECT DISTINCT UNNEST(tags) AS name ON CONFLICT (name) DO NOTHING;
-- Select lecturer tag uuids
WITH tag_uuids AS (
    SELECT uuid
    FROM LecturerTags
    WHERE name = ANY(tags)
) -- Insert lecturer data with tag uuids
INSERT INTO Lecturers (
        title_before,
        first_name,
        middle_name,
        last_name,
        title_after,
        picture_url,
        location,
        claim,
        bio,
        tags,
        price_per_hour,
        contact_phones,
        contact_emails
    )
VALUES (
        title_before,
        first_name,
        middle_name,
        last_name,
        title_after,
        picture_url,
        location,
        claim,
        bio,
        (
            SELECT ARRAY(
                    SELECT uuid
                    FROM tag_uuids
                )
        ),
        price_per_hour,
        contact_phones,
        contact_emails
    )
RETURNING uuid INTO lecturer_uuid;
RAISE NOTICE 'Generated UUID: %', lecturer_uuid;
RETURN lecturer_uuid;
END;
$$;
