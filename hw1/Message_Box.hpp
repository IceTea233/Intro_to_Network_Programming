#include <iostream>
#include <vector>
#include <string>

using namespace std;

string Send(const vector<string> &args, map<string, User> &data, User &user);
void ListMsg(const vector<string> *args);

string Send(const vector<string> &args, map<string, User> &data, User &user) {
    cout << "Receive request: send\n";

    return "Send executed\n";
}
