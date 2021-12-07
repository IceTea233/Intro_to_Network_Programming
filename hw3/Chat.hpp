#ifndef _CHAT_HPP_
#define _CHAT_HPP_

#include "Data.hpp"
#include "Misc.hpp"

using namespace std;

// Chat Room operations
string EnterChatRoom(const vector<string> &args, Data &data, int &uid); // Usage: enter-chat-room <port> <version>

// This is implementation of "Chat" feature for server part.
string Chat(const vector<string> &args, Data &data, int &uid); //Usage: chat <name> <mesg>

string EnterChatRoom(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: enter-chat-room\n";

    if (args.size() < 3) {
        return "Usage: enter-chat-room <port> <version>\n";
    }
    int port, version;
    if (!isnum(args[1]) || (port = stoi(args[1])) < 1 || port > 65535) {
        return "Port " + args[1] + "is not valid.\n";
    }
    if (!isnum(args[2]) || (version = stoi(args[2]) < 1) || version > 2) {
        return "Version " + args[2] + "is not supported.\n";
    }
    if (uid == -1) {
        return "Please login first.\n";
    }
    // TODO: Lead client to the appointed chat room.

    string history;
    for (auto record : data.chat_history[port].infos) {
        history.append(record.second.message + "\n");
    }
    return string("Welcome to public chat room.\n") + "Port:" + args[1] + "\n" + "Version:" + args[2] + "\n";
}

string Chat(const vector<string> &args, Data &data, int &uid) {
    if (args.size() != 3) {
        return "Unsupported input format detected.\n";
    }
    return "name = " + args[1] + "\nmesg = " + args[2] + "\n";
}

#endif
