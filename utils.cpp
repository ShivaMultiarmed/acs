#include "utils.h"

#include "repository/InteractionRepository.h"

std::string toString(const InteractionFailure failure) {
    switch (failure) {
        case InteractionFailure::NOT_AUTHENTICATED: return "NOT_AUTHENTICATED";
        case InteractionFailure::NOT_FOUND: return "NOT_FOUND";
        case InteractionFailure::FORBIDDEN: return "FORBIDDEN";
        default: return "NOT_EXPECTED";
    }
}

uint64_t to_millis(const std::chrono::time_point<std::chrono::system_clock> &point) {
    return std::chrono::system_clock::to_time_t(point);
}

std::string toDateTimeString(const uint64_t millis) {
    time_t ms = millis;
    tm *utcTime = gmtime(&ms);
    std::stringstream dateTime;
    dateTime << std::put_time(utcTime, "%Y-%m-%dT%H:%M:%SZ");
    std::string dt = dateTime.str();
    return dt;
}

std::string toString(const std::chrono::time_point<std::chrono::system_clock> &point) {
    time_t millis = to_millis(point);
    return toDateTimeString(millis);
}

Interaction mapToInteraction(const pqxx::row &row) {
    Interaction interaction{};
    interaction.id = row["id"].as<uint64_t>();
    interaction.worker_id = row["worker_id"].as<uint64_t>();
    interaction.object_id = row["object_id"].as<uint64_t>();
    interaction.date_time = row["date_time"].as<uint64_t>();
    // TODO map enums
    return interaction;
}
