#include <atomic>
#include <string>
#include <vector>
#include <pqxx/connection.hxx>

class DbPool
{
    std::atomic<uint8_t> cur_con;
    std::vector<pqxx::connection> cons;
public:
    DbPool(
        std::string username,
        std::string db,
        std::string password,
        std::string host = "localhost",
        uint32_t port = 5432,
        uint8_t con_count = 4
        );
    const uint8_t con_count;
    const pqxx::connection &get_con();
};
