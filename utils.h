#include <string>

enum class InteractionFailure;

template <typename T>
const char* nullify(T* value);

std::string toString(const InteractionFailure failure);