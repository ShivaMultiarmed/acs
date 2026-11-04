#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <pqxx/row.hxx>

#include "repository/InteractionRepository.h"

using json = nlohmann::json;

Interaction to_interaction(const pqxx::row &row);

std::string to_string(InteractionFailure failure);

InteractionFailure to_interaction_failure(std::string failure);

uint64_t to_millis(const std::chrono::time_point<std::chrono::system_clock> &point);

std::string to_date_time_string(uint64_t millis);

std::string to_string(const std::chrono::time_point<std::chrono::system_clock> &point);

Interaction to_interaction(json data);

InteractionType to_interaction_type(std::string type);

std::string to_string(InteractionType type);

json to_json (const Interaction &interaction);

std::uint64_t to_millis(const std::string& date_time);