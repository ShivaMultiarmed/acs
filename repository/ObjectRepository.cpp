//
// Created by Mikhail_Shell on 2026-10-31.
//

#include "ObjectRepository.h"

#include "pqxx/nontransaction.hxx"

ObjectRepository::ObjectRepository(
    std::shared_ptr<pqxx::connection> con
): con(std::move(con)){

}

bool ObjectRepository::exists_by_id(const uint64_t id) const {
    pqxx::nontransaction non(*con);
    try {
        non.exec_params1("SELECT 1 FROM objects WHERE id = $1;", id);
        return true;
    } catch (pqxx::unexpected_rows &e) {
        return false;
    }
}
