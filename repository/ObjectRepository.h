#pragma once
#include <memory>
#include "DbPool.h"

class ObjectRepository {
    std::shared_ptr<DbPool> pool;
    static constexpr char exists_by_id_query[] = "SELECT 1 FROM objects WHERE id = $1;";
public:
    explicit ObjectRepository(std::shared_ptr<DbPool> pool);
    bool exists_by_id(uint64_t id) const;
};
