#ifndef _HISENSE_BOOKMARK_DATABASE_MANAGER_
#define _HISENSE_BOOKMARK_DATABASE_MANAGER_

#include <string>

#include "browser_component.h"

class BookmarkDBManager : public BrowserComponent
{
public:
    typedef enum
    {
        BOOKMARK,
        SPEEDDIAL
    } ManagerType;

    typedef enum
    {
        BK_SUCCESS,
        BK_ERROR,
        ALREADY_EXIST,
        REACHED_LIMIT,
        DO_NOTEXIST
    } Result;

    BookmarkDBManager(BrowserImpl* browserImpl);
    ~BookmarkDBManager();

    bool initDB(int max_size, ManagerType type = SPEEDDIAL, std::string = "");
    Result addBookmark(std::string url, std::string title, std::string thumbnail);
    bool removeBookmark(std::string url);
    std::string getAllBookmark();
    std::string getBookmarkByUrl(std::string url);
    bool updateThumbnail(std::string url, std::string thumbnail);
    bool updateTitle(std::string url, std::string title);
    bool clear();
    int find(std::string url);
    void setCountry(std::string country);
    bool moveBookmark(std::string url, int position);
    bool modifyBookmark(std::string url, std::string newUrl, std::string newTitle);
private:
    ManagerType m_type;
    std::string m_table_name;
    std::string m_country;
    int m_maxSize;
};

#endif
