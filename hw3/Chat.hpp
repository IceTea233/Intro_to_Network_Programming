#ifndef _CHAT_HPP_
#define _CHAT_HPP_

#include "Data.hpp"
#include "Misc.hpp"

using namespace std;

// Chat Room operations
string EnterChatRoom(const vector<string> &args, Data &data, int &uid); // Usage: enter-chat-room <port> <version>

// This is implemented in Client application.
// string Chat(const vector<string> &args, Data &data); //Usage: chat <message>


string EnterChatRoom(const vector<string> &args, Data &data, int &uid) {
    if (args.size() < 3) {
        return "Usage: enter-chat-room <port>";
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

#endif
