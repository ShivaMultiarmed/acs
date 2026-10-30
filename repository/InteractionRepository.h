#include <chrono>
#include <memory>

#include "pqxx/connection.hxx"

enum class InteractionFailure {
    NOT_AUTHENTICATED, FORBIDDEN
};

class InteractionRepository {
    std::shared_ptr<pqxx::connection> con;
    const std::chrono::system_clock clock;
public:
    explicit InteractionRepository(std::shared_ptr<pqxx::connection> con);
    bool interact(
        const uint64_t *userId,
        const uint64_t objectId,
        const InteractionFailure *failure = nullptr) const;
};
