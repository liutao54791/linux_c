#include "language.h"
#include "browser_debug.h"
#include "browser-opera.h"
#include "browser-impl.h"
#include "configuration.h"

BrowserLanguage::BrowserLanguage(BrowserImpl* browserImpl)
: BrowserComponent(browserImpl)
{
    TRACE_HERE();
}

BrowserLanguage::~BrowserLanguage()
{
    TRACE_HERE();
}

BrowserStatus BrowserLanguage::BrowserLanguageGet(std::string &language)
{
    Configuration & config = browser().getConfiguration();
    BROWSER_LANG_ID lang_id = config.getTVLanguageID();
    BrowserStatus status = idToName2(lang_id, language);

    BLOGA("lang_id:%d,(%s)", lang_id, language.c_str());
    return status;
}

BROWSER_LANG_ID BrowserLanguage::getIdForName(const std::string & name)
{

#define STRINGIFY(x) #x
#define LANG_MAP(idx,enm,n2,n3,c)  { enm,STRINGIFY(enm) }

    static const struct NameMap
    {
        BROWSER_LANG_ID m_id;
        const char * m_name;
    } s_map [] = {
#include LANG_MAP_FILE_NAME
        {UND, "UND"} //dummy one
    };
#undef LANG_MAP

    for (int i = 0; i< sizeof (s_map) / sizeof (s_map[0]); i++) {
        if (s_map[i].m_name == name) {
            return s_map[i].m_id;
        }
    }

    return UND;
}

std::string BrowserLanguage::getNameForId(BROWSER_LANG_ID lang_id)
{

#define STRINGIFY(x) #x
#define LANG_MAP(idx,enm,n2,n3,c)  { enm,STRINGIFY(enm) }

    static const struct NameMap
    {
        BROWSER_LANG_ID m_id;
        const char * m_name;
    } s_map [] = {
#include LANG_MAP_FILE_NAME
        {ENG, "ENG"},
        {FRA, "FRA"},
        {SPA, "SPA"},
        {POR, "POR"},
        {ARA, "ARA"},
        {RUS, "RUS"},
        {CHI, "CHI"},
        {VIE, "VIE"},
        {THA, "THA"},
        {MYA, "MYA"},
        {UZB, "UZB"},
        {PER, "PER"},
        {IND, "IND"},
        {HIN, "HIN"},
        {UKR, "UKR"},
        {MAL, "MAL"},
        {TUR, "TUR"},
        {HBR, "HBR"},
        {ZHO, "ZHO"},
        {DEU, "DEU"},
        {ITA, "ITA"},
        {SWE, "SWE"},
        {DAN, "DAN"},
        {FIN, "FIN"},
        {NOR, "NOR"},
        {CZE, "CZE"},
        {SLK, "SLK"},
        {POL, "POL"},
        {HUN, "HUN"},
        {BUL, "BUL"},
        {LAV, "LAV"},
        {EST, "EST"},
        {LIT, "LIT"},
        {SLO, "SLO"},
        {CRO, "CRO"},
        {SER, "SER"},
        {MAC, "MAC"},
        {ALB, "ALB"}//dummy one
    };
#undef LANG_MAP

    for (int i = 0; i< sizeof (s_map) / sizeof (s_map[0]); i++) {
        if (s_map[i].m_id == lang_id) {
            return s_map[i].m_name;
        }
    }

    return "UND";

}

