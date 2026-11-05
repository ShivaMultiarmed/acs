#include "KeyRepository.h"

#include <pqxx/nontransaction.hxx>

KeyRepository::KeyRepository(std::shared_ptr<pqxx::connection> con): con(std::move(con)) {
    con -> prepare(worker_by_key, "SELECT worker_id FROM keys WHERE id = $1;");
}

std::optional<uint64_t> KeyRepository::get_worker_id_by_key_id(const uint64_t key_id) const {
    pqxx::nontransaction non(*con);
    return non.exec(pqxx::prepped{worker_by_key}, pqxx::params{key_id})
    .opt_row()
    .transform([](const pqxx::row &row){
        return row["worker_id"].as<uint64_t>();
    });
}
