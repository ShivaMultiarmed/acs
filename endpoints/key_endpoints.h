#pragma once
#include "../service/InteractionService.h"
void set_up_key_endpoints(
    std::shared_ptr<httplib::Server> server,
    std::shared_ptr<KeyService> key_service
    );
