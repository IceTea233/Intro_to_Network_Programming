#ifndef _CHAT_HPP_
#define _CHAT_HPP_

#include "Protocal.hpp"
#include "Data.hpp"
#include "Misc.hpp"

using namespace std;

// Chat Room operations
string EnterChatRoom(const vector<string> &args, Data &data, int &uid); // Usage: enter-chat-room <port> <version>

// This is implementation of "Chat" feature for server part.
string Chat(const vector<string> &args, Data &data, int sendfd, sockaddr_in cliaddr);

string EnterChatRoom(const vector<string> &args, Data &data, int &uid, sockaddr_in cliaddr) {
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

    User *user = data.users.access(uid);
    data.move_user_to_room(*user, port);
    user->chat_addr = cliaddr;
    string history;
    for (auto record : data.chat_history[port].infos) {
        history.append(record.second.message + "\n");
    }
    return string("Welcome to public chat room.\n") + "Port:" + args[1] + "\n" + "Version:" + args[2] + "\n";
}

string Chat(const vector<string> &args, Data &data, int sendfd, sockaddr_in cliaddr) {
    if (args.size() != 3) {
        return "Unsupported input format detected.\n";
    }

    User *user = data.find_user(cliaddr);
    int room = user->room;
    message_t mesg;
    mesg.flag = 1;
    mesg.name_len = args[1].size();
    mesg.mesg_len = args[2].size();
    memcpy(mesg.name, args[1].c_str(), mesg.name_len);
    memcpy(mesg.mesg, args[2].c_str(), mesg.mesg_len);
    for (auto recvid : data.room_member[room]) {
        user = data.users.access(recvid);
        mesg.version = user->chat_ver;
        message_pk pack;
        pack_message(&pack, mesg);
        sockaddr_in cliaddr = user->chat_addr;
        sendto(sendfd, pack.data, pack.len, 0, (sockaddr *) &cliaddr, sizeof(cliaddr));
    }
    return "name = " + args[1] + "\nmesg = " + args[2] + "\n";
}

#endif
