#include <iostream>
#include <vector>
#include <string>

using namespace std;

void Send(const vector<string> *args);
void ListMsg(const vector<string> *args);

void Send(const vector<string> *args) {
    cout << "Send executed\n";
}
