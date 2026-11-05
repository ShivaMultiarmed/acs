#include "KeyService.h"

KeyService::KeyService(
    std::shared_ptr<KeyRepository> key_repository
) :
    key_repository(std::move(key_repository))
{
}

std::optional<uint64_t> KeyService::get_worker_by_key(const uint64_t key_id) const
{
    return key_repository -> get_worker_id_by_key_id(key_id);
}
