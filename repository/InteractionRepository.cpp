#include "InteractionRepository.h"
#include <chrono>
#include "../utils.h"
#include "pqxx/nontransaction.hxx"

InteractionRepository::InteractionRepository(
    std::shared_ptr<pqxx::connection> con
) : con(con) {
    clock = std::chrono::system_clock();
    con -> prepare(interact_query,
        "INSERT INTO interactions (worker_id, object_id, date_time, interaction, failure)"
        " VALUES ($1, $2, $3, $4, $5) RETURNING id;"
    );
    con -> prepare(last_by_user_and_object, "SELECT id, worker_id, object_id, date_time, interaction, failure FROM interactions"
    "WHERE worker_id = $1 AND object_id = $2 ORDER BY date_time DESC;");
}

Interaction &InteractionRepository::interact(Interaction &interaction) const {
    pqxx::nontransaction non(*con);
    const std::string now = to_date_time_string(interaction.date_time.value());
    pqxx::field id_field = non.exec(
        pqxx::prepped{interact_query},
        pqxx::params{
            interaction.worker_id,
            interaction.object_id,
            now,
            to_string(interaction.type),
            interaction.failure.transform([](const InteractionFailure &failure){ return to_string(failure); })
        }
    ).one_field();
    interaction.id = id_field.as<uint64_t>();
    return interaction;
}

std::optional<Interaction> InteractionRepository::getLastByUserIdAndObjectId(
    const uint64_t worker_id,
    const uint64_t object_id
) const {
    pqxx::nontransaction non(*con);
    return non.exec(
        pqxx::prepped{last_by_user_and_object},
        pqxx::params{worker_id, object_id}
    ).opt_row()
    .transform([](pqxx::row row){ return to_interaction(row);});
}
