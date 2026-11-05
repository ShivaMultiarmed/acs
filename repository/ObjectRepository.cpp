//
// Created by Mikhail_Shell on 2026-10-31.
//

#include "ObjectRepository.h"

#include "pqxx/nontransaction.hxx"

ObjectRepository::ObjectRepository(
    std::shared_ptr<DbPool> pool
): pool(std::move(pool))
{
    for (uint8_t i = 0; i < pool->con_count; ++i)
    {
        pool -> get_con().prepare(exists_by_id_query, "SELECT 1 FROM objects WHERE id = $1;");
    }
}

bool ObjectRepository::exists_by_id(const uint64_t id) const {
    pqxx::nontransaction non(pool -> get_con());
    std::optional<pqxx::result> row = non.exec(pqxx::prepped {exists_by_id_query}, pqxx::params{id}).opt_row();
    return row.has_value();
}
