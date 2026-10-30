#include "utils.h"

#include "repository/InteractionRepository.h"

std::string toString(const InteractionFailure failure) {
    switch (failure) {
        case InteractionFailure::NOT_AUTHENTICATED: return "NOT_AUTHENTICATED";
        case InteractionFailure::FORBIDDEN: return "FORBIDDEN";
    }
}
