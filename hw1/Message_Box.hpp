#include <iostream>
#include <vector>
#include <string>
#include <list>

using namespace std;

string Send(const vector<string> &args, map<string, User> &data, User &user);
string ListMsg(const vector<string> &args, map<string, User> &data, User &user);
string Receive(const vector<string> &args, map<string, User> &data, User &user);

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

string ListMsg(const vector<string> &args, map<string, User> &data, User &user) {
    cout << "Receive request: list-msg\n";
    if (user.username.empty())
        return "Please login first.\n";

    string msgs;
    stringstream ss;

    if (user.msgbox.empty())
        return "Your message box is empty.\n";

    for (const auto &it : user.msgbox) {
        ss << it.second.size() << " message from " << it.first << ".\n";
    }
    msgs = ss.str();

    return msgs;
}

string Receive(const vector<string> &args, map<string, User> &data, User &user) {
    cout << "Receive request: receive\n";
    if (user.username.empty())
        return "Please login first.\n";
    if (data.find(args[1]) == data.end())
        return "User not existed.\n";
    if (user.msgbox.find(args[1]) == user.msgbox.end())
        return "No new message from this user.\n";

    string msg = user.msgbox[args[1]].front();
    user.msgbox[args[1]].pop_front();
    data[user.username] = user;
    return msg + "\n";
}
