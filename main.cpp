#include <iostream>
#include <pqxx/pqxx>

int main() {
    std::string dbHost = getenv("DB_HOST");
    std::string dbName = getenv("DB_NAME");
    std::string userName = getenv("USER_NAME");
    std::string password = getenv("PASSWORD");
    pqxx::connection con("host=" + dbHost + " dbname=" + dbName + " user=" + userName + " pass=" + password);
    pqxx::work tr(con);
    pqxx::result result= tr.exec_prepared("SELECT date_time, interaction FROM interactions WHERE user_id = $1 ORDER BY date_time;", 1);
    tr.commit();
    pqxx::row row = result[0];
    auto interaction = row[1].as<std::string>();
    return 0;
}
