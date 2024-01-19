#include "ErrorHandler.hpp"

ErrorHandler::ErrorHandler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper> &objectMapper)
    : m_objectMapper(objectMapper) {
}

std::shared_ptr<ErrorHandler::OutgoingResponse>
ErrorHandler::handleError(const Status &status, const oatpp::String &message, const Headers &headers) {
    auto error = StatusDTO::createShared();
    auto outStatus = Status(status);

    error->message = message;

    if (outStatus.code >= 500) {
        OATPP_LOGE("HTTP Code " + std::to_string(status.code), "%s",message->c_str());  // Log the error
        if (error->message->find("DATABASE-ERROR") != std::string::npos) {
            if (error->message->find("duplicate key") != std::string::npos) {
                outStatus.code = 409;
                error->status = "Conflict";
                if (error->message->find("groups_name_key") != std::string::npos) {
                    error->message = "Group name is already taken";
                } else {
                    error->message = "Unknown conflict";
                }
            } else {
                error->message = "Internal server error";
                error->status = "Database error";
            }
        } else {
            error->message = "Internal server error";
        }
    } else {
        OATPP_LOGD("HTTP Code " + std::to_string(status.code), "%s", message->c_str());  // Log the error
        if (error->status == nullptr) {
            error->status = "Error";
        }
    }

    error->code = outStatus.code;
    auto response = ResponseFactory::createResponse(outStatus, error, m_objectMapper);

    for (const auto &pair : headers.getAll()) {
        response->putHeader(pair.first.toString(), pair.second.toString());
    }

    return response;
}