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
    const std::string now = toDateTimeString(interaction.date_time);
    pqxx::row row = non.exec_params1(
        "INSERT INTO interactions (worker_id, object_id, date_time, interaction, failure) VALUES ($1, $2, $3, $4, $5) RETURNING *;",
        interaction.worker_id, interaction.object_id, now, to_string(interaction.type), to_string(interaction.failure));
    interaction.id = row["id"].as<uint64_t>();
    return interaction;
}

Interaction InteractionRepository::getLastByUserIdAndObjectId(
    const uint64_t worker_id,
    const uint64_t object_id
) const {
    pqxx::nontransaction non(*con);
    std::string query = R"(SELECT id, worker_id, object_id, date_time, interaction, failure FROM interactions
    WHERE worker_id = $1 AND object_id = $2 ORDER BY date_time DESC;)";
    pqxx::result rows = non.exec_params(query, worker_id, object_id);
    if (rows.empty()) return {};
    return to_interaction(rows[0]);
}
