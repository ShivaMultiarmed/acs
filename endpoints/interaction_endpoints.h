#pragma once
#include "httplib.h"
#include "../repository/InteractionRepository.h"

class KeyService;

enum class Role
{
    WORKER, ADMIN
};

struct AuthInfo
{
    uint32_t worker_id;
    Role role;
};

inline thread_local auto auth_info = std::unique_ptr<AuthInfo>(nullptr);

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
