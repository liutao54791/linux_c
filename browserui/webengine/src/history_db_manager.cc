#include <sstream>

#include "history_db_manager.h"
#include "browser_debug.h"
#include "sqlite_mgr.h"
#include "browser-impl.h"

const std::string HISTORY_TABLE_NAME = "history";

HistoryDBManager::HistoryDBManager(BrowserImpl* BrowserImpl)
    : BrowserComponent(BrowserImpl) {
    BROWSER_LOG(INFO, "");
}

HistoryDBManager::~HistoryDBManager() {
    BROWSER_LOG(INFO, "");
}

time_t HistoryDBManager::now()
{
    time_t _now = time(&_now);
    return _now;
}

bool HistoryDBManager::initDB(int max_size) {
    m_maxSize = max_size;
    SqliteManager& sqliteMGR = browser().getSqliteManager();

    //create table history(id integer primary key, time text, url text, title text, icon text);
    std::string command = "create table " + HISTORY_TABLE_NAME + "(id integer primary key, time text, url text, title text, icon text, count integer);";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    if(!sqliteMGR.exec_command(command)) {
        BROWSER_LOG(WARNING, "Failed to create table");
    }
    return true;
}

void HistoryDBManager::deleteStartTimeHistory(std::string url, time_t start_time) {
    BROWSER_LOG(INFO, "start time : %ld", start_time);
    std::stringstream stream;
    stream << start_time;

    //delete from history where time > 'start_time';
    std::string command = "delete from " + HISTORY_TABLE_NAME + " where url = '" + url + "' and time > '" + stream.str() + "000';";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    SqliteManager& sqliteMGR = browser().getSqliteManager();
    sqliteMGR.exec_command(command);
}

time_t HistoryDBManager::todayStartTime() {
    struct tm *_localTime;
    time_t _now = now();
    _localTime = localtime(&_now);
    _localTime->tm_hour = 0;
    _localTime->tm_min = 0;
    _localTime->tm_sec = 0;
    return mktime(_localTime);;
}

std::string HistoryDBManager::queryCountByUrl(std::string url){
	 char** result;
        int row, column;
        SqliteManager& sqliteMGR = browser().getSqliteManager();
	 std::string command ="select count from "+HISTORY_TABLE_NAME+" where url='"+url+"';";
	 BROWSER_LOG(INFO, "command : %s", command.c_str());
	 sqliteMGR.get_table(command, &result, &row, &column);
	 std::string count="";
	 if(row&&column){
            count=std::string(result[1]);
	 }
	 BROWSER_LOG(INFO, "row : %d, column : %d, count : %s", row, column,count.c_str());
	 sqliteMGR.free_table(result);
	return count;
}

int HistoryDBManager::addHistory(std::string url, std::string title, std::string icon) {
    std::string countStr=queryCountByUrl(url);
    int count=1;
    if(countStr!=""){
	 count=atoi(countStr.c_str())+1;
    }
    char countString[8] = {0};
    snprintf(countString,8,"%d",count);
    countStr=std::string(countString);

    deleteStartTimeHistory(url, todayStartTime());

    //select * from history;
    std::string command = "select * from " + HISTORY_TABLE_NAME + ";";
    SqliteManager& sqliteMGR = browser().getSqliteManager();
    char** result;
    int row, column;
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    sqliteMGR.get_table(command, &result, &row, &column);
    sqliteMGR.free_table(result);

    if(row >= m_maxSize) {
        //delete from history where time in (select time from history order by id limit 1);
        command = "delete from " + HISTORY_TABLE_NAME + " where time in (select time from " + HISTORY_TABLE_NAME + " order by id limit 1);";
        BROWSER_LOG(INFO, "command : %s", command.c_str());
        sqliteMGR.exec_command(command);
    }

    //insert into history values(null,time,url,title,icon);
    command = "insert into " + HISTORY_TABLE_NAME + " values(";
    command.append("null,");
    std::stringstream ss;
    time_t currentTime = now();
    ss << currentTime;
    command.append("'" + ss.str() + "000',");
    command.append("'" + url + "',");
    command.append("'" + title + "',");
    command.append("'" + icon + "',");
    command.append("1");
    command.append(");");
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    if(!sqliteMGR.exec_command(command)) {
        BROWSER_LOG(ERROR, "Failed to insert values");
    }
    if(countStr!=""){
	 updateCountByUrl(url,countStr);
    }
    return 0;
}
bool HistoryDBManager::updateCountByUrl(std::string url,std::string count){
	if(url.empty() ||count.empty()){
	     return false;
	}
	SqliteManager& sqliteMGR = browser().getSqliteManager();
	std::string command ="update "+HISTORY_TABLE_NAME+" set count = "+count+" where url='"+url+"';";
	BROWSER_LOG(INFO, "command : %s", command.c_str());
	return sqliteMGR.exec_command(command) ? false : true;
}
bool HistoryDBManager::modifyICON(std::string url, std::string icon) {
    if (url.empty() || icon.empty())
        return false;

    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //update history set icon = 'icon' where url = 'url';
    std::string command = "update " + HISTORY_TABLE_NAME + " set icon = '" + icon + "' where url = '" + url + "';";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    return sqliteMGR.exec_command(command) ? false : true;
}

bool HistoryDBManager::modifyTitle(std::string url, std::string title) {
    if (url.empty() || title.empty())
        return false;

    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //update history set title = 'title' where url = 'url';
    std::string command = "update " + HISTORY_TABLE_NAME + " set title = '" + title + "' where url = '" + url + "';";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    return sqliteMGR.exec_command(command) ? false : true;
}

bool HistoryDBManager::deleteSpecifiedHistory(std::string url, std::string specified_time) {
    if (url.empty() || specified_time.empty())
        return false;

    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //delete from history where url = 'url' and time = 'specified_time';
    std::string command = "delete from " + HISTORY_TABLE_NAME + " where url = '" + url + "' and time = '" + specified_time + "';";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    return sqliteMGR.exec_command(command) ? false : true;
}

int HistoryDBManager::clear() {
    int result = -1;
    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //delete from history;
    std::string command = "delete from " + HISTORY_TABLE_NAME + ";";
    BROWSER_LOG(INFO, "command : %s", command.c_str());
    if (sqliteMGR.exec_command(command)) {
        char** tables;
        int row, column;
        //select * from history;
        std::string command = "select * from " + HISTORY_TABLE_NAME + ";";
        BROWSER_LOG(INFO, "command : %s", command.c_str());
        if (sqliteMGR.get_table(command, &tables, &row, &column) && row == 0) {
            result = 0;
            BROWSER_LOG(ALWAYS, "Clear history success");
        }
        sqliteMGR.free_table(tables);
    }
    return result;
}

std::string HistoryDBManager::getAllHistory() {
    char** result;
    int row, column;
    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //select * from history;
    std::string command = "select * from " + HISTORY_TABLE_NAME + ";";
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
std::string HistoryDBManager::getTopHistory(std::string top) {
    char** result;
    int row, column;
    SqliteManager& sqliteMGR = browser().getSqliteManager();
    //select * from history;
    std::string command = "select * from " + HISTORY_TABLE_NAME + " order by count desc limit "+top+";";
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


