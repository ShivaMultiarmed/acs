#pragma once
#include <pqxx/connection.hxx>

#include "DbPool.h"

struct Key {
    uint64_t id, worker_id, expires_at;
    bool is_active;
};

class KeyRepository {
    std::shared_ptr<DbPool> pool;
    static constexpr char worker_by_key[] = "worker_by_key";
public:
    explicit KeyRepository(std::shared_ptr<DbPool> pool);
    std::optional<uint64_t> get_worker_id_by_key_id(uint64_t key_id) const;
};