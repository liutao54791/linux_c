#include <sstream>

#include "bookmark_db_mgr.h"
#include "browser_debug.h"
#include "browser-impl.h"
#include "sqlite_mgr.h"
#include "systeminfo.h"
const std::string SpeedDialTableName = "SpeedDial";
const std::string BookmarkTableName = "Bookmark";
const std::string CountryGroup = "ESP FRA GBR DEU ITA";

BookmarkDBManager::BookmarkDBManager(BrowserImpl* browserImpl)
    : BrowserComponent(browserImpl)
    , m_type(SPEEDDIAL)
    , m_table_name(SpeedDialTableName)
    , m_country(""){
    BROWSER_LOG(INFO, "");
}

BookmarkDBManager::~BookmarkDBManager() {
    BROWSER_LOG(INFO, "");
}

bool BookmarkDBManager::initDB(int max_size, ManagerType type, std::string country) {
    m_type = type;
    m_maxSize = max_size;
    m_country = country;
    if(m_type == SPEEDDIAL) {
        m_table_name = SpeedDialTableName;
    } else {
        m_table_name = BookmarkTableName;
    }
    SqliteManager& sqliteMGR = browser().getSqliteManager();

    //create table table_name(id integer primary key, url text, title text, thumbnail text);
    std::string command = "create table " + m_table_name + "(id integer primary key, url text, title text, thumbnail text, country text);";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    if(!sqliteMGR.exec_command(command)) {
        BROWSER_LOG(ERROR, "Failed to create table");
    }

    return true;
}

BookmarkDBManager::Result BookmarkDBManager::addBookmark(std::string url, std::string title, std::string thumbnail) {
    Result result = BK_ERROR;
    if (url.empty() || title.empty()) {
        return result;
    }

    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //select * from table_name;
    std::string command = "select * from " + m_table_name + " where country = '"+m_country +"' or country ='ALL' or country ='' "+";";
    char** tables;
    int row, column;
    sqliteMGR.get_table(command, &tables, &row, &column);
    sqliteMGR.free_table(tables);
    if (m_type == BOOKMARK && row >= m_maxSize) {
        //delete from table_name where id = (select min(id) from table_name);
        command = "delete from " + m_table_name + " where id = (select min(id) from " + m_table_name + ");";
        BROWSER_LOG(INFO, "command : %s", command.c_str());
        sqliteMGR.exec_command(command);
    }
    if (m_type == SPEEDDIAL && row >= m_maxSize) {
        result = REACHED_LIMIT;
        return result;
    }

    //select * from table_name where url = 'url';
    command = "select * from " + m_table_name + " where url = '" + url + "' and (country = '"+m_country +"' or country ='ALL' or country ='' )"+";";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    row = 0;
    sqliteMGR.get_table(command, &tables, &row, &column);
    sqliteMGR.free_table(tables);
    if (row == 0) {
        //insert into table_name values(url = 'url',title = 'title',thumbnail='thumbnail');
        command = "insert into " + m_table_name + " values(null,'"  + url +"','" + title + "','" + thumbnail + "', '');";
        BROWSER_LOG(INFO, "command : %s", command.c_str());
        if (sqliteMGR.exec_command(command)) {
            result = BK_SUCCESS;
        }
    } else {
        result = ALREADY_EXIST;
    }

    return result;
}

bool BookmarkDBManager::removeBookmark(std::string url) {
    if (url.empty()) {
        return false;
    }

    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //delete from table_name where url = 'url';
    std::string command = "delete from " + m_table_name + " where url = '" + url + "' and (country = '"+m_country +"' or country ='ALL' or country ='')" + ";";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    return sqliteMGR.exec_command(command);
}

