#ifndef _CHAT_HPP_
#define _CHAT_HPP_

#include "Protocal.hpp"
#include "Data.hpp"
#include "Misc.hpp"
#include "Basic.hpp"

using namespace std;

// Chat Room operations
string EnterChatRoom(const vector<string> &args, Data &data, int &uid); // Usage: enter-chat-room <port> <version>

// This is implementation of "Chat" function for server part. Internal calling only.
string Chat(const vector<string> &args, Data &data, int sendfd, sockaddr_in cliaddr);

string EnterChatRoom(const vector<string> &args, Data &data, int &uid, sockaddr_in cliaddr) {
    // TODO: Issue: Different user on same address may cause duplicate message received.
    cout << "Receive request: enter-chat-room\n";

    if (args.size() != 3) {
        return "Usage: enter-chat-room <port> <version>\n";
    }
    int port, version;
    if (!isnum(args[1]) || (port = stoi(args[1])) < 1 || port > 65535) {
        return "Port " + args[1] + " is not valid.\n";
    }
    if (!isnum(args[2]) || (version = stoi(args[2])) < 1 || version > 2) {
        return "Version " + args[2] + " is not supported.\n";
    }
    if (uid == -1) {
        return "Please login first.\n";
    }

    User *user = data.users.access(uid);
    data.move_user_to_room(*user, 0); // Always send the user to the public room aka room #0.
    user->chat_addr = cliaddr;
    user->chat_addr.sin_port = htons(port);
    user->chat_ver = version;
    string history;
    for (auto record : data.chat_history[0].infos) {
        string name = record.second.author->name;
        string message = record.second.message;
        filter(message);
        history.append(name + ":" + message + "\n");
    }
    return string("Welcome to public chat room.\n") + "Port:" + to_string(port) + "\n" + "Version:" + to_string(version) + "\n" + history;
}

// This is implementation of "Chat" function for server part. Internal calling only.
string Chat(message_t mesg, Data &data, int sendfd, sockaddr_in cliaddr) {
    // if (args.size() != 3) {
    //     return "Unsupported input format detected.\n";
    // }
    string name = string((char *) mesg.name);
    string message = string((char *) mesg.mesg);

    User *user = data.users.access(name);
    Record record(user, message);
    data.add_record(0, record);

    int room = user->room;

    if (filter(message))
        user->violate ++;
    if (user->violate >= 3) {
        data.move_user_to_room(*user, -1);
    }

    mesg.name_len = name.size();
    mesg.mesg_len = message.size();
    strcpy((char*) mesg.name, name.c_str());
    strcpy((char*) mesg.mesg, message.c_str());
    for (auto recvid : data.room_member[room]) {
        printf("handle user %d\n", recvid);
        user = data.users.access(recvid);
        mesg.version = user->chat_ver;
        message_pk pack;
        printf("Ready to pack message.\n");
        printf("flag -> %d\n", mesg.flag);
        printf("version -> %d\n", mesg.version);
        printf("name_len -> %d\n", mesg.name_len);
        printf("name -> [%s]\n", mesg.name);
        printf("mesg_len -> %d\n", mesg.mesg_len);
        printf("mesg -> [%s]\n", mesg.mesg);
        pack_message(&pack, mesg);
        sockaddr_in cliaddr = user->chat_addr;
        sendto(sendfd, pack.data, pack.len, 0, (sockaddr *) &cliaddr, sizeof(cliaddr));
    }
    printf("Chat OK\n");
    // return "Chat: name = " + name + "; mesg = " + message + "\n";
    return "Chat OK\n";
}

#endif
