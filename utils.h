#include <chrono>
#include <iomanip>
#include <string>
#include "pqxx/row.hxx"


struct Interaction;
enum class InteractionFailure;

template <typename T>
const char* nullify(T* value);

std::string toString(const InteractionFailure failure);

Interaction& mapToInteraction(const pqxx::row &row);

uint64_t toMillis(const std::chrono::time_point<std::chrono::system_clock> &point);

std::string &toDateTimeString(const uint64_t millis);

std::string& toString(const std::chrono::time_point<std::chrono::system_clock> &point);