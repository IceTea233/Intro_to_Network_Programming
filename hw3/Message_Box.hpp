#ifndef _MESSAGE_BOX_
#define _MESSAGE_BOX_

#include "Data.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <sstream>

using namespace std;

string Send(const vector<string> &args, Data &data, int &uid);
string ListMsg(const vector<string> &args, Data &data, int &uid);
string Receive(const vector<string> &args, Data &data, int &uid);

string Send(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: send\n";
    if (args.size() != 3)
        return "Usage: send <username> <message>\n";

    if (uid == -1)
        return "Please login first.\n";
    if (!data.users.exist(args[1]))
        return "User not existed.\n";

    User *receiver = data.users.access(args[1]);
    receiver->msgbox[data.users.get(uid).name].push_back(args[2]);
    return "";
}

string ListMsg(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: list-msg\n";
    if (uid == -1)
        return "Please login first.\n";

    string msgs;
    stringstream ss;

    User user = data.users.get(uid);
    if (user.msgbox.empty())
        return "Your message box is empty.\n";

    for (const auto &it : user.msgbox) {
        ss << it.second.size() << " message from " << it.first << ".\n";
    }
    msgs = ss.str();

    return msgs;
}

string Receive(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: receive\n";
    if (args.size() != 2)
        return "Usage: receive <username>\n";
    if (uid == -1)
        return "Please login first.\n";
    if (!data.users.exist(args[1]))
        return "User not existed.\n";

    User *user = &(data.users.infos[uid]);
    if (user->msgbox.find(args[1]) == user->msgbox.end())
        return "";

    string msg = user->msgbox[args[1]].front();
    user->msgbox[args[1]].pop_front();
    if (user->msgbox[args[1]].empty())
        user->msgbox.erase(args[1]);

    return msg + "\n";
}

#endif
