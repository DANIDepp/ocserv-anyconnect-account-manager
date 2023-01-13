#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <algorithm>
#include <filesystem>
#include <sqlite3.h>

using namespace std;

static int user_check(void *fn, int num, char **data, char **column)
{
    int *flag = (int *)fn;

    if (stoi(data[0]) == 1)
    {
        *flag = true;
    }
    else
    {
        *flag = false;
    }

    return 0;
}

#define backup "backup.db"
#define datadb "database.db"

int main()
{

    sqlite3 *database;
    if (filesystem::exists(datadb))
    {
        sqlite3_open(datadb, &database);
    }
    else
    {
        sqlite3_open(datadb, &database);
        sqlite3_exec(database, "CREATE TABLE 'users' ('name' TEXT,'time' INTEGER);", NULL, NULL, NULL);
    }

    // clear termianl
    system("clear");

    cout << "1 - Add user" << endl;
    cout << "2 - Charge user" << endl;
    cout << ">> ";

    int input_main;
    cin >> input_main;

    if (input_main == 1)
    {
        system("clear");

        cout << "Username > ";
        string username;
        cin >> username;

        cout << "Password > ";
        string password;
        cin >> password;

        cout << "Days > ";
        int day;
        cin >> day;

        int custom_unix = time(nullptr) + (86400 * day);

        bool user_exist;
        string user_query = "SELECT COUNT(*) FROM users WHERE name='" + username + "'";
        sqlite3_exec(database, user_query.c_str(), user_check, &user_exist, NULL);

        if (user_exist)
        {
            cout << "\033[1;31mUser Has Exists\033[0m" << endl;
            return 0;
        }

        string query = "INSERT INTO users VALUES('" + username + "'," + to_string(custom_unix) + ")";
        sqlite3_exec(database, query.c_str(), NULL, NULL, NULL);

        string command = "echo ";
        command.append(password + " | sudo ocpasswd -c /etc/ocserv/ocpasswd " + username);
        system(command.c_str());
        cout << "User added" << endl;
    }
    if (input_main == 2)
    {
        cout << "Username > ";
        string username;
        cin >> username;

        cout << "Days > ";
        int day;
        cin >> day;

        int custom_unix = time(nullptr) + (86400 * day);

        bool user_exist;
        string user_query = "SELECT COUNT(*) FROM users WHERE name='" + username + "'";
        sqlite3_exec(database, user_query.c_str(), user_check, &user_exist, NULL);

        if (user_exist == false)
        {
            cout << "\033[1;31mUser Not Exists\033[0m" << endl;
            return 0;
        }

        string query = "UPDATE users SET time=" + to_string(custom_unix) + " WHERE name='" + username + "'";
        sqlite3_exec(database, query.c_str(), NULL, NULL, NULL);

        cout << "User Charged" << endl;
        string command = "sudo ocpasswd -c /etc/ocserv/ocpasswd --unlock ";
        command.append(username);
        system(command.c_str());
    }

    sqlite3_close(database);

}
