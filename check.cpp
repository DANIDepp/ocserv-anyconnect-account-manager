#include <iostream>
#include <string>
#include <ctime>
#include <sqlite3.h>

using namespace std;

static int user_expire(void *fn, int num, char **data, char **column)
{

    string command = "sudo ocpasswd -c /etc/ocserv/ocpasswd --lock ";
    command.append(*data);
    system(command.c_str());
    cout << *data << " Locked" << endl;
    return 0;
}

int main()
{
    // clear termianl
    system("clear");

    sqlite3 *database;

    cout << "Starting Bot..." << endl;

    int now_unix = time(nullptr);

    sqlite3_open("database.db", &database);
    string query = "SELECT name FROM users WHERE time<=" + to_string(now_unix);
    sqlite3_exec(database, query.c_str(), user_expire, NULL, NULL);

    sqlite3_close(database);
}
