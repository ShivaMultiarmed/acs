#include "middleware.h"

#include "interaction_endpoints.h"
#include "../repository/WorkerRepository.h"
#include "../service/WorkerService.h"

void set_up_middleware(
    std::shared_ptr<httplib::Server> server,
    std::shared_ptr<WorkerService> worker_service
    )
{
    server->set_pre_routing_handler([worker_service](const httplib::Request& request, httplib::Response& response)
    {
        if (request.has_header("Authorization"))
        {
            std::string prefix = "Bearer ";
            std::string token = request.get_header_value("Authorization").substr(prefix.size());
            uint32_t worker_id; // TODO: parse from the token
            std::optional<Worker> worker = worker_service->get_by_id(worker_id);
            if (!worker)
            {
                response.status = 401;
                return httplib::Server::HandlerResponse::Handled;
            }
            *auth_info = {worker->id, worker->role};
        }
        return httplib::Server::HandlerResponse::Unhandled;
    });
}
