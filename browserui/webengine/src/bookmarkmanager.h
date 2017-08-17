#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include <string>
#include <vector>
#include "json/json.h"

using namespace std;

class BookmarkManager
{
public:

    typedef enum
    {
        BK_SUCCESS = 0,
        BK_ERROR = 1,
        ALREADY_EXIST = 2,
        REACHED_LIMIT = 3,
        DO_NOTEXIST = 4
    } Result;

    typedef enum
    {
        BOOKMARK,
        SPEEDDIAL
    } ManagerType;
    BookmarkManager();
    BookmarkManager(string bookmarkStr);
    virtual ~BookmarkManager();
    Result addBookmark(string url, string title, string thumbNail);
    string getAllBookmarkString();
    string getBookmarkString(int index);
    bool removeBookmark(string url);
    bool updateThumbnail(string url, string thumbnail);
    bool updateTitle(string url, string title);
    void clear();
    void init(string bookmarkStr, int max_size, ManagerType type = SPEEDDIAL);
    bool moveBookmark(string url, int position);
    bool modifyBookmark(string url, string newUrl, string newTitle);
    int find(string url);
protected:
private:
    string m_allBookmarkStr;
    Json::Value m_allBookmarkValue;
    //Json::Value m_fastSearcher;
    int m_maxSize;
    ManagerType mType;
};

#endif // BOOKMARKMANAGER_H
