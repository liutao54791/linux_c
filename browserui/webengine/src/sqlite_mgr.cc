
#include <dlfcn.h>

#include "sqlite_mgr.h"
#include "browser_debug.h"
#include "browser-impl.h"
#include "configuration.h"

const std::string DEFAULT_BROWSER_DB_DIR = "/var/local/webengine/opera_home_browser/browser.db";
const std::string SQLITE3_LIB_PATH = "libsqlite3.so";

SqliteManager::SqliteManager(BrowserImpl* browserImpl)
    : BrowserComponent(browserImpl)
	, m_browserDB(NULL)
    , m_library(NULL) {
    BROWSER_LOG(INFO, "SqliteManager()");
}

SqliteManager::~SqliteManager() {
    BROWSER_LOG(INFO, "~SqliteManager()");
}

BrowserStatus SqliteManager::start() {
    BROWSER_LOG(INFO, "start");
    if (!init_sqlite_lib()) {
        BROWSER_LOG(ERROR, "Failed to init library for sqlite3");
    }

    m_db_dir = browser().getConfiguration().getValueByKey("BrowserDataBaseDir");
    if (m_db_dir.empty())
        m_db_dir = DEFAULT_BROWSER_DB_DIR;
    BROWSER_LOG(INFO, "Browser database dir : %s", m_db_dir.c_str());
    if (!open_db(m_db_dir)) {
        BROWSER_LOG(ERROR, "Failed to open sqlite3 database");
        return BrowserError;
    }

    return BrowserOK;
}

void SqliteManager::stop() {
    BROWSER_LOG(INFO, "");
    if (!close_db()) {
        BROWSER_LOG(ERROR, "Falied to close browser database");
    }
    deinit_sqlite_lib();
}

bool SqliteManager::exec_command(std::string command, SQL_CALLBACK callback, void* callback_data) {
    char* error_msg;
    if (m_sqlite3_exec(m_browserDB, command.c_str(), callback, callback_data, &error_msg ) != SQLITE_OK) {
        BROWSER_LOG(ERROR, "ERROR message : %s", error_msg);
        m_sqlite3_free(error_msg);
        return false;
    } else {
        return true;
    }
}

bool SqliteManager::get_table(std::string command, char*** result, int* row, int* column) {
    char* error_msg;
    if (m_sqlite3_get_table(m_browserDB, command.c_str(), result, row, column, &error_msg) != SQLITE_OK) {
        BROWSER_LOG(ERROR, "ERROR message : %s", error_msg);
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
    if ((result = m_sqlite3_open(db_name.c_str(), &m_browserDB)) != SQLITE_OK) {
        BROWSER_LOG(ERROR, "result : %d, error : %s", result, m_sqlite3_errmsg(m_browserDB));
        return false;
    }
    return true;
}

bool SqliteManager::close_db() {
    return m_sqlite3_close(m_browserDB) ? false : true;
}

bool SqliteManager::init_sqlite_lib() {
    m_library = dlopen(SQLITE3_LIB_PATH.c_str(), RTLD_LAZY);
    if (!m_library) {
        BROWSER_LOG(ERROR, "Failed to load sqlite3 library error : %s", dlerror());
        return false;
    }

    m_sqlite3_open = (SQL_OPEN)dlsym(m_library, "sqlite3_open");
    if (!m_sqlite3_open) {
        BROWSER_LOG(ERROR, "Failed to load sqlite3_open function error : %s", dlerror());
        return false;
    }

    m_sqlite3_close = (SQL_CLOSE)dlsym(m_library, "sqlite3_close");
    if (!m_sqlite3_close) {
        BROWSER_LOG(ERROR, "Failed to load sqlite3_close function error : %s", dlerror());
        return false;
    }

    m_sqlite3_exec = (SQL_EXEC)dlsym(m_library, "sqlite3_exec");
    if (!m_sqlite3_exec) {
        BROWSER_LOG(ERROR, "Failed to load sqlite3_exec function error : %s", dlerror());
        return false;
    }

    m_sqlite3_get_table = (SQL_GET_TABLE)dlsym(m_library, "sqlite3_get_table");
    if (!m_sqlite3_get_table) {
        BROWSER_LOG(ERROR, "Failed to load sqlite3_get_table function error : %s", dlerror());
        return false;
    }

    m_sqlite3_free_table = (SQL_FREE_TABLE)dlsym(m_library, "sqlite3_free_table");
    if (!m_sqlite3_free_table) {
        BROWSER_LOG(ERROR, "Failed to load sqlite3_free_table function error : %s", dlerror());
        return false;
    }

    m_sqlite3_free = (SQL_FREE)dlsym(m_library, "sqlite3_free");
    if (!m_sqlite3_free) {
        BROWSER_LOG(ERROR, "Failed to load sqlite3_free function error : %s", dlerror());
        return false;
    }

    m_sqlite3_errmsg = (SQL_ERRMSG)dlsym(m_library, "sqlite3_errmsg");
    if (!m_sqlite3_errmsg) {
        BROWSER_LOG(ERROR, "Failed to load sqlite3_errmsg function error : %s", dlerror());
        return false;
    }

#if 0
    m_sqlite3_config = (SQL_CONFIG)dlsym(m_library, "sqlite3_config");
    if (!m_sqlite3_config) {
        BROWSER_LOG(ERROR, "Failed to load sqlite3_config function error : %s", dlerror());
        return false;
    }
#endif

    return true;
}

bool SqliteManager::deinit_sqlite_lib() {
    dlclose(m_library);
    return true;
}
