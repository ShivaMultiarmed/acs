#include <memory>

#include "pqxx/connection.hxx"

class ObjectRepository {
    std::shared_ptr<pqxx::connection> con;
    static constexpr char exists_by_id_query[] = "SELECT 1 FROM objects WHERE id = $1;";
public:
    explicit ObjectRepository(std::shared_ptr<pqxx::connection> con);
    bool exists_by_id(uint64_t id) const;
};
