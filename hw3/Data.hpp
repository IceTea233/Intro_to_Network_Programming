#ifndef _DATA_HPP_
#define _DATA_HPP_

#include <ctime>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <iostream>
#include <arpa/inet.h>

#define MAXPORT 65535

struct Info;
struct Post;
struct Comment;
struct User;
struct Board;

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

    Post(): Info(), board(NULL), author(NULL) {};
    Post(int arg1, std::string arg2, std::string arg3): Info(arg1, arg2), board(NULL), author(NULL), content(arg3) {};
    Post(std::string arg1, std::string arg2): Info(arg1), board(NULL), author(NULL), content(arg2) {};
};

struct Comment : Info {
    User *author;
    std::string content;

    Comment(): Info(), author(NULL) {};
    Comment(int arg1, std::string arg2, std::string arg3): Info(arg1, arg2), content(arg3) {};
    Comment(std::string arg1, std::string arg2): Info(arg1), content(arg2) {};
};

struct User : Info {
    int tcp_sock;
    sockaddr_in chat_addr;
    int chat_ver;
    int violate;
    int room;
    std::string pass;
    std::map<std::string, std::list<std::string>> msgbox;
    std::map<int, Post*> posts;

    User(): Info(), pass(""), tcp_sock(-1), violate(0), room(-1) {};
    User(int arg1, std::string arg2, std::string arg3): Info(arg1, arg2), pass(arg3), tcp_sock(-1), violate(0), room(-1) {};
    User(std::string arg1, std::string arg2): Info(arg1), pass(arg2), tcp_sock(-1), violate(0), room(-1) {};

    void add_post(Post *post) {
        post->author = this;
        posts[post->id] = post;
    }
    void comment_to_post(Post *post, Comment *comment) {
        comment->author = this;
        post->comments[comment->id] = comment;
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

struct Record : Info {
    User *author;
    std::string message;

    Record(): Info(), author(NULL), message("") {};
    Record(User *arg1, std::string arg2): Info("msg"), author(arg1), message(arg2) {};
};

template<typename T>
struct Infoset {
    int sn = 0; //series number
    std::map<int, T> infos; // map id to data
    std::map<std::string, int> dic; // map string to id


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
    void remove(std::string s) {
        int id = get_id(s);
        infos.erase(id);
        dic.erase(s);
    }
    void remove(int id) {
        std::string s = infos[id].name;
        infos.erase(id);
        dic.erase(s);
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
    T get(int id) {
        return infos[id];
    }
    T* access(std::string s) {
        return &infos[get_id(s)];
    }
    T* access(int id) {
        return &infos[id];
    }
};

struct Data {
    Infoset<User> users;
    Infoset<Board> boards;
    Infoset<Post> posts;
    Infoset<Comment> comments;
    Infoset<Record> chat_history[MAXPORT + 1];
    std::set<int> room_member[MAXPORT + 1];

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
    Record* add_record(int port, Record &record) {
        return chat_history[port].add(&record);
    }

    void remove_post(Post &post) {
        post.author->posts.erase(post.id);
        post.board->posts.erase(post.id);
        posts.remove(post.id);
    }

    User* find_user(sockaddr_in addr) {
        char buff[1000];
        printf("searching %s:%d among users...\n",
            inet_ntop(AF_INET, &addr.sin_addr.s_addr, buff, sizeof(buff)),
            (int) ntohs(addr.sin_port));
        User* ret = nullptr;
        for (auto &info : users.infos) {
            User *user = &(info.second);
            printf(">> %s:%d <<\n",
                inet_ntop(AF_INET, &user->chat_addr.sin_addr.s_addr, buff, sizeof(buff)),
                (int) ntohs(user->chat_addr.sin_port));
            if (user->chat_addr.sin_port == addr.sin_port && user->chat_addr.sin_addr.s_addr == addr.sin_addr.s_addr)
                ret = user;
        }
        return ret;
    }

    void move_user_to_room(User &user, int roomid) {
        room_member[user.room].erase(user.id);
        if (roomid >= 0)
            room_member[roomid].insert(user.id);
        user.room = roomid;
    }
};

#endif
