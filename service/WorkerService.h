#include <memory>

#include "../repository/WorkerRepository.h"

class WorkerService
{
    std::shared_ptr<WorkerRepository> worker_repository;
public:
    explicit WorkerService(
        std::shared_ptr<WorkerRepository> worker_repository
    );
    std::optional<Worker> get_by_id(uint32_t id) const;
};
