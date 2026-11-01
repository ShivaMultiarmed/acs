#include "interaction_endpoints.h"
#include <nlohmann/json.hpp>

#include "../service/InteractionService.h"

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Interaction, id, worker_id, object_id, date_time, type, failure)

void set_up_interaction_endpoints(
    std::shared_ptr<httplib::Server> server,
    std::shared_ptr<InteractionService> interactionService
) {
    server->Post("/interactions",[interactionService](const httplib::Request &request, httplib::Response &response) {
        uint64_t workerId = stoll(request.get_param_value("worked_id"));
        uint64_t objectId = stoll(request.get_param_value("object_id"));
        std::cout << "Got worker id = " << workerId << " and object id = " << objectId << "\n";
        std::string code = request.get_param_value("code");
        std::cout << "Received the code: " << code << "\n";
        Interaction &interaction = interactionService ->interact(workerId, objectId, code);
        json responseBody = interaction; // transforms struct into JSON implicitly
        response.set_content(responseBody.dump(), "application/json");
    });
}

// json body = json::parse(request.body, nullptr, false, true);