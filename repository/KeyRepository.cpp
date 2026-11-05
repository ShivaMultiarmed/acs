#include "KeyRepository.h"

#include <pqxx/nontransaction.hxx>

KeyRepository::KeyRepository(std::shared_ptr<DbPool> pool): pool(std::move(pool)) {
    for (uint8_t i = 0; i < pool->con_count; ++i)
    {
        pool -> get_con() -> prepare(worker_by_key, "SELECT worker_id FROM keys WHERE id = $1;");
    }
}

std::optional<uint64_t> KeyRepository::get_worker_id_by_key_id(const uint64_t key_id) const {
    pqxx::nontransaction non(pool->get_con());
    return non.exec(pqxx::prepped{worker_by_key}, pqxx::params{key_id})
    .opt_row()
    .transform([](const pqxx::row &row){
        return row["worker_id"].as<uint64_t>();
    });
}
