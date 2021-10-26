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
void Login(const vector<string> &args);
void logout(const vector<string> &args);
void Whoami(const vector<string> &args);
void ListUser(const vector<string> &args);
void Exit(const vector<string> &args);

string Register(const vector<string> &args, map<string, User> &data) {
    cout << "Receive cmd: Register\n";
    if (args.size() != 3)
        return "register <username> <password>\n";
    if (data.find(args[1]) != data.end())
        return "Username is already used.\n";

    User user(args[1], args[2]);
    data[user.username] = user;
    cout << "User " << user.username << " registered.\n";
    return "Register successfully.\n";
}
