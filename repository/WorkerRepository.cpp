#include "WorkerRepository.h"
#include "pqxx/nontransaction.hxx"

WorkerRepository::WorkerRepository(
    std::shared_ptr<pqxx::connection> con
) : con(con) {
}

bool WorkerRepository::exists_by_id(const uint64_t workerId) const {
    pqxx::nontransaction non(*con);
    try {
        non.exec("SELECT 1 FROM workers WHERE id = $1;", pqxx::params{workerId}).one_row();
        return true;
    } catch (pqxx::unexpected_rows &e) {
        return false;
    }
}
