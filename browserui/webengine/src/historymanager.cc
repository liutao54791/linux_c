#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "historymanager.h"
#include "browser_debug.h"

#define MAX_HISTORY_SIZE 100

HistoryManager::HistoryManager()
    : m_maxSize( MAX_HISTORY_SIZE )
{
    //ctor
}

HistoryManager::~HistoryManager()
{
    //dtor
}

time_t HistoryManager::now()
{
    time_t _now = time(&_now);
    return _now;
}

int HistoryManager::addHistory(string url, string title, string icon)
{
    time_t _currentTime = now();
    if (deleteHistory(url, todayBeginTime()) < 0 && m_allHistoryValue.size() >= m_maxSize)
    {
        Json::Value o;
        m_allHistoryValue.removeIndex(0, &o);
    }

    Json::Value _history;
    stringstream ss;
    ss << _currentTime;
    _history["time"] = ss.str() + "000";
    _history["url"] = url;
    _history["title"] = title;
    _history["icon"] = icon;
    m_allHistoryValue.append(_history);
    return 0;
}

int HistoryManager::deleteHistory(string url, time_t start)
{
    int result = -1;
    result = find(url, start);
    if(result >= 0)
    {
        Json::Value o;
        m_allHistoryValue.removeIndex(result, &o);
    }
    return result;
}


int HistoryManager::modifyICON(string url, string icon)
{
    Json::Value history;
    if ( icon == "" )
        return 0;

    int index = 0;
    int size = m_allHistoryValue.size();
    for(; index < size; index++)
    {
        if(m_allHistoryValue[index].get("url", "").asString() == url)
        {
            history = m_allHistoryValue[index];
            history["icon"] = icon;
            m_allHistoryValue[index].swap(history);
        }
    }

    return index;
}

int HistoryManager::modifyTitle(string url, string title)
{
    Json::Value history;
    if ( title == "" )
        return 0;

    int index = 0;
    int size = m_allHistoryValue.size();
    for(; index < size; index++)
    {
        if(m_allHistoryValue[index].get("url", "").asString() == url)
        {
            history = m_allHistoryValue[index];
            history["title"] = title;
            m_allHistoryValue[index].swap(history);
        }
    }

    return index;
}

void HistoryManager::deleteSpecifiedHistory(string url, string specified_time)
{
    if ( specified_time == "0" )
        return;

    BROWSER_LOG( INFO, "url : %s, time : %s", url.c_str(), specified_time.c_str() );
    int index = 0;
    int size = m_allHistoryValue.size();
    for( index = 0; index < size; index ++ )
    {
        if ( m_allHistoryValue[index].get("url", "").asString() == url &&
            m_allHistoryValue[index].get("time", "0").asString() == specified_time)
        {
            Json::Value o;
            m_allHistoryValue.removeIndex(index, &o);
            break;
        }
    }
}

int HistoryManager::clear()
{
    int result = -1;
    m_allHistoryValue.clear();
    if (m_allHistoryValue.empty())
    {
        BROWSER_LOG(INFO, "Clear History succeed");
        result = 0;
    }
    return result;
}

int HistoryManager::find(string url,time_t start)
{
    int result = -1;
    int index = 0;
    int size = m_allHistoryValue.size();
    for(;index < size;index ++){
        if((m_allHistoryValue[index].get("url","").asString() == url )&&
           (atoll(m_allHistoryValue[index].get("time","0").asString().c_str()) > start))
        {
            result = index;
            break;
        }
    }
    return result;
}

void HistoryManager::init(string data, int max_size)
{
    if ( max_size > 0 )
    {
        m_maxSize = max_size;
    }
    Json::Reader reader;
    clear();
    reader.parse(data , m_allHistoryValue);
}

string HistoryManager::getAllHistory()
{
    Json::FastWriter writer;
    if (m_allHistoryValue.empty())
        return "";
    else
        return writer.write(m_allHistoryValue);
}

time_t HistoryManager::todayBeginTime()
{
    struct tm *_localTime;
    time_t _now = now();
    _localTime = localtime(&_now);
    _localTime->tm_hour = 0;
    _localTime->tm_min = 0;
    _localTime->tm_sec = 0;
    return mktime(_localTime) * 1000;
}

