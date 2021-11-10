#ifndef _DATA_HPP_
#define _DATA_HPP_
#include "Data.hpp"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

// Account Administration
string Register(const vector<string> &args, Data &data);
string Login(const vector<string> &args, Data &data, int &uid);
string Logout(const vector<string> &args, Data &data, int &uid);
string Whoami(const vector<string> &args, Data &data, int &uid);
string ListUser(const vector<string> &args, Data &data);
string Exit(const vector<string> &args, Data &data, int &uid);

// Board Operations
string CreateBoard(const vector<string> &args, Data &data, int &uid);
string CreatePost(const vector<string> &args, Data &data, int &uid); // TODO: Date tag
string ListBoard(const vector<string> &args, Data &data);
string ListPost(const vector<string> &args, Data &data);
string Read(const vector<string> &args, Data &data);
string DeletePost(const vector<string> &args, Data &data, int &uid);
string UpdatePost(const vector<string> &args, Data &data, int &uid);
string Comment(const vector<string> &args, Data &data, int &uid);

string Register(const vector<string> &args, Data &data) {
    cout << "Receive request: register\n";
    if (args.size() != 3)
        return "Usage: register <username> <password>\n";
    if (data.users.exist(args[1]))
        return "Username is already used.\n";

    User user(args[1], args[2]);
    data.add_user(user);
    cout << "User " << user.name << " registered.\n";
    return "Register successfully.\n";
}

string Login(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: login\n";
    if (args.size() != 3)
        return "Usage: login <username> <password>\n";
    if (uid != -1)
        return "Please logout first.\n";
    if (!data.users.exist(args[1]) || data.users.get(args[1]).pass != args[2]) {
        return "Login failed.\n";
    }
    // if (data.find(args[1]) == data.end() || data[args[1]].pass != args[2])
    //     return "Login failed.\n";
    User user = data.users.get(args[1]);
    uid = user.id;

    return "Welcome, " + user.name + ".\n";
}

string Logout(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: logout\n";

    if (uid == -1)
        return "Please login first.\n";

    string tmp = data.users.get(uid).name;
    uid = -1;
    return "Bye, " + tmp + ".\n";
}

string Whoami(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: whoami\n";

    if (uid == -1)
        return "Please login first.\n";

    return data.users.get(uid).name + "\n";
}

string ListUser(const vector<string> &args, Data &data) {
    cout << "Receive request: list-user\n";

    string users;
    stringstream ss;
    for (const auto &it : data.users.dic) {
        ss << it.first << "\n";
    }
    users = ss.str();

    return users;
}

string Exit(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: exit\n";

    if (uid != -1) {
        string tmp = data.users.get(uid).name;
        uid = -1;
        return "Bye, " + tmp + ".\n";
    }

    return "";
}

// Implementation of Board functions.
string CreateBoard(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: create-board\n";

    if (args.size() != 2)
        return "Usage: create-board <name>\n";
    if (uid == -1)
        return "Please login first.\n";
    if (data.boards.exist(args[1]))
        return "Board already exists.\n";

    Board board(args[1], data.users.access(uid));
    data.add_board(board);
    cout << "Create a new Board: " << board.name << "; Moderator: " << board.moderator->name << "\n";

    return "Create board successfully.\n";
}

string CreatePost(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: create-post\n";

    if (!(args.size() == 6 && args[2] == "--title" && args[4] == "--content"))
        return "Usage: create-post <board-name> --title <title> --content <content>\n";
    if (uid == -1)
        return "Please login first.\n";
    if (!data.boards.exist(args[1]))
        return "Board does not exist.\n";

    Post post(args[3], args[5]);
    Post *posted = data.add_post(post);
    data.users.access(uid)->add_post(posted);
    data.boards.access(args[1])->add_post(posted);

    return "Create post successfully.\n";
}

string ListBoard(const vector<string> &args, Data &data) {
    cout << "Receive request: list-board\n";


    string board_list;
    stringstream ss;
    ss << "Index\tName\tModerator\n";
    for (const auto &it : data.boards.infos) {
        ss << it.first << "\t" << it.second.name << "\t" << it.second.moderator->name << "\n";
    }
    board_list = ss.str();

    return board_list;
}

string ListPost(const vector<string> &args, Data &data) {
    cout << "Receive request: list-post\n";

    if (args.size() != 2)
        return "Usage: list-post <board-name>\n";
    if (!data.boards.exist(args[1]))
        return "Board does not exist.\n";

    string post_list;
    stringstream ss;
    ss << "S/N\tTitle\tAuthor\tData\n";
    for (const auto post : data.boards.get(args[1]).posts) {
        ss << post.first << "\t" << post.second->name << "\t" << post.second->author->name << "\n";
    }
    post_list = ss.str();

    return post_list;
}
