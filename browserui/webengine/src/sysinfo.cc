#include <stdlib.h>

#include "systeminfo.h"
#include "browser_debug.h"
#include "sysinfo.h"

SysInfo* SysInfo::m_instatce = NULL;


SysInfo* SysInfo::getSysInfoInstance()
{
    if (m_instatce == NULL)
    {
        m_instatce = new SysInfo();
    }
    return m_instatce;
}

void SysInfo::delSysInfoInstance()
{
    if (m_instatce != NULL)
    {
        delete m_instatce;
        m_instatce = NULL;
    }
}

SysInfo::SysInfo()
{
    model_init();
    region_init();
}
SysInfo::~SysInfo() {}

void SysInfo::model_init()
{
    memset(m_model,0x00,sizeof(m_model));
/*
#ifdef APP_MARKET
    strncpy(m_model,APP_MARKET,
        (strlen(APP_MARKET)<(SYS_INFO_32_LEN-1)) ? strlen(APP_MARKET) : (SYS_INFO_32_LEN-1));
#else
    strncpy(m_model, "eu", strlen("eu"));
#endif
    BROWSER_LOG( ALWAYS,"Market Model: %s!", m_model );
*/
    
    std::string region = getregion();
    BROWSER_LOG( ALWAYS,"Market Model: %s!", region.c_str() );
    strncpy(m_model,region.c_str(),
        (strlen(region.c_str())<(SYS_INFO_32_LEN-1)) ? strlen(region.c_str()) : (SYS_INFO_32_LEN-1));
    
}

void SysInfo::region_init()
{
#define AREA_FILE  "AREA_FILE"
    char *area_file = getenv(AREA_FILE);

    memset(m_region,0x00,sizeof(m_region));
    if (NULL == area_file)
    {
        area_file = (char*)"/tmp/am_tv_region";
    }
    FILE* fp = fopen(area_file,"r");
    if (!fp)
    {
        memcpy(m_region,"0",strlen("0"));
    }
    else
    {
        fread(m_region,SYS_INFO_32_LEN, 1, fp);
        fclose(fp);
    }
}

char* SysInfo::getModel()
{
    if (m_model[0] == '\0')
    {
        model_init();
    }
    return m_model;
}

char* SysInfo::getRegion()
{
    if (m_region[0] == '\0')
    {
        region_init();
    }
    return m_region;
}

