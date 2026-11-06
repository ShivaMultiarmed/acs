#include "key_utils.h"

#include "../utils.h"

Key to_key(const pqxx::row& row)
{
    return {
        row["id"].as(),
        row["worker_id"].as(),
        to_millis(row["expires_at"].as<std::string>()),
        row["is_active"].as()
    };
}

Key to_key(const json object)
{
    return {
        object["id"].get<uint32_t>(),
        object["worker_id"].get<uint32_t>(),
        object["expires_at"].get<uint32_t>(),
        object["expires_at"].get<bool>()
    };
}

json to_json(const Key& key)
{
    return json::object({
        {"id", key.id},
        {"worker_id", key.worker_id},
        {"expires_at", key.expires_at},
        {"is_active", key.is_active}
    });
}