#ifndef _SQLITE_MANAGER_H_
#define _SQLITE_MANAGER_H_

#include <sqlite3.h>
#include <string>
#include <stdio.h>
#include "compoent-mode.h"

typedef int (*SQL_CALLBACK)(void*, int, char**, char**);

//Sqlite3 API  
typedef int (*SQL_OPEN)(const char*, sqlite3**);
typedef int (*SQL_CLOSE)(sqlite3*);
typedef int (*SQL_EXEC)(sqlite3*, const char*, int (*callback)(void*, int, char**, char**), void*, char**);
typedef int (*SQL_GET_TABLE)(sqlite3*, const char*, char***, int*, int*, char**);
typedef void (*SQL_FREE_TABLE)(char**);
typedef void (*SQL_FREE)(void*);

typedef const char*(*SQL_ERRMSG)(sqlite3*);
typedef int (*SQL_CONFIG)(int, ...);

class SqliteManager : public CompoentMode
{
public:
    SqliteManager();
    ~SqliteManager();

    ComStatus db_start();
    void db_stop();

    bool exec_command(std::string command, SQL_CALLBACK callback = NULL, void* callback_data = NULL);
    bool get_table(std::string command, char*** result, int* row, int* column);
    void free_table(char** table);
private:
    bool open_db(std::string db_name);
    bool close_db();
    bool init_sqlite_lib();
    bool deinit_sqlite_lib();

private:
    sqlite3* m_db;
    std::string m_db_dir;

    void* m_library;

    SQL_OPEN m_sqlite3_open;
    SQL_CLOSE m_sqlite3_close;
    SQL_EXEC m_sqlite3_exec;
    SQL_GET_TABLE m_sqlite3_get_table;
    SQL_FREE_TABLE m_sqlite3_free_table;
    SQL_FREE m_sqlite3_free;
    SQL_ERRMSG m_sqlite3_errmsg;
    SQL_CONFIG m_sqlite3_config;
};

#endif