std::string BookmarkDBManager::getAllBookmark() {
    char** result;
    int row, column;
	std::string command;
    SqliteManager& sqliteMGR = browser().getSqliteManager();
	std::size_t found = CountryGroup.find(m_country);
    if(found ==std::string::npos)
    {
        BROWSER_LOG(ERROR, "The country is not the preset data!");
        command = "select * from " + m_table_name + " where country = '"+ m_country + "' or country ='ALL' or country =''"+";";
    }
    else
    {
    	BROWSER_LOG(INFO, "The country is : %s", m_country.c_str());
        command = "select * from " + m_table_name + " where country = '"+ m_country + "' or country =''"+";";
    }
    //select * from table_name;
    //std::string command = "select * from " + m_table_name + " where country = '"+ m_country + "' or country =''"+";";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    sqliteMGR.get_table(command, &result, &row, &column);
    BROWSER_LOG(INFO, "row : %d, column : %d", row, column);
    std::string json_str = "[";
    int i, j = 0;
    for(i = 1; i < row + 1; i++) {//don't append key to json string
        json_str.append("{");
        for(j = 1; j < column; j++) {//don't append id to json string
            json_str.append("\"" + std::string(result[j]) + "\"");
            json_str.append(":");
            json_str.append("\"" + std::string(result[i * column + j]) + "\"");
            if (j != column - 1) {
                json_str.append(",");
            }
        }
        json_str.append("}");
        if (i != row) {
            json_str.append(",");
        }
    }
    json_str.append("]");
    sqliteMGR.free_table(result);
    return json_str;
}

std::string BookmarkDBManager::getBookmarkByUrl(std::string url) {
    char** result;
    int row, column;
    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //select * from table_name;
    std::string command = "select * from " + m_table_name + " where url = '" + url + "' and (country = '"+m_country +"' or country ='')" + ";";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    sqliteMGR.get_table(command, &result, &row, &column);
    BROWSER_LOG(INFO, "row : %d, column : %d", row, column);
    std::string json_str = "[";
    int i, j = 0;
    for(i = 1; i < row + 1; i++) {//don't append key to json string
        json_str.append("{");
        for(j = 1; j < column; j++) {//don't append id to json string
            json_str.append("\"" + std::string(result[j]) + "\"");
            json_str.append(":");
            json_str.append("\"" + std::string(result[i * column + j]) + "\"");
            if (j != column - 1) {
                json_str.append(",");
            }
        }
        json_str.append("}");
        if (i != row) {
            json_str.append(",");
        }
    }
    json_str.append("]");
    sqliteMGR.free_table(result);
    return json_str;
}

bool BookmarkDBManager::updateThumbnail(std::string url, std::string thumbnail) {
    if (url.empty() || thumbnail.empty())
        return false;

    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //update table_name set title = 'title' where thumbnail = 'thumbnail';
    std::string command = "update " + m_table_name + " set thumbnail = '" + thumbnail + "' where url = '" + url + "' and (country = '"+m_country +"' or country ='ALL' or country ='')" + ";";
    //BROWSER_LOG(INFO, "command : %s", command.c_str());
    return sqliteMGR.exec_command(command) ? false : true;
}

bool BookmarkDBManager::updateTitle(std::string url, std::string title) {
    if (url.empty() || title.empty())
        return false;

    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //update table_name set title = 'title' where url = 'url';
    std::string command = "update " + m_table_name + " set title = '" + title + "' where url = '" + url + "' and (country = '"+m_country +"' or country ='ALL' or country ='')" + ";";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    return sqliteMGR.exec_command(command) ? false : true;
}

bool BookmarkDBManager::clear() {
    bool result = false;
    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //delete from table_name;
    std::string command = "delete from " + m_table_name + ";";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    if (sqliteMGR.exec_command(command)) {
        char** tables;
        int row, column;
        //select * from table_name;
        std::string command = "select * from " + m_table_name + ";";
        BROWSER_LOG(INFO, "command : %s", command.c_str());
        if (sqliteMGR.get_table(command, &tables, &row, &column) && row == 0) {
            result = true;
            BROWSER_LOG(ALWAYS, "Clear history success");
        }
        sqliteMGR.free_table(tables);
    }
    return result;
}

int BookmarkDBManager::find(std::string url) {
    char** tables;
    int row = 0;
    int column = 0;
    if (url.empty()) {
        return row;
    }

    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //select * from table_name where url = 'url';
    std::string command = "select * from " + m_table_name + " where url = '" + url + "' and (country = '"+m_country +"' or country ='ALL' or country ='')" + ";";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    sqliteMGR.get_table(command, &tables, &row, &column);
    sqliteMGR.free_table(tables);
    return row;
}


