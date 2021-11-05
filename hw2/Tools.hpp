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
#include <sstream>
#include <cstring>
#include <cctype>

using namespace std;

vector<string> GetArg(char *input) {
    string buff;
    vector<string> ret;
    bool flag = false;
    for (int i=0; input[i] != '\0'; i++) {
        if (flag) {
            if (input[i] == '\"') {
                ret.push_back(buff);
                buff.clear();
                flag = false;
            } else {
                buff.push_back(input[i]);
            }
        } else {
            if (input[i] == '\"') {
                if (!buff.empty()) {
                    ret.push_back(buff);
                    buff.clear();
                }
                flag = true;
            }
            else if (isgraph(input[i])) {
                buff.push_back(input[i]);
            } else {
                if (!buff.empty()) {
                    ret.push_back(buff);
                    buff.clear();
                }
            }
        }

        if (!isprint(input[i]))
            ret.push_back("\n");
    }

    return ret;
}

string CmdHint() {
    // stringstream ss;
    // cout << "Received illegal command.\n";
    // string ret = "Usage: register, login, logout, whoami, list-user, exit, send, list-msg, receive\n";
    // return ret;
    return "";
}

int Handle(char *input, char *buff, int buff_len) {
    static map<string, User> data;
    static User user;
    int code = 0;
    memset(buff, 0, buff_len);
    vector<string> args_arr = GetArg(input);

    vector<string> args;
    for (auto it : args_arr) {
        if (it == "\n") {
            string res;
            if (args.empty()) {
                res = CmdHint();
                strncat(buff, res.c_str(), buff_len);
            } else {
                string ret;
                if (args[0] == "register") {
                    res = Register(args, data);
                } else if (args[0] == "login") {
                    res = Login(args, data, user);
                } else if (args[0] == "logout") {
                    res = Logout(args, data, user);
                } else if (args[0] == "whoami") {
                    res = Whoami(args, data, user);
                } else if (args[0] == "list-user") {
                    res = ListUser(args, data);
                } else if (args[0] == "exit") {
                    res = Exit(args, data, user);
                    code = 1;
                } else if (args[0] == "send") {
                    res = Send(args, data, user);
                } else if (args[0] == "list-msg") {
                    res = ListMsg(args, data, user);
                } else if (args[0] == "receive") {
                    res = Receive(args, data, user);
                } else {
                    res = CmdHint();
                }
                strncat(buff, res.c_str(), buff_len);
            }

            if (code == 1)
                return code;
            args.clear();
        } else {
            args.push_back(it);
        }
    }

    return 0;
}
