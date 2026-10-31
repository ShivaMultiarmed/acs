#pragma once
#include "../repository/InteractionRepository.h"
#include "../repository/ObjectRepository.h"
#include "../repository/PermissionRepository.h"
#include "../repository/WorkerRepository.h"

class InteractionService {
    std::shared_ptr<PermissionRepository> permission_repo;
    std::shared_ptr<InteractionRepository> interaction_repo;
    std::shared_ptr<WorkerRepository> worker_repository;
    std::shared_ptr<ObjectRepository> object_repository;
public:
    explicit InteractionService(
        std::shared_ptr<PermissionRepository> permission_repository,
        std::shared_ptr<InteractionRepository> interaction_repository,
        std::shared_ptr<WorkerRepository> worker_repository,
        std::shared_ptr<ObjectRepository> object_repository
    );

    Interaction &interact(
        uint64_t worker_id,
        uint64_t object_id,
        std::string code
    ) const;
};
