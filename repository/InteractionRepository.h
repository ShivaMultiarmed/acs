#pragma once
#include <chrono>
#include <memory>
#include "pqxx/connection.hxx"
#include <optional>

enum class InteractionFailure : int {
    NOT_AUTHENTICATED, NOT_FOUND, FORBIDDEN, NOT_EXPECTED
};

enum class InteractionType : int {
    ENTER, EXIT
};

struct Interaction {
    std::optional<uint64_t> id = std::nullopt, date_time = std::nullopt;
    uint64_t worker_id, object_id;
    InteractionType type;
    std::optional<InteractionFailure> failure = std::nullopt;
};

class InteractionRepository {
    std::shared_ptr<pqxx::connection> con;
    std::chrono::system_clock clock;
public:
    explicit InteractionRepository(std::shared_ptr<pqxx::connection> con);
    Interaction& interact(Interaction &interaction) const;
    std::optional<Interaction> getLastByUserIdAndObjectId(
        const uint64_t worker_id,
        const uint64_t object_id
        ) const;

};