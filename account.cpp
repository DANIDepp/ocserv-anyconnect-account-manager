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
int main()
{

    // check backup file exists
    if (filesystem::exists("backup.txt"))
    {
        // remove and replace backup to database
        filesystem::remove("database.txt");
        filesystem::copy("backup.txt","database.txt");
    }else {
        // backup from database
        filesystem::copy("database.txt","backup.txt");
    }
    

    // clear termianl
    system("clear");

    ifstream database;
    database.open("database.txt");

    // usernames and time unix list
    vector<string> usernames;
    vector<string> unixs;

    string data;
    while (getline(database, data))
    {
        string username = data.substr(0, data.find(":"));
        string date_unix = data.erase(0, data.find(":") + 1);
        usernames.push_back(username);
        unixs.push_back(date_unix);
    }

    database.close();

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

        ofstream database_w("database.txt");

        int custom_unix = time(nullptr) + (86400 * day);

        bool exist = false;
        for (size_t i = 0; i < usernames.size(); i++)
        {
            string uname = usernames[i];
            int utime = stoi(unixs[i]);

            if (uname == username)
            {
                exist = true;
            }

            database_w << uname << ":" << utime << "\n";
        }

        if (exist == true)
        {
            cout << "User Has Exist" << endl;
            return 0;
        }

        database_w << username << ":" << custom_unix << "\n";
        string command = "echo ";
        command.append(password + " | sudo ocpasswd -c /etc/ocserv/ocpasswd " + username);
        system(command.c_str());
        database_w.close();
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

        ofstream database_w("database.txt");

        bool exist;

        for (size_t i = 0; i < usernames.size(); i++)
        {
            string uname = usernames[i];
            int utime = stoi(unixs[i]);

            if (uname != username)
            {
                database_w << uname << ":" << utime << "\n";
            }
            else
            {
                database_w << uname << ":" << custom_unix << "\n";
                exist = true;
            }
        }

        if (exist == true)
        {
            cout << "User Charged" << endl;
            string command = "sudo ocpasswd -c /etc/ocserv/ocpasswd --unlock ";
            command.append(username);
            system(command.c_str());
            database_w.close();
        }
        else
        {
            cout << "User not found" << endl;
        }
    }

    filesystem::remove("backup.txt");
}