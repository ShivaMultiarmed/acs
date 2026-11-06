#pragma once
#include <expected>
#include <pqxx/connection.hxx>
#include <pqxx/transaction.hxx>

#include "DbPool.h"

struct Key {
    uint32_t id, worker_id;
    uint64_t expires_at;
    bool is_active;
};

enum class KeyError
{
    NOT_FOUND, EXPIRED, NOT_ACTIVE
};

class KeyRepository {
    std::shared_ptr<DbPool> pool;
    static constexpr char get_key[] = "get_key";
    bool disable_by_worker_id(pqxx::work &tx, uint32_t worker_id) const;
public:
    explicit KeyRepository(std::shared_ptr<DbPool> pool);
    std::expected<Key, KeyError> get_worker_by_key_id(uint64_t key_id) const;
    std::optional<Key> insert(const Key& key) const;
    bool disable_by_worker_id(uint32_t worker_id) const;
};