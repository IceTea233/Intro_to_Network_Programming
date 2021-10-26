#ifndef _User_
#define _User_
#include "User.hpp"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

string Register(const vector<string> &args, map<string, User> &data);
string Login(const vector<string> &args, map<string, User> &data, User &user);
string Logout(const vector<string> &args, map<string, User> &data, User &user);
string Whoami(const vector<string> &args, map<string, User> &data, User &user);
string ListUser(const vector<string> &args, map<string, User> &data);
string Exit(const vector<string> &args, map<string, User> &data, User &user);

string Register(const vector<string> &args, map<string, User> &data) {
    cout << "Receive request: register\n";
    if (args.size() < 3)
        return "Usage: register <username> <password>\n";
    if (data.find(args[1]) != data.end())
        return "Username is already used.\n";

    User user(args[1], args[2]);
    data[user.username] = user;
    cout << "User " << user.username << " registered.\n";
    return "Register successfully.\n";
}

string Login(const vector<string> &args, map<string, User> &data, User &user) {
    cout << "Receive request: login\n";
    if (args.size() < 3)
        return "Usage: Login <username> <password>\n";
    if (!user.username.empty())
        return "Please logout first.\n";
    if (data.find(args[1]) == data.end() || data[args[1]].pass != args[2])
        return "Login failed.\n";

    data[args[1]].login = true;
    user = data[args[1]];

    return "Welcome, " + user.username + ".\n";
}

string Logout(const vector<string> &args, map<string, User> &data, User &user) {
    cout << "Receive request: logout\n";

    if (user.username.empty())
        return "Please login first.\n";

    data[user.username].login = false;
    string tmp = user.username;
    user = User();
    return "Bye, " + tmp + ".\n";
}

string Whoami(const vector<string> &args, map<string, User> &data, User &user) {
    cout << "Receive request: whoami\n";

    if (user.username.empty())
        return "Please login first.\n";

    return user.username + "\n";
}

string ListUser(const vector<string> &args, map<string, User> &data) {
    cout << "Receive request: list-user\n";

    string users;
    stringstream ss;
    for (const auto &it : data) {
        ss << it.first << "\n";
    }
    users = ss.str();

    return users;
}

string Exit(const vector<string> &args, map<string, User> &data, User &user) {
    cout << "Receive request: list-user\n";

    if (!user.username.empty()) {
        data[user.username].login = false;
        string tmp = user.username;
        user = User();
        return "Bye, " + tmp + ".\n";
    }

    return "";
}
