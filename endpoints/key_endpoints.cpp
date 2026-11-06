#include "key_endpoints.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "../repository/KeyService.h"
#include "../utils/key_utils.h"

using json = nlohmann::json;

void set_up_key_endpoints(std::shared_ptr<httplib::Server> server, std::shared_ptr<KeyService> key_service)
{
    server->Post("/keys", [server, key_service](const httplib::Request &request, httplib::Response &response)
    {
        if (!auth_info || auth_info->role != Role::ADMIN)
        {
            response.status = 401;
            return;
        }
        json object = json::parse(response.body, nullptr, false, true);
        Key key = to_key(object);
        std::optional<Key> inserted_key = key_service->insert(key);
        if (!inserted_key) {
            response.status = 500;
        } else {
            response.set_content(to_json(inserted_key.value()), "application/json");
        }
    });
    server->Delete("/keys", [server, key_service](const httplib::Request &request, httplib::Response &response)
    {
        if (!auth_info || auth_info->role != Role::ADMIN)
        {
            response.status = 401;
            return;
        }
        uint32_t worker_id = std::stoi(request.get_param_value("worker_id"));
        if (!key_service->disable_by_worker_id(worker_id))
        {
            response.status = 500;
        }
    });
}