bool BookmarkDBManager::moveBookmark(std::string url, int position) {
    bool result = false;
    if (url.empty() || position >= m_maxSize || position < 0) {
        return result;
    }
    char** tables;
    int row, column = 0;
    SqliteManager& sqliteMGR = browser().getSqliteManager();
    std::stringstream stream;
    stream << position;
    //find specified postion values
    //select * from table_name order by id limit 1 offset position;
    std::string command = "select * from " + m_table_name + " order by id limit 1 offset " + stream.str() + ";";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    sqliteMGR.get_table(command, &tables, &row, &column);
    if (row != 1) {
        return result;
    }
    std::string position_id;
    std::string position_url, position_title, position_thumbnail;
    position_id = tables[1 * column + 0];
    position_url = tables[1 * column + 1];
    position_title = tables[1 * column + 2];
    position_thumbnail = tables[1 * column + 3];
    sqliteMGR.free_table(tables);

    row, column = 0;
    //find specified url values
    //select * from table_name where url = 'url';
    command = "select * from " + m_table_name + " where url = '" + url + "';";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    sqliteMGR.get_table(command, &tables, &row, &column);
    if (row != 1) {
        return result;
    }
    std::string url_id;
    std::string url_url, url_title, url_thumbnail;
    url_id = tables[1 * column + 0];
    url_url = tables[1 * column + 1];
    url_title = tables[1 * column + 2];
    url_thumbnail = tables[1 * column + 3];
    sqliteMGR.free_table(tables);

    //update table_name set url = 'url_url',title='url_title',thumbnail='url_thumbnail'  where id = position_id;
    command = "update " + m_table_name + " set url = '" + url_url +
                          "',title = '" + url_title +
                          "',thumbnail = '" + url_thumbnail + "' where id = " + position_id + ";";
    if (!sqliteMGR.exec_command(command)) {
        return result;
    }
    //update table_name set url = 'position_url',title='position_title',thumbnail='position_thumbnail'  where id = url_id;
    command = "update " + m_table_name + " set url = '" + position_url +
                          "',title = '" + position_title +
                          "',thumbnail = '" + position_thumbnail + "' where id = " + url_id + ";";
    if (!sqliteMGR.exec_command(command)) {
        return result;
    }
    result = true;
    return result;
}

bool BookmarkDBManager::modifyBookmark(std::string url , std::string newUrl , std::string newTitle) {
    bool result = false;
    if (url.empty() || newUrl.empty()) {
        return result;
    }

    SqliteManager& sqliteMGR = browser().getSqliteManager();
    if (url == newUrl) {
        //update table_name set title = 'newTitle' where url = 'url';
        std::string command = "update " + m_table_name + " set title = '" + newTitle + "' where url = '" + url + "';";
        BROWSER_LOG(INFO, "command : %s", command.c_str());
        if (sqliteMGR.exec_command(command)) {
            result = true;
        }
    } else if (find(newUrl) == 0){
        //update table_name set url = 'newUrl','title = 'newTitle' where url = 'url';
        std::string command = "update " + m_table_name + " set url = '" + newUrl + "',title = '" + newTitle + "' where url = '" + url + "';";
        BROWSER_LOG(INFO, "command : %s", command.c_str());
        if (sqliteMGR.exec_command(command)) {
            result = true;
        }
    }
    return result;
}
void BookmarkDBManager::setCountry(std::string country){
    std::string region = getregion();
    BROWSER_LOG(ALWAYS, "BookmarkDBManager get region is %s",region.c_str());
    if (region != "na")
    {
        if (country == "UK")
        {
            m_country = "GBR";
            BROWSER_LOG(ALWAYS, "UK has been change to %s",country.c_str());
        }
        else
        {
            m_country=country;
        }
    }
    else
    {
        m_country="UK";
    }
    BROWSER_LOG(ALWAYS, "BookmarkDBManager country is %s",m_country.c_str());
}


