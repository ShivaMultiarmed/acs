#include "InteractionService.h"

#include <iomanip>

#include "../utils.h"

InteractionService::InteractionService(
    std::shared_ptr<PermissionRepository> permission_repository,
    std::shared_ptr<InteractionRepository> interaction_repository,
    std::shared_ptr<WorkerRepository> worker_repository,
    std::shared_ptr<ObjectRepository> object_repository
) : permission_repo(std::move(permission_repository)),
    interaction_repo(std::move(interaction_repository)),
    worker_repository(std::move(worker_repository)),
    object_repository(std::move(object_repository)) {
}

Interaction &InteractionService::interact(
    uint64_t worker_id,
    uint64_t object_id,
    std::string code
) const {
    auto now = std::chrono::system_clock::now();
    Interaction interaction{}, last_interaction;
    interaction.worker_id = worker_id;
    interaction.object_id = object_id;
    interaction.date_time = to_millis(now);
    if (!worker_repository->exists_by_id(worker_id)) {
        interaction.failure = InteractionFailure::NOT_AUTHENTICATED;
        goto proceed;
    }
    if (!object_repository->exists_by_id(object_id)) {
        interaction.failure = InteractionFailure::NOT_FOUND;
        goto proceed;
    }
    if (!permission_repo->has_permission(worker_id, object_id, std::move(code))) {
        interaction.failure = InteractionFailure::FORBIDDEN;
        goto proceed;
    }
    last_interaction = interaction_repo->getLastByUserIdAndObjectId(worker_id, object_id);
    if (interaction.type == last_interaction.type) {
        interaction.failure = InteractionFailure::FORBIDDEN;
        goto proceed;
    }
proceed:
    return interaction_repo->interact(interaction);
}
