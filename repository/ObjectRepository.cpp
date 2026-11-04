//
// Created by Mikhail_Shell on 2026-10-31.
//

#include "ObjectRepository.h"

#include "pqxx/nontransaction.hxx"

ObjectRepository::ObjectRepository(
    std::shared_ptr<pqxx::connection> con
): con(std::move(con))
{
    con -> prepare(exists_by_id_query, "SELECT 1 FROM objects WHERE id = $1;");
}

bool ObjectRepository::exists_by_id(const uint64_t id) const {
    pqxx::nontransaction non(*con);
    std::optional<pqxx::result> row = non.exec(pqxx::prepped {exists_by_id_query}, pqxx::params{id}).opt_row();
    return row.has_value();
}
