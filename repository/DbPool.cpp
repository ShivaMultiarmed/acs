#include "DbPool.h"

#include <pqxx/transaction.hxx>

DbPool::DbPool(
    std::string username,
    std::string db,
    std::string password,
    std::string host,
    uint32_t port,
    uint8_t con_count
) : cons(std::vector<pqxx::connection>(con_count)),
    con_count(con_count)
{
    cur_con = 0;
    std::string con_string = std::format(
        "host={} port={} dbname={} user={} password={}",
        host, port, db, username, password
        );
    for (pqxx::connection& con : cons)
    {
        con = pqxx::connection(con_string);
    }
}

pqxx::connection& DbPool::get_con()
{
    uint8_t cur = cur_con.load();
    cur_con.store((cur + 1) % cons.size());
    return cons[cur];
}
