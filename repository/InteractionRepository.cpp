#include "InteractionRepository.h"
#include <chrono>
#include <iomanip>
#include "../utils.h"
#include "pqxx/nontransaction.hxx"

InteractionRepository::InteractionRepository(std::shared_ptr<pqxx::connection> con) {
    this->con = con;
}

bool InteractionRepository::interact(
    const uint64_t *userId,
    const uint64_t objectId,
    const InteractionFailure *failure
) const {
    pqxx::nontransaction non(*con);
    const char *userIdArgument = userId ? std::to_string(*userId).c_str() : nullptr;
    const char *failureArgument = failure ? toString(*failure).c_str() : nullptr;
    const auto now = clock.now();
    const time_t millis = std::chrono::system_clock::to_time_t(now);
    const tm *utcTime = gmtime(&millis);
    std::stringstream dateTime;
    dateTime << std::put_time(utcTime, "%Y-%m-%dT%H:%M:%SZ");
    try {
        non.exec_params1(
        "INSERT INTO interactions (user_id, object_id, date_time, failure) VALUES ($1, $2, $3, $4);",
        userIdArgument, objectId, dateTime.str(), failureArgument);
        return true;
    } catch (pqxx::unexpected_rows &e) {
        return false;
    }
}