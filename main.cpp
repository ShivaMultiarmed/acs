#include <pqxx/pqxx>

#include "endpoints/interaction_endpoints.h"
#include "repository/InteractionRepository.h"
#include "repository/PermissionRepository.h"
#include "repository/WorkerRepository.h"
#include "service/InteractionService.h"

int main() {
    std::string db_host = getenv("DB_HOST");
    std::string db_port = getenv("DB_PORT");
    std::string db_name = getenv("DB_NAME");
    std::string username = getenv("DB_USERNAME");
    std::string password = getenv("DB_PASSWORD");
    std::string con_string = std::format("host={} port={} dbname={} user={} password={}", db_host, db_port, db_name, username, password);
    auto con = std::make_shared<pqxx::connection>(con_string);

    std::shared_ptr<httplib::Server> server(new httplib::Server());

    std::shared_ptr<WorkerRepository> worker_repository(new WorkerRepository(con));
    auto interaction_repository = std::make_shared<InteractionRepository>(con);
    auto permission_repository = std::make_shared<PermissionRepository>(con);
    auto object_repository = std::make_shared<ObjectRepository>(con);

    auto interaction_service = std::make_shared<InteractionService>(
        permission_repository,
        interaction_repository,
        worker_repository,
        object_repository
    );

    set_up_interaction_endpoints(server, interaction_service);

    server -> listen("0.0.0.0", 9999);

    return 0;
}
