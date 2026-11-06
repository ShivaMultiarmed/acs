#pragma once
#include <nlohmann/json_fwd.hpp>
#include <nlohmann/detail/meta/std_fs.hpp>

#include "../repository/KeyRepository.h"

using json = nlohmann::json;

Key to_key(const pqxx::row &row);

Key to_key(const json object);

json to_json(const Key& key);