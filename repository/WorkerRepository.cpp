#include "WorkerRepository.h"
#include "pqxx/nontransaction.hxx"
#include "pqxx/transaction.hxx"

WorkerRepository::WorkerRepository(std::shared_ptr<pqxx::connection> con) {
    this->con = con;
}

bool WorkerRepository::authenticate(const uint64_t workerId, const std::string code) const {
    if (code.size() != 24) {
        return false;
    }
    pqxx::nontransaction non(*con);
    try {
        non.exec_params1("SELECT code FROM workers WHERE worker_id = $1;", workerId);
    } catch (pqxx::unexpected_rows& e) {
        return false;
    }
}
