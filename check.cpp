#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <algorithm>

using namespace std;
int main()
{
    // clear termianl
    system("clear");

    cout << "Starting Bot..." << endl;
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

    ifstream cispass;
    cispass.open("/etc/ocserv/ocpasswd");

    // usernames and time unix list
    vector<string> cis_account;

    string cis_data;
    while (getline(cispass, cis_data))
    {
        string username = cis_data.substr(0, cis_data.find(":"));
        cis_account.push_back(username);
    }

    cispass.close();

    int now_unix = time(nullptr);
    int month_unix = time(nullptr) + (86400 * 29);

    ofstream database_w("database.txt");

    for (size_t i = 0; i < usernames.size(); i++)
    {
        string uname = usernames[i];
        int utime = stoi(unixs[i]);

        if (find(cis_account.begin(), cis_account.end(), uname) != cis_account.end())
        {
            database_w << uname << ":" << utime << "\n";
            cis_account.erase(remove(cis_account.begin(), cis_account.end(), uname), cis_account.end());
        }

        if (utime <= now_unix)
        {
            string command = "sudo ocpasswd -c /etc/ocserv/ocpasswd --lock ";
            command.append(uname);
            system(command.c_str());
            cout << "Lock" << endl;
        }
    }

    for (size_t i = 0; i < cis_account.size(); i++)
    {
        string cis_name = cis_account[i];
        database_w << cis_name << ":" << month_unix << "\n";
    }

    database_w.close();
}
