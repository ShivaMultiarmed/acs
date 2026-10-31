#include "WorkerRepository.h"
#include "pqxx/nontransaction.hxx"
#include "pqxx/transaction.hxx"

WorkerRepository::WorkerRepository(
    std::shared_ptr<pqxx::connection> con
) : con(con) {
}
