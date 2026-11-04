#include "WorkerRepository.h"
#include "pqxx/nontransaction.hxx"

WorkerRepository::WorkerRepository(
    std::shared_ptr<pqxx::connection> con
) : con(std::move(con)) {
    con -> prepare(exists_by_id_query, "SELECT 1 FROM workers WHERE id = $1;");
}

bool WorkerRepository::exists_by_id(const uint64_t workerId) const {
    pqxx::nontransaction non(*con);
    std::optional<pqxx::row> row = non.exec(pqxx::prepped{exists_by_id_query}, pqxx::params{workerId}).opt_row();
    return row.has_value();
}