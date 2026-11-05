#include "WorkerRepository.h"
#include "pqxx/nontransaction.hxx"

WorkerRepository::WorkerRepository(
    std::shared_ptr<DbPool> pool
) : pool(std::move(pool)) {
    for (uint8_t i = 0; i < pool->con_count; ++i)
    {
        pool ->get_con().prepare(exists_by_id_query, "SELECT 1 FROM workers WHERE id = $1;");
    }
}

bool WorkerRepository::exists_by_id(const uint64_t workerId) const {
    pqxx::nontransaction non(pool -> get_con());
    std::optional<pqxx::row> row = non.exec(pqxx::prepped{exists_by_id_query}, pqxx::params{workerId}).opt_row();
    return row.has_value();
}