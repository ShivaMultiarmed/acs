#pragma once
#include <memory>

#include "repository/KeyRepository.h"

class KeyService
{
    std::shared_ptr<KeyRepository> key_repository;
public:
    explicit KeyService(std::shared_ptr<KeyRepository> key_repository);
    std::optional<uint64_t> get_worker_by_key(uint64_t key_id) const;
};
