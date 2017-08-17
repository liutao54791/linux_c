#ifndef _BROWSER_UTIL_H_
#define _BROWSER_UTIL_H_

#include <string>
#include <vector>

#include "browser_type.h"

class BrowserUtil
{
public:
    static std::vector<std::string> parse2vector(std::string argument);
    static void parseArguments(std::vector<std::string> argument, AppType& type, std::string& url, BrowserModeType& mode);

    static AppType m_storeType;
};

#endif