BROWSER_LANG_ID BrowserLanguage::Name3Toid(const std::string & name3)
{

#define LANG_MAP(idx,enm,n2,n3,c)  { enm,n3 }

    static const struct NameMap
    {
        BROWSER_LANG_ID m_id;
        const char * m_name;
    } s_map [] = {
#include LANG_MAP_FILE_NAME
        {ENG, "eng"},
        {FRA, "fre"},
        {SPA, "spa"},
        {POR, "por"},
        {ARA, "ara"},
        {RUS, "rus"},
        {CHI, "chi"},
        {VIE, "vie"},
        {THA, "tha"},
        {MYA, "bur"},
        {UZB, "uzb"},
        {PER, "per"},
        {IND, "ind"},
        {HIN, "hin"},
        {UKR, "ukr"},
        {MAL, "mal"},
        {TUR, "tur"},
        {HBR, "hbr"},
        {ZHO, "zho"},
        {DEU, "ger"},
        {ITA, "ita"}, //意大利语
        {SWE, "swe"},
        {DAN, "dan"},
        {FIN, "fin"},
        {NOR, "NOR"},
        {CZE, "cze"},
        {SLK, "slk"},
        {POL, "pol"},
        {HUN, "hun"},
        {BUL, "bul"},
        {LAV, "lav"},
        {EST, "est"},
        {LIT, "lit"},
        {SLO, "slo"}, //斯洛文尼亚
        {CRO, "hrv"}, //克罗地亚
        {SER, "srp"}, //Serbian
        {MAC, "mac"}, //马其顿
        {ALB, "alb"} //阿尔巴尼亚
    };
#undef LANG_MAP

    for (int i = 0; i< sizeof (s_map) / sizeof (s_map[0]); i++) {
        if (s_map[i].m_name == name3) {
            return s_map[i].m_id;
        }
    }
    BLOGW("No languageid matches %s", name3.c_str());
    return UND;

}

BrowserStatus BrowserLanguage::idToName3(BROWSER_LANG_ID lang_id, std::string & name3)
{

#define LANG_MAP(idx,enm,n2,n3,c)  { enm,n3 }

    static const struct NameMap
    {
        BROWSER_LANG_ID m_id;
        const char * m_name;
    } s_map [] = {
#include LANG_MAP_FILE_NAME
        {UND, "eng"} //dummy one
    };
#undef LANG_MAP

    for (int i = 0; i< sizeof (s_map) / sizeof (s_map[0]); i++) {
        if (s_map[i].m_id == lang_id) {
            name3 = s_map[i].m_name;
            return BrowserOK;
        }
    }
    //FIXME: do we need provide the default value?
    name3 = "eng";
    return BrowserError;
}

BrowserStatus BrowserLanguage::idToName2(BROWSER_LANG_ID lang_id, std::string & name2)
{

#define LANG_MAP(idx,enm,n2,n3,c)  { enm,n2 }

    static const struct NameMap
    {
        BROWSER_LANG_ID m_id;
        const char * m_name;
    } s_map [] = {
#include LANG_MAP_FILE_NAME
        {ENG, "en"},
        {FRA, "fr"},
        {SPA, "es"},
        {POR, "pt"},
        {ARA, "ar"},
        {RUS, "ru"},
        {CHI, "zh-cn"},
        {VIE, "vi"},
        {THA, "th"},
        {MYA, "mm"},
        {UZB, "uz"},
        {PER, "fa"},
        {IND, "id"},
        {HIN, "hi"},
        {UKR, "uk"},
        {MAL, "ms"},
        {TUR, "tr"},
        {HBR, "he"},
        {ZHO, "zh-tw"},
        {DEU, "de"},
        {ITA, "it"},
        {SWE, "sv"},
        {DAN, "da"},
        {FIN, "fi"},
        {NOR, "nb"},
        {CZE, "cs"},
        {SLK, "sk"},
        {POL, "pl"},
        {HUN, "hu"},
        {BUL, "bg"},
        {LAV, "lv"},
        {EST, "et"},
        {LIT, "lt"},
        {SLO, "sl"},
        {CRO, "hr"},
        {SER, "sr"},
        {MAC, "mk"},
        {ALB, "sq"}//dummy one
    };
#undef LANG_MAP

    for (int i = 0; i< sizeof (s_map) / sizeof (s_map[0]); i++) {
        if (s_map[i].m_id == lang_id) {
            name2 = s_map[i].m_name;
            return BrowserOK;
        }
    }
    //FIXME: do we need provide the default value?
    name2 = "en";
    return BrowserError;
}

BrowserStatus BrowserLanguage::start()
{
    BROWSER_LOG(INFO, "BrowserLanguage start.");

#if 0
    if (BrowserLanguageSet(language)) {
        BROWSER_LOG(ERROR, "BrowserLanguageSet error!");
        return BrowserError;
    }
#endif

    return BrowserOK;
}

BrowserStatus BrowserLanguage::stop()
{
    BROWSER_LOG(INFO, "BrowserLanguage stop");
    return BrowserOK;
}
