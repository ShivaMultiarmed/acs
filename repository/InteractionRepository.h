#pragma once
#include <chrono>
#include <memory>
#include "pqxx/connection.hxx"

enum class InteractionFailure : int {
    NONE, NOT_AUTHENTICATED, NOT_FOUND, FORBIDDEN, NOT_EXPECTED
};

enum class InteractionType : int {
    ENTER, EXIT
};

struct Interaction {
    uint64_t id, worker_id, object_id, date_time;
    InteractionType type;
    InteractionFailure failure;
};

class InteractionRepository {
    std::shared_ptr<pqxx::connection> con;
    std::chrono::system_clock clock;
public:
    explicit InteractionRepository(std::shared_ptr<pqxx::connection> con);
    Interaction& interact(Interaction &interaction) const;
    Interaction getLastByUserIdAndObjectId(
        const uint64_t userId,
        const uint64_t objectId
        ) const;

};