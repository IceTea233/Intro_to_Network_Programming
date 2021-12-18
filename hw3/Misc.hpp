#ifndef _MISC_HPP_
#define _MISC_HPP_

#include <string>
#include <vector>

bool isnum(std::string str) {
    for (auto it : str) {
        if (!isdigit(it))
            return false;
    }
    return true;
}

bool filter(std::string &str) {
    const static std::vector<std::string> fltlist = {"how", "you", "or", "pek0", "tea", "ha", "kon", "pain", "Starburst Stream"};
    bool flag = false;
    for (auto target : fltlist) {
        int pos;
        while ((pos = str.find(target)) != -1) {
            flag = true;
            for (int i = 0; i < (int) target.size(); i++) {
                str[pos + i] = '*';
            }
        }
    }

    return flag;
}

#endif
