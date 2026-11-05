#pragma once
#include <memory>
#include "DbPool.h"
#include "../endpoints/interaction_endpoints.h"

struct Worker
{
    uint32_t id;
    Role role;
    std::string first_name, last_name;
};

class WorkerRepository {
    std::shared_ptr<DbPool> pool;
    static constexpr char exists_by_id_query[] = "exists_by_id_query";
public:
    explicit WorkerRepository(std::shared_ptr<DbPool> pool);
    bool exists_by_id(uint64_t workerId) const;
    std::optional<Worker> get_by_id(uint32_t worker_id) const;
};