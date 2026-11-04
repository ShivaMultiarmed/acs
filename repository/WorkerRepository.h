#pragma once
#include <memory>
#include "pqxx/connection.hxx"

class WorkerRepository {
    std::shared_ptr<pqxx::connection> con;
    static constexpr char exists_by_id_query[] = "exists_by_id_query";
public:
    explicit WorkerRepository(std::shared_ptr<pqxx::connection> con);
    bool exists_by_id(uint64_t workerId) const;
};