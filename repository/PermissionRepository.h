#include <memory>

#include "pqxx/connection.hxx"

class PermissionRepository {
    std::shared_ptr<pqxx::connection> con;
    std::hash<std::string> hasher;
public:
    explicit PermissionRepository(std::shared_ptr<pqxx::connection> con);
    bool hasPermission(
        const uint64_t userId,
        const uint64_t objectId,
        const std::string code
        ) const;
};
