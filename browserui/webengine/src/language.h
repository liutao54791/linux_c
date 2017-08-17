#ifndef _BROWSER_LANGUAGE_
#define _BROWSER_LANGUAGE_

#include <string>

#include "browser_type.h"
#include "browser_component.h"

#define MAX_PATH_LEN  256
#define MAX_CFG_BUF   512
#define LANG_MAX_LEN  32
#define LANG_MAP_FILE_NAME  "language_map.hii"

#if defined(LANG_MAP)
#undef LANG_MAP
#endif

#define LANG_MAP(idx,enm,n2,n3,c)   enm = idx

typedef enum
{
    UND = 0,
#include  LANG_MAP_FILE_NAME

} BROWSER_LANG_ID;

#undef LANG_MAP

class BrowserLanguage : public BrowserComponent
{
public:
    BrowserLanguage(BrowserImpl* browserImpl);
    ~BrowserLanguage();

    BrowserStatus start();
    BrowserStatus stop();
    BrowserStatus BrowserLanguageGet(std::string &lang);

    static BrowserStatus idToName2(BROWSER_LANG_ID, std::string &);
    static BrowserStatus idToName3(BROWSER_LANG_ID, std::string &);
    static BROWSER_LANG_ID Name3Toid(const std::string & name3);
    static std::string getNameForId(BROWSER_LANG_ID);
    static BROWSER_LANG_ID getIdForName(const std::string &);
private:
    std::string m_lang;
    std::string m_ware;
    std::string m_model;
    std::string m_country;
    std::string m_string;

};

#endif
