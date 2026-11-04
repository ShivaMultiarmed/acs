#include "utils.h"

#include <regex>

#include "repository/InteractionRepository.h"

std::string to_string(const InteractionFailure failure) {
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

Interaction to_interaction(json data) {
    return {
        std::nullopt,
        0,
        data["worker_id"].get<uint64_t>(),
        data["object_id"].get<uint64_t>(),
        to_interaction_type(data["interaction"].get<std::string>()),
        std::nullopt
    };
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
        {"id", interaction.id ? std::to_string(*interaction.id) : "null"},
        {"worker_id", interaction.worker_id},
        {"object_id", interaction.object_id},
        {"date_time", interaction.date_time},
        {"type", to_string(interaction.type)},
        {"failure", interaction.failure ? to_string(*interaction.failure) : std::string("null")}
    });
}

Interaction to_interaction(const pqxx::row &row) {
    Interaction interaction{};
    interaction.id = row["id"].as<uint64_t>();
    interaction.worker_id = row["worker_id"].as<uint64_t>();
    interaction.object_id = row["object_id"].as<uint64_t>();
    interaction.date_time = to_millis(row["date_time"].as<std::string>());
    interaction.failure = row["failure"]
        .as<std::optional<std::string>>()
        .transform([](std::string s) {
            return to_interaction_failure(std::move(s));
        });
    interaction.type = to_interaction_type(row["interaction"].as<std::string>());
    return interaction;
}

InteractionFailure to_interaction_failure(const std::string failure) {
    if (failure == "NOT_AUTHENTICATED") return InteractionFailure::NOT_AUTHENTICATED;
    if (failure == "NOT_FOUND") return InteractionFailure::NOT_FOUND;
    if (failure == "FORBIDDEN") return InteractionFailure::FORBIDDEN;
    return InteractionFailure::NOT_EXPECTED;
}

std::uint64_t to_millis(const std::string& date_time) {
    std::istringstream iss{date_time};
    std::chrono::sys_time<std::chrono::milliseconds> tp;

    iss >> std::chrono::parse("%FT%T%z", tp);

    if (iss.fail()) {
        throw std::runtime_error("Failed to parse ISO string");
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(
        tp.time_since_epoch()
    ).count();
}