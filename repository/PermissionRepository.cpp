#include "PermissionRepository.h"
#include "pqxx/nontransaction.hxx"
#include "pqxx/transaction.hxx"

PermissionRepository::PermissionRepository(std::shared_ptr<pqxx::connection> con)
    :con(std::move(con)){
    con -> prepare(check_permission_query, "SELECT code FROM permissions WHERE worker_id = $1 AND object_id = $2;");
}

bool PermissionRepository::has_permission(const Permission &permission) const {
    if (!permission.code || permission.code.value().size() != 24) return false;
    pqxx::nontransaction transaction(*con);
    std::optional<pqxx::row> row = transaction.exec(
        pqxx::prepped{check_permission_query},
        pqxx::params{permission.worker_id, permission.object_id}
    ).opt_row();
    if (!row) return false;
    auto [codeHash] = row.value().as<std::string>(); // TODO: store real hash
    return codeHash == permission.code.value();
}