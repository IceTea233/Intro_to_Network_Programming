#include <iostream>
#include <vector>
#include <string>

using namespace std;

void Register(const vector<string> &args);
void Login(const vector<string> &args);
void logout(const vector<string> &args);
void Whoami(const vector<string> &args);
void ListUser(const vector<string> &args);
void Exit(const vector<string> &args);

void Register(const vector<string> &args) {
    cout << "Register executed\n";
}
