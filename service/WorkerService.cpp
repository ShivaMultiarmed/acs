#include "WorkerService.h"

WorkerService::WorkerService(
    std::shared_ptr<WorkerRepository> worker_repository
    ): worker_repository(std::move(worker_repository))
{

}

std::optional<Worker> WorkerService::get_by_id(uint32_t id) const
{
    return worker_repository->get_by_id(id);
}
