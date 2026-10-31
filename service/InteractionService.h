#pragma once
#include "../repository/InteractionRepository.h"
#include "../repository/PermissionRepository.h"

class InteractionService {
    std::shared_ptr<PermissionRepository> permissionRepo;
    std::shared_ptr<InteractionRepository> interactionRepo;
public:
    explicit InteractionService(
        std::shared_ptr<PermissionRepository> permissionRepo,
        std::shared_ptr<InteractionRepository> interactionRepo
    );

    Interaction &interact(
        uint64_t workerId,
        uint64_t objectId,
        std::string code
    ) const;
};
