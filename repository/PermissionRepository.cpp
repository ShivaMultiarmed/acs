#include "PermissionRepository.h"
#include "pqxx/nontransaction.hxx"
#include "pqxx/transaction.hxx"

PermissionRepository::PermissionRepository(std::shared_ptr<DbPool> pool)
    : pool(std::move(pool))
{
    for (uint8_t i = 0; i < pool->con_count; ++i)
    {
        pool->get_con().prepare(check_permission_query,
                                "SELECT code FROM permissions WHERE worker_id = $1 AND object_id = $2;");
    }
}

bool PermissionRepository::has_permission(
    const Permission& permission,
    const std::string& code
) const
{
    if (code.size() != 24) return false;
    pqxx::nontransaction transaction(pool -> get_con());
    std::optional<pqxx::row> row = transaction.exec(
        pqxx::prepped{check_permission_query},
        pqxx::params{permission.worker_id, permission.object_id}
    ).opt_row();
    if (!row) return false;
    auto [codeHash] = row.value().as<std::string>(); // TODO: store real hash
    return codeHash == code;
}
