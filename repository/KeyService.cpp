#include "KeyService.h"

KeyService::KeyService(
    std::shared_ptr<KeyRepository> key_repository
) :
    key_repository(std::move(key_repository))
{
}

std::expected<Key, KeyError> KeyService::get_worker_by_key(const uint64_t key_id) const
{
    return key_repository -> get_worker_by_key_id(key_id);
}

std::optional<Key> KeyService::insert(const Key& key) const
{
    return key_repository -> insert(key);
}

bool KeyService::disable_by_worker_id(const uint32_t worker_id) const
{
    return key_repository->disable_by_worker_id(worker_id);
}
