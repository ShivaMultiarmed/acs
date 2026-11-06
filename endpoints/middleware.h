#pragma once
#include <httplib.h>
#include <memory>
void set_up_middleware(std::shared_ptr<httplib::Server> server);