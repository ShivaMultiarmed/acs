#include "InteractionRepository.h"
#include <chrono>
#include <iomanip>
#include "../utils.h"
#include "pqxx/nontransaction.hxx"

InteractionRepository::InteractionRepository(
    std::shared_ptr<pqxx::connection> con
) : con(con) {
    clock = std::chrono::system_clock();
}

Interaction &InteractionRepository::interact(Interaction &interaction) const {
    pqxx::nontransaction non(*con);
    const char *failureArgument = interaction.failure == (InteractionFailure) NULL
                                      ? toString(interaction.failure).c_str()
                                      : nullptr;
    const std::string now = toString(clock.now());
    pqxx::row row = non.exec_prepared1(
        "INSERT INTO interactions (worker_id, object_id, date_time, failure) VALUES ($1, $2, $3, $4);",
        interaction.worker_id, interaction.object_id, now, failureArgument);
    interaction.id = row["id"].as<uint64_t>();
    return interaction;
}

Interaction InteractionRepository::getLastByUserIdAndObjectId(
    const uint64_t userId,
    const uint64_t objectId
) const {
    pqxx::nontransaction non(*con);
    std::string query = R"(SELECT id, worker_id, object_id, date_time, interaction_type, failure FROM interactions
    WHERE userId = $1 AND object_id = $2 ORDER BY date_time DESC;)";
    pqxx::row row = non.exec_prepared1(query, userId, objectId);
    return mapToInteraction(row);
}
