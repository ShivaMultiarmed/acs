#include "PermissionRepository.h"
#include "pqxx/nontransaction.hxx"
#include "pqxx/transaction.hxx"

PermissionRepository::PermissionRepository(std::shared_ptr<pqxx::connection> con) {
    this -> con = con;
}

bool PermissionRepository::hasPermission(const uint64_t userId, const uint64_t objectId) const {
    pqxx::nontransaction transaction(*con);
    try {
        transaction.exec_prepared("SELECT 1 FROM permissions WHERE user_id = $1 AND object_id = $2;", userId, objectId);
        return true;
    } catch (const pqxx::unexpected_rows &e) {
        return false;
    }
}