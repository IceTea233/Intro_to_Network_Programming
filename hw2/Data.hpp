#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>

struct Info {
    virtual ~Info() = default;

    int id;
    std::string name;

    Info(): id(-1), name("") {};
    Info(int arg1, std::string arg2): id(arg1), name(arg2) {};
    Info(std::string arg): name(arg) {};
};

struct User : Info {
    std::string pass;
    bool login;
    std::map<std::string, std::list<std::string>> msgbox;

    User(): Info(), pass(""), login(false) {};
    User(int arg1, std::string arg2, std::string arg3): Info(arg1, arg2), pass(arg3), login(false) {};
    User(std::string arg1, std::string arg2): Info(arg1), pass(arg2), login(false) {};
};

struct Post : Info {
    int board_id;
    std::string name;
    std::string content;

    Post(): Info(), content("") {};
    Post(int arg1, std::string arg2, std::string arg3): Info(arg1, arg2), content(arg3) {};
    Post(std::string arg1, std::string arg2): Info(arg1), content(arg2) {};
};

struct Board : Info {
    std::string moderator;
    std::vector<int> posts; // only id was stored.

    Board(): Info(), moderator(""), posts({}) {};
    Board(int arg1, std::string arg2, std::string arg3): Info(arg1, arg2), moderator(arg3) {};
    Board(std::string arg1, std::string arg2): Info(arg1), moderator(arg2) {};
};

template<typename T>
struct Infoset {
    int sn = 0;
    std::map<int, T> infos;
    std::map<std::string, int> dic;

    void add(Info *info) {
        if (T *object = dynamic_cast<T*> (info)) {
            object->id = ++sn;
            infos[object->id] = *object;
            dic[object->name] = object->id;
        } else { // temporary error detection
            std::cerr << "An unsuppored object has been attempted to add into the dataset and failed.\n";
        }
    }
};

struct Data {
    Infoset<User> users;
    Infoset<Board> boards;
    Infoset<Post> posts;

    void add_user(User user) {
        users.add(&user);
    }
    void add_board(Board board) {
        boards.add(&board);
    }
    void add_post(Post post) {
        posts.add(&post);
    }
};
