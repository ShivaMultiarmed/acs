#pragma once
#include <memory>
#include "DbPool.h"
#include "pqxx/connection.hxx"

class WorkerRepository {
    std::shared_ptr<DbPool> pool;
    static constexpr char exists_by_id_query[] = "exists_by_id_query";
public:
    explicit WorkerRepository(std::shared_ptr<DbPool> pool);
    bool exists_by_id(uint64_t workerId) const;
};