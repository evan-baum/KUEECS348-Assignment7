#include <stdio.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

int main() {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con; // to establish connection
    sql::Statement *stmt; // to execute SQL statement
    sql::ResultSet *res;  // a place to store the query results

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("mysql.eecs.ku.edu", "348f25_e724b371", "Jah7EWek");

    stmt = con->createStatement();

    stmt->execute("USE 348f25_e724b371"); // entering database

    cout << "Prompt 1:" << endl;
    res = stmt->executeQuery("SELECT * FROM Student WHERE StdMajor LIKE 'IS'");
    while (res->next())
    {
        // You can use either numeric offsets...
        // getInt(1) returns the first column
        std::cout << "id = " << res->getInt(1);
        // ... or column names for accessing results (recommended).
        std::cout << ", label = '" << res->getString("label") << "'" << std::endl;
    }

    delete stmt;
    delete con;
    delete con;

    return 0;
}