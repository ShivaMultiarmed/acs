#include <pqxx/pqxx>

#include "endpoints/interaction_endpoints.h"
#include "repository/InteractionRepository.h"
#include "repository/PermissionRepository.h"
#include "repository/WorkerRepository.h"
#include "service/InteractionService.h"

int main() {
    std::string db_host = getenv("DB_HOST");
    std::string db_name = getenv("DB_NAME");
    std::string username = getenv("USER_NAME");
    std::string password = getenv("PASSWORD");
    auto con = std::make_shared<pqxx::connection>("host=" + db_host + " dbname=" + db_name + " user=" + username + " pass=" + password);

    httplib::Server server;

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

    setUpInteractionEndpoints(server, interaction_service);

    server.listen("0.0.0.0", 9999);

    return 0;
}
