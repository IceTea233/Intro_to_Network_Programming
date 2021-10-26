#include "Basic.hpp"
#include "Message_Box.hpp"

#ifndef _User_
#define _User_
#include "User.hpp"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <string.h>
#include <sstream>

using namespace std;

vector<string> GetArg(char *input) {
    stringstream ss;
    ss << input;
    string str;
    vector<string> ret;
    while (ss >> str) {
        ret.push_back(str);
    }
    return ret;
}

string CmdHint() {
    stringstream ss;
    cout << "Received illegal command.\n";
    string ret = "Available command: register, login, logout, whoami, list-user, exit, send, list-msg, receive\n";
    return ret;
}

int Handle(char *input, char *buff, int buff_len) {
    static map<string, User> data;
    static User user;
    int code = 0;
    memset(buff, 0, sizeof(buff));
    vector<string> args = GetArg(input);

    string ret;
    if (args.empty()) {
        ret = CmdHint();
        strncpy(buff, ret.c_str(), buff_len);
    } else {
        string ret;
        if (args[0] == "register") {
            ret = Register(args, data);
        } else if (args[0] == "login") {
            ret = Login(args, data, user);
        } else if (args[0] == "logout") {
            ret = Logout(args, data, user);
        } else if (args[0] == "whoami") {

        } else if (args[0] == "list-user") {

        } else if (args[0] == "exit") {

        } else if (args[0] == "send") {

        } else if (args[0] == "list-msg") {

        } else if (args[0] == "receive") {

        } else {
            ret = CmdHint();
        }
        strncpy(buff, ret.c_str(), buff_len);
    }
    return code;
}
