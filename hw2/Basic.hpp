#ifndef _DATA_HPP_
#define _DATA_HPP_
#include "Data.hpp"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

// Account Administration
string Register(const vector<string> &args, Data &data);
string Login(const vector<string> &args, Data &data, int &uid);
string Logout(const vector<string> &args, Data &data, int &uid);
string Whoami(const vector<string> &args, Data &data, int &uid);
string ListUser(const vector<string> &args, Data &data);
string Exit(const vector<string> &args, Data &data, int &uid);

// Board Operations
string CreateBoard(const vector<string> &args, Data &data, int &uid);
string CreatePost(const vector<string> &args, Data &data, int &uid);
string ListBoard(const vector<string> &args, Data &data);
string ListPost(const vector<string> &args, Data &data);
string Read(const vector<string> &args, Data &data);
string DeletePost(const vector<string> &args, Data &data, int &uid);
string UpdatePost(const vector<string> &args, Data &data, int &uid);
string Comment(const vector<string> &args, Data &data, int &uid);

string Register(const vector<string> &args, Data &data) {
    cout << "Receive request: register\n";
    if (args.size() != 3)
        return "Usage: register <username> <password>\n";
    if (data.users.exist(args[1]))
        return "Username is already used.\n";

    User user(args[1], args[2]);
    data.add_user(user);
    cout << "User " << user.name << " registered.\n";
    return "Register successfully.\n";
}

string Login(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: login\n";
    if (args.size() != 3)
        return "Usage: login <username> <password>\n";
    if (uid != -1)
        return "Please logout first.\n";
    if (!data.users.exist(args[1]) || data.users.get(args[1]).pass != args[2]) {
        return "Login failed.\n";
    }
    // if (data.find(args[1]) == data.end() || data[args[1]].pass != args[2])
    //     return "Login failed.\n";
    User user = data.users.get(args[1]);
    uid = user.id;

    return "Welcome, " + user.name + ".\n";
}

string Logout(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: logout\n";

    if (uid == -1)
        return "Please login first.\n";

    string tmp = data.users.get(uid).name;
    uid = -1;
    return "Bye, " + tmp + ".\n";
}

string Whoami(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: whoami\n";

    if (uid == -1)
        return "Please login first.\n";

    return data.users.get(uid).name + "\n";
}

string ListUser(const vector<string> &args, Data &data) {
    cout << "Receive request: list-user\n";

    string users;
    stringstream ss;
    for (const auto &it : data.users.dic) {
        ss << it.first << "\n";
    }
    users = ss.str();

    return users;
}

string Exit(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: exit\n";

    if (uid != -1) {
        string tmp = data.users.get(uid).name;
        uid = -1;
        return "Bye, " + tmp + ".\n";
    }

    return "";
}
