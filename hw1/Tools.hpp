#include "Basic.hpp"
#include "Message_Box.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<string> GetArg(char *input, int len) {
    stringstream ss;
    ss << input;
    string str;
    vector<string> ret;
    while (ss >> str) {
        ret.push_back(str);
    }
    return ret;
}

void CmdHint() {
    cout << "Available command: "
        << "register, login, logout, whoami, list-user, exit, send, list-msg, receive" << endl;
}

int Handle(char *input, int len) {
    int ret = 0;
    vector<string> args = GetArg(input, len);

    if (args.empty()) {
        CmdHint();
    } else {
        if (args[0] == "register") {
            cout << "Command: register" << "\n";
            Register(args);
        } else if (args[0] == "login") {

        } else if (args[0] == "logout") {

        } else if (args[0] == "whoami") {

        } else if (args[0] == "list-user") {

        } else if (args[0] == "exit") {

        } else if (args[0] == "send") {

        } else if (args[0] == "list-msg") {

        } else if (args[0] == "receive") {

        } else {
            CmdHint();
        }
    }
    return ret;
}
