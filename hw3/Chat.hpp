#ifndef _CHAT_HPP_
#define _CHAT_HPP_

#include "Data.hpp"
#include "Misc.hpp"

using namespace std;

// Chat Room operations
string EnterChatRoom(const vector<string> &args, Data &data); // Usage: chat <message>
string Chat(const vector<string> &args, Data &data); //Usage: chat <message>



#endif
