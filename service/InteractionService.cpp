#include "InteractionService.h"

#include <iomanip>

#include "../utils.h"

InteractionService::InteractionService(
    std::shared_ptr<PermissionRepository> permissionRepo,
    std::shared_ptr<InteractionRepository> interactionRepo
) : permissionRepo(permissionRepo),
    interactionRepo(interactionRepo) {
}

Interaction &InteractionService::interact(
    uint64_t workerId,
    uint64_t objectId,
    std::string code
) const {
    auto now = std::chrono::system_clock::now();
    Interaction interaction{}, lastInteraction;
    interaction.workerId = workerId;
    interaction.objectId = objectId;
    interaction.dateTime = toMillis(now);
    if (!permissionRepo->hasPermission(workerId, objectId, std::move(code))) {
        interaction.failure = InteractionFailure::FORBIDDEN;
        goto proceed;
    }
    lastInteraction = interactionRepo->getLastByUserIdAndObjectId(workerId, objectId);
    if (interaction.type == lastInteraction.type) {
        interaction.failure = InteractionFailure::FORBIDDEN;
        goto proceed;
    }
    proceed:
    return interactionRepo->interact(interaction);
}
