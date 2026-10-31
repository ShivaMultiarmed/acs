#pragma once
#include <memory>
#include "pqxx/connection.hxx"

class WorkerRepository {
    std::shared_ptr<pqxx::connection> con;
public:
    explicit WorkerRepository(std::shared_ptr<pqxx::connection> con);
    bool exists_by_id(const uint64_t workerId) const;
};