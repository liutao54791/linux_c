
#define SYS_INFO_32_LEN 32
#define SYS_INFO_128_LEN 128

class SysInfo
{
public:
    static SysInfo* getSysInfoInstance();
    static void delSysInfoInstance();

public:
    ~SysInfo();
    char* getModel();
    char* getRegion();

private:
    static SysInfo* m_instatce;
    SysInfo();

    void model_init();
    void region_init();

private:
    char m_model[SYS_INFO_32_LEN];
    char m_region[SYS_INFO_32_LEN];
};

