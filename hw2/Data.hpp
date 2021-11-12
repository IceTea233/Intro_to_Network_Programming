#ifndef _DATA_HPP_
#define _DATA_HPP_

#include <ctime>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>

struct Info;
struct User;
struct Post;
struct Board;
struct Comment;

struct Info {
    virtual ~Info() = default;

    int id;
    std::string name;

    Info(): id(-1), name("") {};
    Info(int arg1, std::string arg2): id(arg1), name(arg2) {};
    Info(std::string arg): name(arg) {};
};

struct Post : Info {
    Board *board;
    User *author;
    std::tm time;
    std::string content;
    std::map<int,Comment*> comments;

    Post(): Info(), board(NULL), author(NULL), content("") {};
    Post(int arg1, std::string arg2, std::string arg3): Info(arg1, arg2), board(NULL), author(NULL), content(arg3) {};
    Post(std::string arg1, std::string arg2): Info(arg1), board(NULL), author(NULL), content(arg2) {};
};

struct User : Info {
    std::string pass;
    std::map<std::string, std::list<std::string>> msgbox;
    std::map<int, Post*> posts;

    User(): Info(), pass("") {};
    User(int arg1, std::string arg2, std::string arg3): Info(arg1, arg2), pass(arg3) {};
    User(std::string arg1, std::string arg2): Info(arg1), pass(arg2) {};

    void add_post(Post *post) {
        post->author = this;
        posts[post->id] = post;
    }
};

struct Board : Info {
    User *moderator;
    std::map<int, Post*> posts; // Stored as pointer.

    Board(): Info(), moderator(NULL), posts() {};
    Board(int arg1, std::string arg2, User *arg3): Info(arg1, arg2), moderator(arg3) {};
    Board(std::string arg1, User *arg2): Info(arg1), moderator(arg2) {};

    void add_post(Post *post) {
        post->board = this;
        posts[post->id] = post;
    }
};

struct Comment : Info {
    User *from;
    std::string content;

    Comment(): Info(), from(NULL) {};
    Comment(int arg1, std::string arg2, User *arg3): Info(arg1, arg2), from(arg3) {};
    Comment(std::string arg1, User *arg2): Info(arg1), from(arg2) {};
};

template<typename T>
struct Infoset {
    int sn = 0;
    std::map<int, T> infos;
    std::map<std::string, int> dic;

    T* add(Info *info) {
        if (T *object = dynamic_cast<T*> (info)) {
            object->id = ++sn;
            infos[object->id] = *object;
            dic[object->name] = object->id;
            return &(infos[object->id]);
        } else { // temporary error detection
            std::cerr << "An unsuppored object has been attempted to add into the dataset and failed.\n";
            return NULL;
        }
    }
    bool exist(std::string s) {
        return dic.find(s) != dic.end();
    }
    bool exist(int id) {
        return infos.find(id) != infos.end();
    }
    int get_id(std::string s) {
        return dic[s];
    }
    T get(std::string s) {
        return infos[get_id(s)];
    }
    T get(int uid) {
        return infos[uid];
    }
    T* access(std::string s) {
        return &infos[get_id(s)];
    }
    T* access(int uid) {
        return &infos[uid];
    }
};

struct Data {
    Infoset<User> users;
    Infoset<Board> boards;
    Infoset<Post> posts;
    Infoset<Comment> comments;

    User* add_user(User &user) {
        return users.add(&user);
    }
    Board* add_board(Board &board) {
        return boards.add(&board);
    }
    Post* add_post(Post &post) {
        Post* ret = posts.add(&post);
        std::time_t t = std::time(nullptr);
        ret->time = *std::localtime(&t);
        return ret;
    }
    Comment* add_comment(Comment &comment) {
        return comments.add(&comment);
    }
};

#endif
