#pragma once
#include <memory>
#include "pqxx/connection.hxx"

struct Permission {
    uint64_t worker_id, object_id;
};

class PermissionRepository {
    std::shared_ptr<pqxx::connection> con;
    std::hash<std::string> hasher;
    static constexpr char check_permission_query[] = "check_permission_query";
public:
    explicit PermissionRepository(std::shared_ptr<pqxx::connection> con);
    bool has_permission(
        const Permission& permission,
        const std::string& code
    ) const;
};
