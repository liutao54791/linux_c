#ifndef _HISTORY_DATABASE_MANAGER_H_
#define _HISTORY_DATABASE_MANAGER_H_

#include <string>
#include <time.h>
#include "browser_component.h"

class HistoryDBManager : public BrowserComponent
{
public:
    HistoryDBManager(BrowserImpl* BrowserImpl);
    ~HistoryDBManager();

    bool initDB(int max_size = 100);
    int addHistory(std::string url, std::string title, std::string icon);
    bool modifyICON(std::string url, std::string icon);
    bool modifyTitle(std::string url, std::string title);
    bool updateCountByUrl(std::string url, std::string count);
    bool deleteSpecifiedHistory(std::string url, std::string specified_time);
    int clear();
    std::string getAllHistory();
    std::string getTopHistory(std::string top);

private:
    time_t now();
    void deleteStartTimeHistory(std::string url, time_t start_time);
    std::string queryCountByUrl(std::string url);
    time_t todayStartTime();

private:

    int m_maxSize;
};

#endif
