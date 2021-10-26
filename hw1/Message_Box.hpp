#include <iostream>
#include <vector>
#include <string>
#include <list>

using namespace std;

string Send(const vector<string> &args, map<string, User> &data, User &user);
void ListMsg(const vector<string> *args);

string Send(const vector<string> &args, map<string, User> &data, User &user) {
    cout << "Receive request: send\n";
    if (args.size() < 3)
        return "Usage: send <username> <message>\n";
    if (user.username.empty())
        return "Please login first.\n";
    if (data.find(args[1]) == data.end())
        return "User not existed.\n";

    data[args[1]].msgbox[user.username].push_back(args[2]);
    return "";
}
