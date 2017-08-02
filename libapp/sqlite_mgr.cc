
#include <dlfcn.h>

#include "sqlite_mgr.h"

const std::string DEFAULT_BROWSER_DB_DIR = "/home/liutao/linux_c/dbhome/mycode.db";
const std::string SQLITE3_LIB_PATH = "/home/liutao/linux_c/commom/libsqlite3.so";

SqliteManager::SqliteManager()
    : m_db(NULL)
    , m_library(NULL) {
    printf( "SqliteManager()\n");
}

SqliteManager::~SqliteManager() {
    printf( "~SqliteManager()\n");
}

ComStatus SqliteManager::db_start() {
    printf( "start\n");
    if (!init_sqlite_lib()) {
        printf( "Failed to init library for sqlite3\n");
    }

    m_db_dir = DEFAULT_BROWSER_DB_DIR;
    printf( "Browser database dir : %s\n", m_db_dir.c_str());
    if (!open_db(m_db_dir)) {
        printf( "Failed to open sqlite3 database\n");
        return Comfailed;
    }

    return Comok;
}

void SqliteManager::db_stop() {
    printf("SqliteManager::stop\n");
    if (!close_db()) {
        printf( "Falied to close browser database\n");
    }
    deinit_sqlite_lib();
}

bool SqliteManager::exec_command(std::string command, SQL_CALLBACK callback, void* callback_data) {
    char* error_msg;
    if (m_sqlite3_exec(m_db, command.c_str(), callback, callback_data, &error_msg ) != SQLITE_OK) {
        printf( "ERROR message : %s\n", error_msg);
        m_sqlite3_free(error_msg);
        return false;
    } else {
        return true;
    }
}

bool SqliteManager::get_table(std::string command, char*** result, int* row, int* column) {
    char* error_msg;
    if (m_sqlite3_get_table(m_db, command.c_str(), result, row, column, &error_msg) != SQLITE_OK) {
        printf( "ERROR message : %s\n", error_msg);
        m_sqlite3_free(error_msg);
        return false;
    } else {
        return true;
    }
}

void SqliteManager::free_table(char** table) {
    m_sqlite3_free_table(table);
}

bool SqliteManager::open_db(std::string db_name) {
    int result = SQLITE_ERROR;
    if ((result = m_sqlite3_open(db_name.c_str(), &m_db)) != SQLITE_OK) {
        printf( "result : %d, error : %s\n", result, m_sqlite3_errmsg(m_db));
        return false;
    }
    return true;
}

bool SqliteManager::close_db() {
    return m_sqlite3_close(m_db) ? false : true;
}

bool SqliteManager::init_sqlite_lib() {
    m_library = dlopen(SQLITE3_LIB_PATH.c_str(), RTLD_LAZY);
    if (!m_library) {
        printf( "Failed to load sqlite3 library error : %s\n", dlerror());
        return false;
    }

    m_sqlite3_open = (SQL_OPEN)dlsym(m_library, "sqlite3_open");
    if (!m_sqlite3_open) {
        printf( "Failed to load sqlite3_open function error : %s\n", dlerror());
        return false;
    }

    m_sqlite3_close = (SQL_CLOSE)dlsym(m_library, "sqlite3_close");
    if (!m_sqlite3_close) {
        printf( "Failed to load sqlite3_close function error : %s\n", dlerror());
        return false;
    }

    m_sqlite3_exec = (SQL_EXEC)dlsym(m_library, "sqlite3_exec");
    if (!m_sqlite3_exec) {
        printf( "Failed to load sqlite3_exec function error : %s\n", dlerror());
        return false;
    }

    m_sqlite3_get_table = (SQL_GET_TABLE)dlsym(m_library, "sqlite3_get_table");
    if (!m_sqlite3_get_table) {
        printf( "Failed to load sqlite3_get_table function error : %s\n", dlerror());
        return false;
    }

    m_sqlite3_free_table = (SQL_FREE_TABLE)dlsym(m_library, "sqlite3_free_table");
    if (!m_sqlite3_free_table) {
        printf( "Failed to load sqlite3_free_table function error : %s\n", dlerror());
        return false;
    }

    m_sqlite3_free = (SQL_FREE)dlsym(m_library, "sqlite3_free");
    if (!m_sqlite3_free) {
        printf( "Failed to load sqlite3_free function error : %s\n", dlerror());
        return false;
    }

    m_sqlite3_errmsg = (SQL_ERRMSG)dlsym(m_library, "sqlite3_errmsg");
    if (!m_sqlite3_errmsg) {
        printf( "Failed to load sqlite3_errmsg function error : %s\n", dlerror());
        return false;
    }

#if 0
    m_sqlite3_config = (SQL_CONFIG)dlsym(m_library, "sqlite3_config");
    if (!m_sqlite3_config) {
        printf( "Failed to load sqlite3_config function error : %s", dlerror());
        return false;
    }
#endif

    return true;
}

bool SqliteManager::deinit_sqlite_lib() {
    dlclose(m_library);
    return true;
}
