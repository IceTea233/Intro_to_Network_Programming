#ifndef _BASIC_HPP_
#define _BASIC_HPP_

#include "Data.hpp"
#include "Misc.hpp"

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
string CreatePost(const vector<string> &args, Data &data, int &uid);
string ListBoard(const vector<string> &args, Data &data);
string ListPost(const vector<string> &args, Data &data);
string Read(const vector<string> &args, Data &data);
string DeletePost(const vector<string> &args, Data &data, int &uid);
string UpdatePost(const vector<string> &args, Data &data, int &uid);
string CreateComment(const vector<string> &args, Data &data, int &uid);

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
    if (uid != -1 || data.users.get(args[1]).logged)
        return "Please logout first.\n";
    if (!data.users.exist(args[1]) || data.users.get(args[1]).pass != args[2]) {
        return "Login failed.\n";
    }
    User user = data.users.get(args[1]);
    if (data.users.get(args[1]).bad) {
        return "We don\'t welcome " + user.name + "!\n";
    }
    if (user.pass != args[2]) {
        return "Login failed.\n";
    }
    data.users.access(args[1])->logged = true;
    uid = user.id;

    return "Welcome, " + user.name + ".\n";
}

string Logout(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: logout\n";

    if (uid == -1)
        return "Please login first.\n";

    string tmp = data.users.get(uid).name;
    data.users.access(uid)->logged = false;
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

    string title, content;
    for (int i=1; i<args.size(); i++) {
        if (args[i-1] == "--title")
            title = args[i];
        if (args[i-1] == "--content")
            content = args[i];
    }
    if (!(args.size() == 6 && !title.empty() && !content.empty()))
        return "Usage: create-post <board-name> --title <title> --content <content>\n";
    if (uid == -1)
        return "Please login first.\n";
    if (!data.boards.exist(args[1]))
        return "Board does not exist.\n";

    Post post(title, content);
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
    ss << "S/N\tTitle\tAuthor\tDate\n";
    for (const auto post : data.boards.get(args[1]).posts) {
        ss << post.first << "\t" << post.second->name << "\t" << post.second->author->name << "\t" << post.second->time.tm_mon + 1 << "/" << post.second->time.tm_mday << "\n";
    }
    post_list = ss.str();

    return post_list;
}

string Read(const vector<string> &args, Data &data) {
    cout << "Receive request: read\n";

    if (args.size() != 2 || !isnum(args[1]))
        return "Usage: read <post-S/N>\n";
    if (!data.posts.exist(stoi(args[1])))
        return "Post does not exist.\n";

    stringstream ss;

    Post post = data.posts.get(stoi(args[1]));
    ss << "Author: " << post.author->name << "\n";
    ss << "Title: " << post.name << "\n";
    ss << "Date: " << post.time.tm_mon + 1 << "/" << post.time.tm_mday << "\n";
    ss << "--\n";

    string content;
    for (auto ch : post.content) {
        content.push_back(ch);
        if (content.size() >= 4 && content.substr(content.size() - 4, 4) == "<br>")
            content.replace(content.size() - 4, 4, "\n");
    }
    ss << content << "\n";
    ss << "--\n";
    for (auto comment : post.comments) {
        ss << comment.second->author->name << ": " << comment.second->content << "\n";
    }

    string output = ss.str();

    return output;
}

string DeletePost(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: delete-post\n";

    if (args.size() != 2 || !isnum(args[1]))
        return "Usage: delete-post <post-S/N>\n";
    int post_id = stoi(args[1]);
    if (uid == -1)
        return "Please login first.\n";
    if (!data.posts.exist(post_id))
        return "Post does not exist.\n";

    Post post = data.posts.get(post_id);
    if (uid != post.author->id)
        return "Not the post owner.\n";

    data.remove_post(post);
    return "Delete successfully.\n";
}

string UpdatePost(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: update-post\n";

    string title, content;
    for (int i=1; i<args.size(); i++) {
        if (args[i-1] == "--title")
            title = args[i];
        if (args[i-1] == "--content")
            content = args[i];
    }
    if (!(args.size() == 4 && isnum(args[1]) && !(title.empty() && content.empty())))
        return "Usage: update-post <post-S/N> --title/content <new>\n";
    int post_id = stoi(args[1]);
    if (uid == -1)
        return "Please login first.\n";
    if (!data.posts.exist(post_id))
        return "Post does not exist.\n";

    Post *post = data.posts.access(post_id);
    if (uid != post->author->id)
        return "Not the post owner.\n";

    if (!title.empty())
        post->name = title;
    if (!content.empty())
        post->content = content;

    return "Update successfully.\n";
}

string CreateComment(const vector<string> &args, Data &data, int &uid) {
    cout << "Receive request: comment\n";

    if (args.size() != 3 || !isnum(args[1]))
        return "Usage: comment <post-S/N> <comment>\n";
    int post_id = stoi(args[1]);
    if (uid == -1)
        return "Please login first.\n";
    if (!data.posts.exist(post_id))
        return "Post does not exist.\n";

    Comment comment("comment", args[2]);
    Comment *commented = data.add_comment(comment);

    Post *post = data.posts.access(post_id);
    User *user = data.users.access(uid);
    user->comment_to_post(post, commented);

    return "Comment successfully.\n";
}

#endif
