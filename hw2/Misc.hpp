#ifndef _MISC_HPP_
#define _MISC_HPP_

#include <string>

bool isnum(std::string str) {
    for (auto it : str) {
        if (!isdigit(it))
            return false;
    }
    return true;
}

#endif
