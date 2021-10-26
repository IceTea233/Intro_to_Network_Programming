#include <string>
#include <vector>
#include <map>

struct User {
    std::string username;
    std::string pass;
    bool login;
    std::map<std::string, std::vector<std::string>> msg_box;

    User(): username(""), pass(""), login(false) {};
    User(std::string arg1, std::string arg2): username(arg1), pass(arg2), login(false){};
};
