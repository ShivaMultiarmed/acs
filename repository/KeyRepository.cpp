#include "KeyRepository.h"

#include <expected>
#include <pqxx/nontransaction.hxx>
#include "../utils.h"
#include "../utils/key_utils.h"

KeyRepository::KeyRepository(std::shared_ptr<DbPool> pool) : pool(std::move(pool))
{
    for (uint8_t i = 0; i < pool->con_count; ++i)
    {
        pool->get_con()->prepare(get_key, "SELECT id, worker_id, expires_at, is_active FROM keys WHERE id = $1;");
    }
}

std::expected<Key, KeyError> KeyRepository::get_worker_by_key_id(const uint64_t key_id) const
{
    pqxx::nontransaction non(pool->get_con());
    std::optional<pqxx::row> row = non.exec(pqxx::prepped{get_key}, pqxx::params{key_id}).opt_row();
    if (!row) return std::unexpected(KeyError::NOT_FOUND);
    uint64_t now = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now()
    ).time_since_epoch().count();
    Key key = to_key(row.value());
    if (key.expires_at < now)
    {
        return std::unexpected(KeyError::EXPIRED);
    }
    if (!key.is_active)
    {
        return std::unexpected(KeyError::EXPIRED);
    }
    return key;
}

std::optional<Key> KeyRepository::insert(const Key& key) const
{
    pqxx::work tx(pool->get_con());
    if (!disable_by_worker_id(tx, key.worker_id))
        return std::nullopt;
    pqxx::row row = non.exec("INSERT INTO keys (id, worker_id, expires_at, is_active)"
                             " VALUES ($1, $2, $3, $4)"
                             " RETURNING id, worker_id, expires_at, is_active",
                             pqxx::params{key.id, key.worker_id, key.expires_at, key.is_active}
    ).one_row();
    tx.commit();
    return to_key(row);
}

bool KeyRepository::disable_by_worker_id(const uint32_t worker_id) const
{
    pqxx::nontransaction non(pool->get_con());
    return disable_by_worker_id(non, worker_id);
}

bool KeyRepository::disable_by_worker_id(pqxx::work& tx, uint32_t worker_id) const
{
    return tx.exec("UPDATE keys SET is_active = FALSE WHERE worker_id = $1;", pqxx::params{worker_id}).size() > 0;
}
