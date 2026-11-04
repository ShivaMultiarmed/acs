#include "InteractionService.h"

#include <iomanip>
#include <iostream>

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

Interaction &InteractionService::interact(Interaction& interaction, const std::string code) const {
    auto now = std::chrono::system_clock::now();
    std::optional<Interaction> last_interaction;
    interaction.date_time = to_millis(now);
    Permission permission;
    if (!worker_repository->exists_by_id(interaction.worker_id)) {
        interaction.failure = InteractionFailure::NOT_AUTHENTICATED;
        goto proceed;
    }
    if (!object_repository->exists_by_id(interaction.object_id)) {
        interaction.failure = InteractionFailure::NOT_FOUND;
        goto proceed;
    }
    permission = {interaction.worker_id, interaction.object_id, code};
    if (!permission_repo->has_permission(permission)) {
        interaction.failure = InteractionFailure::FORBIDDEN;
        goto proceed;
    }
    last_interaction = interaction_repo->getLastByUserIdAndObjectId(interaction.worker_id, interaction.object_id);
    if (last_interaction && interaction.type == last_interaction.value().type
        || !last_interaction && interaction.type == InteractionType::EXIT) {
        interaction.failure = std::optional(InteractionFailure::FORBIDDEN);
        goto proceed;
    }
proceed:
    return interaction_repo->interact(interaction);
}
