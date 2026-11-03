#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "pqxx/row.hxx"
#include "repository/InteractionRepository.h"

using json = nlohmann::json;

struct Interaction;
enum class InteractionFailure;

template <typename T>
const char* nullify(T* value);

Interaction to_interaction(const pqxx::row &row);

std::string to_string(const InteractionFailure failure);

InteractionFailure to_interaction_failure(const std::string failure);

uint64_t to_millis(const std::chrono::time_point<std::chrono::system_clock> &point);

std::string toDateTimeString(const uint64_t millis);

std::string toString(const std::chrono::time_point<std::chrono::system_clock> &point);

Interaction to_interaction(json data);

InteractionType to_interaction_type(const std::string type);

std::string to_string(const InteractionType type);

json to_json (const Interaction &interaction);

std::uint64_t to_millis(const std::string& date_time);