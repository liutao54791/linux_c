#include "memory-controller.h"
#include "browser_debug.h"
#include "configuration.h"
#include "browser-impl.h"


#include <sys/sysinfo.h>
#include <unistd.h>


const int DROP_CACHE_TIME = 10;
const int OUT_OF_MEMROY_SIZE = 26214400;
const int OUT_OF_FREE_MEMORY_SIZE=10485760;
const std::string DROP_CACHE_FILE = "/proc/sys/vm/drop_caches";
const int DROP_CACHE_FLAG = 3;


MemroyController::MemroyController(BrowserImpl* browserImpl)
    : BrowserComponent(browserImpl), m_dropCacheTime(DROP_CACHE_TIME)
    , m_outOfMemroySize(OUT_OF_MEMROY_SIZE)
    ,m_outOfFreeMemorySize(OUT_OF_FREE_MEMORY_SIZE){
    BROWSER_LOG(INFO, "");
}

MemroyController::~MemroyController() {
    BROWSER_LOG(INFO, "");
}

BrowserStatus MemroyController::start()
{
    std::string dropCacheTime = browser().getConfiguration().getValueByKey("DropCacheTime");
    if(!dropCacheTime.empty()) {
        m_dropCacheTime = atoi(dropCacheTime.c_str());
    }

    std::string outOfMemroySize = browser().getConfiguration().getValueByKey("OutOfMemroySize");
    if(!outOfMemroySize.empty()) {
        m_outOfMemroySize = atoi(outOfMemroySize.c_str());
    }

    std::string outOfFreeMemorySize = browser().getConfiguration().getValueByKey("OutOfFreeMemorySize");
    if(!outOfFreeMemorySize.empty()){
        m_outOfFreeMemorySize = atoi(outOfFreeMemorySize.c_str());
    }

    set_oom_adj();

    return BrowserOK;
}


BrowserStatus MemroyController::stop()
{
    BROWSER_LOG( INFO, "" );
    return BrowserOK;
}

void MemroyController::set_oom_adj() {
    const int MAX_SIZE = 128;
    const int MAX_PID_PRIORITY = 15;
    int pid = 0;
    pid = getpid();
    if (pid == 0) {
        BROWSER_LOG(ERROR, "Failed to get the current pid");
        return;
    }
    char absolute_pid[MAX_SIZE] = {0};
    snprintf(absolute_pid, MAX_SIZE, "/proc/%d/oom_score_adj", pid);

    BROWSER_LOG(ALWAYS, "absolute_pid : %s", absolute_pid);

    FILE *file_pid = NULL;
    file_pid = fopen (absolute_pid, "wb");
    if (!file_pid)
        BROWSER_LOG(ERROR, "Failed to open oom_adj of current pid!");
    else
        fprintf(file_pid, "%d", MAX_PID_PRIORITY);

    fclose(file_pid);
}

void MemroyController::drop_cache() {
    BROWSER_LOG(ERROR,"drop_cache !!!!!!!!!!!!!!!!!");

    FILE *fd = NULL;
    fd = fopen (DROP_CACHE_FILE.c_str(), "wb");
    if (!fd)
        BROWSER_LOG(ERROR, "Failed to open :%s!", DROP_CACHE_FILE.c_str());
    else
        fprintf(fd, "%d", DROP_CACHE_FLAG);

    fclose(fd);
    //system("top -bn 1");
}


void MemroyController::dumpMemory() {
    struct sysinfo s_info;
    int error;
    error = sysinfo(&s_info);
    if (error != 0) {
        BROWSER_LOG( ERROR, "Failed to get system info." );
        return ;
    }
    BROWSER_LOG(ALWAYS, "TotalRAM:%ld,FreeRAM:%ld"
                        ", SharedRAM:%ld,BufferRAM:%ld"
                        ", TotalSwap:%ld,FreeSwap:%ld"
                        ,s_info.totalram,s_info.freeram
                        ,s_info.sharedram,s_info.bufferram
                        ,s_info.totalswap,s_info.freeswap);
}


bool MemroyController::memoryCheck() {
    static int drop_cache_cnt = 0;
    static int times = m_dropCacheTime;

    struct sysinfo s_info;
    int free_mem = 0;
    int error;
    error = sysinfo(&s_info);
    if (error != 0) {
        BROWSER_LOG( ERROR, "Failed to get system info." );
        return false;
    }

    free_mem = s_info.freeram + s_info.bufferram + s_info.freeswap;

    if(free_mem <= m_outOfMemroySize /*|| s_info.freeram <= m_outOfFreeMemorySize*/) {
        if(times >= m_dropCacheTime) {
            drop_cache_cnt = 0;
            times = 0;
        }
        dumpMemory();
        if(times < m_dropCacheTime && drop_cache_cnt >= 3) {
            times = m_dropCacheTime;
            drop_cache_cnt = 0;
            return true;
        } else {
            drop_cache();
            drop_cache_cnt ++;
        }
    }
    times++;

    return false;
}


