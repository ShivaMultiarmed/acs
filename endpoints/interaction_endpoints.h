#pragma once
#include "httplib.h"
#include "../KeyService.h"
#include "../repository/InteractionRepository.h"

class InteractionService;

struct InteractionRequest
{
    uint64_t key_id, object_id;
    InteractionType type;
};

void set_up_interaction_endpoints(
    std::shared_ptr<httplib::Server> server,
    std::shared_ptr<InteractionService> interactionService,
    std::shared_ptr<KeyService> key_service
);
