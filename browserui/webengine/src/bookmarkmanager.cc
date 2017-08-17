#include <iostream>
#include "bookmarkmanager.h"
#include <fstream>
#include <ostream>
#include "browser_debug.h"

BookmarkManager::BookmarkManager()
{
    //ctor
}

BookmarkManager::BookmarkManager(string bookmarkStr)
{
    Json::Reader reader;
    reader.parse(bookmarkStr , m_allBookmarkValue);
}

BookmarkManager::~BookmarkManager()
{
    //dtor
}

BookmarkManager::Result BookmarkManager::addBookmark(string url, string title, string thumbNail)
{
    Result result = BK_ERROR;
    if ( m_allBookmarkValue.size() >= m_maxSize && mType == BOOKMARK)
    {
        Json::Value o;
        m_allBookmarkValue.removeIndex(0, &o);
    }
    if( m_allBookmarkValue.size() >= m_maxSize &&mType == SPEEDDIAL){
        result = REACHED_LIMIT;
        return result;
    }
    if(find(url) < 0 && !url.empty() && !title.empty())
    {
        Json::Value o;
        o["url"] = url;
        o["title"] = title;
        o["thumbnail"] = thumbNail;
        m_allBookmarkValue.append(o);
        result = BK_SUCCESS;
    }
    else
    {
        result = ALREADY_EXIST;
    }

    return result;
}

string BookmarkManager::getAllBookmarkString()
{
    Json::FastWriter write;
    m_allBookmarkStr = write.write(m_allBookmarkValue);

    //cout << "AllBookmarkString" << endl << m_allBookmarkStr << endl;
    //BROWSER_LOG(ALWAYS,"AllBookmarkString:%s",m_allBookmarkStr.c_str());
    return m_allBookmarkStr;
}

string BookmarkManager::getBookmarkString(int index)
{
    Json::FastWriter write;
    string bookmarkstr = write.write(m_allBookmarkValue[index]);

    //cout << "Index=" << index << ",Bookmark=" << bookmarkstr << endl;
    //BROWSER_LOG(ALWAYS,"Index = %d,Bookmark = %s",index,bookmarkstr.c_str());
    return "";
}

bool BookmarkManager::removeBookmark(string url)
{
    int index = -1;
    bool result = false;
    index = find(url);
    if(index >= 0)
    {
        Json::Value o;
        result = m_allBookmarkValue.removeIndex(index, &o);
    }
    return result;
}

/*if have,return the index,or -1*/
int BookmarkManager::find(string url)
{
    int result = -1;
    int index = 0;
    int size = m_allBookmarkValue.size();
    for(;index < size;index ++){
        if(m_allBookmarkValue[index].get("url","").asString() == url)
        {
            result = index;
            break;
        }
    }
    return result;
}

bool BookmarkManager::updateThumbnail(string url, string thumbnail)
{
    bool result = false;
    BROWSER_LOG(ALWAYS,"updateThumbnail for url:%s",url.c_str());
    int index = find(url);
    if(index >= 0)
    {
        m_allBookmarkValue[index]["thumbnail"] = thumbnail;
        result = true;
    }
    return result;
}

bool BookmarkManager::updateTitle(string url, string title)
{
    bool result = false;
    BROWSER_LOG(ALWAYS,"updateTitle for url:%s",url.c_str());
    int index = find(url);
    if(index >= 0)
    {
        m_allBookmarkValue[index]["title"] = title;
        result = true;
    }
    return result;
}

void BookmarkManager::clear()
{
    m_allBookmarkValue.clear();
}

void BookmarkManager::init(string bookmarkStr,int max_size,ManagerType type)
{
    if ( max_size > 0 )
    {
        m_maxSize = max_size;
    }
    mType=type;
    Json::Reader reader;
    clear();
    reader.parse(bookmarkStr , m_allBookmarkValue);
}

bool BookmarkManager::moveBookmark(string url,int position)
{
    int curPosition = find(url);
    if(curPosition >= 0 && position != curPosition && position < m_allBookmarkValue.size()){
        Json::Value tmp = m_allBookmarkValue;
        m_allBookmarkValue.clear();
        int start,end,offset;
        if(curPosition > position){
            end = curPosition;
            start = position;
            offset = -1;
            m_allBookmarkValue[end] = tmp[end + offset];
        }else{
            end = position;
            start = curPosition;
            offset = 1;
            m_allBookmarkValue[start] = tmp[start + offset];
        }
        m_allBookmarkValue[position] = tmp[curPosition];
        int index = 0;

        for(;index < start;index ++){
            m_allBookmarkValue[index] = tmp[index];
        }
        for(index = start + 1;index < end;index ++){
            m_allBookmarkValue[index] = tmp[index + offset];
        }
        for(index = end + 1;index < tmp.size();index ++){
            m_allBookmarkValue[index] = tmp[index];
        }
        return true;
    }else{
        return false;
    }
}
bool BookmarkManager::modifyBookmark(string url , string newUrl , string newTitle){
    int urlIndex = find(url);
    int newUrlIndex = find(newUrl);
    if(url == newUrl || url != newUrl && urlIndex >= 0 && newUrlIndex < 0){
        m_allBookmarkValue[urlIndex]["title"] = newTitle;
        m_allBookmarkValue[urlIndex]["url"] = newUrl;
        return true;
    }
    return false;
}


