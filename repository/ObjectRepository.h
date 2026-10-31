#include <memory>

#include "pqxx/connection.hxx"

class ObjectRepository {
    std::shared_ptr<pqxx::connection> con;
public:
    explicit ObjectRepository(std::shared_ptr<pqxx::connection> con);
    bool exists_by_id(const uint64_t id) const;
};
