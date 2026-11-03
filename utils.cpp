#include "utils.h"

#include "repository/InteractionRepository.h"

std::string to_string(const InteractionFailure failure) {
    switch (failure) {
        case InteractionFailure::NONE: return "NONE";
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

Interaction to_interaction(json data) {
    Interaction interaction{
        0,
        data["worker_id"].get<uint64_t>(),
        data["object_id"].get<uint64_t>(),
        0,
        to_interaction_type(data["interaction"].get<std::string>())
    };
    return interaction;
}

InteractionType to_interaction_type(const std::string type) {
    if (type == "ENTER") return InteractionType::ENTER;
    return InteractionType::EXIT;
}

std::string to_string(const InteractionType type) {
    if (type == InteractionType::ENTER) return "ENTER";
    return "EXIT";
}

json to_json(const Interaction &interaction) {
    return json::object({
        {"id", interaction.id},
        {"worker_id", interaction.worker_id},
        {"object_id", interaction.object_id},
        {"date_time", interaction.date_time},
        {"type", to_string(interaction.type)},
        {"failure", to_string(interaction.failure)}
    });
}

Interaction to_interaction(const pqxx::row &row) {
    Interaction interaction{};
    interaction.id = row["id"].as<uint64_t>();
    interaction.worker_id = row["worker_id"].as<uint64_t>();
    interaction.object_id = row["object_id"].as<uint64_t>();
    interaction.date_time = to_millis(row["date_time"].as<std::string>());
    interaction.type = to_interaction_type(row["interaction"].as<std::string>());
    interaction.failure = to_interaction_failure(row["failure"].as<std::string>());
    return interaction;
}

InteractionFailure to_interaction_failure(const std::string failure) {
    if (failure == "NONE") return InteractionFailure::NONE;
    if (failure == "NOT_AUTHENTICATED") return InteractionFailure::NOT_AUTHENTICATED;
    if (failure == "NOT_FOUND") return InteractionFailure::NOT_FOUND;
    if (failure == "FORBIDDEN") return InteractionFailure::FORBIDDEN;
    return InteractionFailure::NOT_EXPECTED;
}

std::uint64_t to_millis(const std::string& date_time) {
    std::tm tm{};
    std::istringstream in(date_time);
    in >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S+00");
    if (in.fail())
        throw std::invalid_argument("bad datetime: " + date_time);
    #ifdef _WIN32
    std::time_t t = _mkgmtime(&tm);   // tm как UTC → epoch (Windows)
    #else
    std::time_t t = timegm(&tm);      // то же на Linux
    #endif
    return static_cast<uint64_t>(t);
}