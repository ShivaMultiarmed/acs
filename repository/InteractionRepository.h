#include <chrono>
#include <memory>

#include "pqxx/connection.hxx"

enum class InteractionFailure : int {
    NOT_AUTHENTICATED, FORBIDDEN
};

enum class InteractionType : int {
    ENTER, EXIT
};

struct Interaction {
    uint64_t id, workerId, objectId, dateTime;
    InteractionType type;
    InteractionFailure failure;
};

class InteractionRepository {
    std::shared_ptr<pqxx::connection> con;
    const std::chrono::system_clock clock;
public:
    explicit InteractionRepository(std::shared_ptr<pqxx::connection> con);
    Interaction& interact(Interaction &interaction) const;
    Interaction& getLastByUserIdAndObjectId(
        const uint64_t userId,
        const uint64_t objectId
        ) const;

};
