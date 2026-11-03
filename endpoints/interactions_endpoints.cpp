#include "interaction_endpoints.h"
#include <nlohmann/json.hpp>

#include "../utils.h"
#include "../service/InteractionService.h"

using json = nlohmann::json;

// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Interaction, id, worker_id, object_id, date_time, type, failure)

void set_up_interaction_endpoints(
    std::shared_ptr<httplib::Server> server,
    std::shared_ptr<InteractionService> interactionService
) {
    server->Post("/interactions",[interactionService](const httplib::Request &request, httplib::Response &response) {
        json body = json::parse(request.body, nullptr, false, true);
        if (body.is_discarded() || !body.is_object()) {
            response.status = 404;
            return;
        }
        if (!request.has_header("Authorization")) {
            response.status = 401;
            return;
        }
        try {
            std::string code = request.get_header_value("Authorization");
            Interaction interaction = to_interaction(std::move(body));
            json response_body = to_json(interactionService -> interact(interaction, std::move(code)));
            response.set_content(response_body.dump(), "application/json");
        } catch (std::exception &e) {
            response.status = 500;
            response.set_content("Error occurred: " + std::string(e.what()), "text/plain");
        }
    });
}