#pragma once
#include <memory>

#include "KeyRepository.h"

class KeyService
{
    std::shared_ptr<KeyRepository> key_repository;
public:
    explicit KeyService(std::shared_ptr<KeyRepository> key_repository);
    std::expected<Key, KeyError> get_worker_by_key(uint64_t key_id) const;
    std::optional<Key> insert(const Key &key) const;
    bool disable_by_worker_id(const uint32_t worker_id) const;
};
