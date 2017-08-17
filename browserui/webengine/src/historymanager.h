#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <time.h>
#include <string>
#include "json/json.h"

using namespace std;

class HistoryManager
{
public:
    /** Default constructor */
    HistoryManager();
    /** Default destructor */
    virtual ~HistoryManager();
    int addHistory(string url, string title, string icon);
    int clear();
    string getAllHistory();
    void init(string data, int max_size);
    void deleteSpecifiedHistory(string url, string specified_time);
    int modifyICON(string url, string icon = "");
    int modifyTitle(string url, string title = "");
protected:

private:
    int deleteHistory(string url, time_t start);
    time_t now();
    time_t todayBeginTime();
    int find(string url, time_t start = 0);
    Json::Value m_allHistoryValue;
    int m_maxSize;
};

#endif // HISTORYMANAGER_H


