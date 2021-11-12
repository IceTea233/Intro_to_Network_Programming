#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_

#include "Basic.hpp"
#include "Data.hpp"
#include "Message_Box.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <cstring>
#include <cctype>
#define MAXFD 16

using namespace std;

void push_in(vector<string> &ret, string &buff, string &long_buff) {
    if (!buff.empty()) {
        long_buff.append(buff);
        buff.clear();
    }
    if (!long_buff.empty()) {
        long_buff.pop_back();
        ret.push_back(long_buff);
        long_buff.clear();
    }
}

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

        if (!isprint(input[i])) {
            push_in(ret, buff, long_buff);
            ret.push_back("\n");
            flag = 0;
        }
    }

    push_in(ret, buff, long_buff);

    return ret;
}

string CmdHint() {
    // stringstream ss;
    // cout << "Received illegal command.\n";
    // string ret = "Usage: register, login, logout, whoami, list-user, exit, send, list-msg, receive\n";
    // return ret;
    return "";
}

int Handle(int sockfd, char *input, char *buff, int buff_len) {
    static Data data; // All information is saved in data.
    static vector<int> client(MAXFD, -1); // Indicate which account the interested client is currently using.

    memset(buff, 0, buff_len);
    vector<string> args_arr = GetArg(input);
    cout << "read args: ";
    for (auto it : args_arr) {
        cout << " {" << (it == "\n" ? "\'\\n\'" : it) << "}";
        if (it == "\n")
            cout << "\n";
    }
    cout << "\n";

    int code = 0;
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
                    res = Login(args, data, client[sockfd]);
                } else if (args[0] == "logout") {
                    res = Logout(args, data, client[sockfd]);
                } else if (args[0] == "whoami") {
                    res = Whoami(args, data, client[sockfd]);
                } else if (args[0] == "list-user") {
                    res = ListUser(args, data);
                } else if (args[0] == "exit") {
                    res = Exit(args, data, client[sockfd]);
                    code = 1;
                } else if (args[0] == "send") {
                    res = Send(args, data, client[sockfd]);
                } else if (args[0] == "list-msg") {
                    res = ListMsg(args, data, client[sockfd]);
                } else if (args[0] == "receive") {
                    res = Receive(args, data, client[sockfd]);
                } else if (args[0] == "create-board") {
                    res = CreateBoard(args, data, client[sockfd]);
                } else if (args[0] == "create-post") {
                    res = CreatePost(args, data, client[sockfd]);
                } else if (args[0] == "list-board") {
                    res = ListBoard(args, data);
                } else if (args[0] == "list-post") {
                    res = ListPost(args, data);
                } else if (args[0] == "read") {
                    res = Read(args, data);
                } else if (args[0] == "delete-post") {
                    res = DeletePost(args, data, client[sockfd]);
                }
                else {
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

#endif
