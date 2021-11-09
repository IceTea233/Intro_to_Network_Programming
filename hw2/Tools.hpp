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
    string long_buff;
    vector<string> ret;
    int flag = 0;
    // 0: normal.
    // 1: quoted - Simply copy the raw content in two quotation, exclusive of the quotation symbol.
    // 2: string argument - Any non-graph character is regarded as a space character. Detection depends on --flag.

    for (int i=0; input[i] != '\0'; i++) {
        if (flag == 0) {
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
                    if (buff.size() >= 2 && buff.substr(0, 2) == "--") {
                        long_buff.clear();
                        flag = 2;
                    }
                    buff.clear();
                }
            }
        } else if (flag == 1) {
            if (input[i] == '\"') {
                ret.push_back(buff);
                buff.clear();
                flag = false;
            } else {
                buff.push_back(input[i]);
            }
        } else if (flag == 2) {
            if (isgraph(input[i])) {
                buff.push_back(input[i]);
            } else {
                if (buff.size() >= 2 && buff.substr(0, 2) == "--") {
                    if (!long_buff.empty()) {
                        long_buff.pop_back();
                        ret.push_back(long_buff);
                    }
                    long_buff.clear();
                    ret.push_back(buff);
                } else {
                    long_buff.append(buff + ' ');
                }
                buff.clear();
            }
        }
    }

    if (!buff.empty())
        long_buff.append(buff);
    if (!long_buff.empty()) {
        long_buff.pop_back();
        ret.push_back(long_buff);
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
    cout << "read args: ";
    for (auto it : args_arr) {
        cout << " {" << it << "}";
    }
    cout << "\n";

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
