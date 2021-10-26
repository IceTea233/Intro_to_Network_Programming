#ifndef _User_
#define _User_
#include "User.hpp"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

string Register(const vector<string> &args, map<string, User> &data);
string Login(const vector<string> &args, map<string, User> &data, User &user);
string Logout(const vector<string> &args, map<string, User> &data, User &user);
string Whoami(const vector<string> &args, map<string, User> &data);
string ListUser(const vector<string> &args, map<string, User> &data);
string Exit(const vector<string> &args, map<string, User> &data);

string Register(const vector<string> &args, map<string, User> &data) {
    cout << "Receive request: Register\n";
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
    cout << "Receive request: Login\n";
    if (args.size() < 3)
        return "Usage: Login <username> <password>\n";
    if (args[1] == user.username)
        return "Please logout first.\n";
    if (data.find(args[1]) == data.end() || data[args[1]].pass != args[2])
        return "Login failed.\n";

    data[args[1]].login = true;
    user = data[args[1]];

    return "Welcome, " + user.username + ".\n";
}

string Logout(const vector<string> &args, map<string, User> &data, User &user) {
    cout << "Receive request: Login\n";

    if (user.username.empty())
        return "Please login first.\n";

    data[user.username].login = false;
    string tmp = user.username;
    user = User();
    return "Bye, " + tmp + ".\n";
}
