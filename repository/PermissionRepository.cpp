#include "PermissionRepository.h"
#include "pqxx/nontransaction.hxx"
#include "pqxx/transaction.hxx"

PermissionRepository::PermissionRepository(std::shared_ptr<pqxx::connection> con) {
    this -> con = con;
}

bool PermissionRepository::has_permission(
    const uint64_t userId,
    const uint64_t objectId,
    const std::string code
    ) const {
    if (code.size() != 24) return false;
    pqxx::nontransaction transaction(*con);
    try {
        pqxx::row row = transaction.exec_prepared1(
            "SELECT code FROM permissions WHERE user_id = $1 AND object_id = $2;", userId, objectId);
        pqxx::field field = row["code"];
        auto codeHash = field.as<std::string>(); // TODO: store real hash
        if (code != codeHash) throw std::runtime_error("");
        return true;
    } catch (const std::exception &e) {
        return false;
    }
}