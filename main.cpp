#include <pqxx/pqxx>

#include "endpoints/interaction_endpoints.h"
#include "repository/DbPool.h"
#include "repository/InteractionRepository.h"
#include "repository/KeyRepository.h"
#include "repository/PermissionRepository.h"
#include "repository/WorkerRepository.h"
#include "service/InteractionService.h"

int main() {
    std::string db_host = getenv("DB_HOST");
    uint32_t db_port = std::stoi(getenv("DB_PORT"));
    std::string db_name = getenv("DB_NAME");
    std::string username = getenv("DB_USERNAME");
    std::string password = getenv("DB_PASSWORD");

    auto con_pool = std::make_shared<DbPool>(username, db_name, password, db_host, db_port);

    std::shared_ptr<httplib::Server> server(new httplib::Server());

    std::shared_ptr<KeyRepository> key_repository(new KeyRepository(con_pool));
    std::shared_ptr<WorkerRepository> worker_repository(new WorkerRepository(con_pool));
    auto interaction_repository = std::make_shared<InteractionRepository>(con_pool);
    auto permission_repository = std::make_shared<PermissionRepository>(con_pool);
    auto object_repository = std::make_shared<ObjectRepository>(con_pool);

    auto interaction_service = std::make_shared<InteractionService>(
        permission_repository,
        interaction_repository,
        worker_repository,
        object_repository,
        key_repository
    );

    std::shared_ptr<KeyService> key_service(new KeyService(key_repository));

    set_up_interaction_endpoints(server, interaction_service, key_service);

    server -> listen("0.0.0.0", 9999);

    return 0;
}
