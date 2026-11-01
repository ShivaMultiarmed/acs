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
    auto con = std::make_shared<pqxx::connection>("host=" + db_host + " port=" + db_port + " dbname=" + db_name + " user=" + username + " password=" + password);

    auto server = std::make_shared<httplib::Server>();

    auto worker_repository = std::make_shared<WorkerRepository>(con);
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
