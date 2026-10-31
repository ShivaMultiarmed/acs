#include <memory>

#include "pqxx/connection.hxx"

class WorkerRepository {
    std::shared_ptr<pqxx::connection> con;
public:
    explicit WorkerRepository(std::shared_ptr<pqxx::connection> con);
};