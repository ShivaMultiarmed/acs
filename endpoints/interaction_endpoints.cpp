#include "interaction_endpoints.h"
#include <nlohmann/json.hpp>

#include "../repository/KeyService.h"
#include "../utils.h"
#include "../service/InteractionService.h"

using json = nlohmann::json;

// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Interaction, id, worker_id, object_id, date_time, type, failure)

void set_up_interaction_endpoints(
    std::shared_ptr<httplib::Server> server,
    std::shared_ptr<InteractionService> interactionService,
    std::shared_ptr<KeyService> key_service
)
{
    server->Post("/interactions",
                 [interactionService, key_service](const httplib::Request& request, httplib::Response& response)
                 {
                     json body = json::parse(request.body, nullptr, false, true);
                     if (body.is_discarded() || !body.is_object())
                     {
                         response.status = 404;
                         return;
                     }
                     if (!request.has_header("Authorization"))
                     {
                         response.status = 401;
                         return;
                     }
                     try
                     {
                         const std::string auth_prefix = "Bearer ";
                         std::string code = request.get_header_value("Authorization").substr(auth_prefix.size());
                         InteractionRequest interaction_request = {
                             body["key_id"].get<uint64_t>(),
                             body["object_id"].get<uint64_t>(),
                             to_interaction_type(body["type"].get<std::string>())
                         };
                         std::expected<Key, KeyError> worker = key_service->get_worker_by_key(
                             interaction_request.key_id);
                         if (!worker.has_value())
                         {
                             switch (worker.error())
                             {
                             case KeyError::NOT_FOUND:
                                 response.status = 401;
                                 break;
                             case KeyError::EXPIRED:
                             case KeyError::NOT_ACTIVE:
                                 response.status = 403;
                             }
                             return;
                         }
                         Interaction interaction = {
                             std::nullopt,
                             std::nullopt,
                             worker.value().id,
                             interaction_request.object_id,
                             interaction_request.type
                         };
                         json response_body = to_json(interactionService->interact(interaction, std::move(code)));
                         response.set_content(response_body.dump(), "application/json");
                     }
                     catch (std::exception& e)
                     {
                         response.status = 500;
                         response.set_content("Error occurred: " + std::string(e.what()), "text/plain");
                     }
                 });
}
