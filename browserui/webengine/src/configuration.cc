#include <iostream>
#include <fstream>
#include <string>

#include <opera/components/integration_utils.h>
#include "tts/TTSClient.h"
#include "configuration.h"
#include "language.h"
#include "systeminfo.h"
#include "browser_debug.h"
#include "hs_dtv_access.h"
#include "base/log/browser_log.h"
#include "NotImplemented.h"

#define TTS_CUSTOMER_INI "/tvcustomer/customer/ttsplayer.ini"
//static const std::string browser_config_file = "browser.config";

Configuration::Configuration(BrowserImpl* browser, int argc, char** argv)
: BrowserComponent(browser)
, m_commandLine(NULL)
{
    ///TODO:
    for (int i = 0; i < argc; i++) {
        BROWSER_LOG(INFO, "argv[%d] = %s", i, argv[i]);
    }

    m_commandLine.reset(new opera::components::CommandLine(argc, argv));
    m_commandLine->AddOption("no-ime", 'i', true, "Disable IME"); // "-i" no ime
    m_commandLine->AddOption("no-virtual-keyboard", 'K', true, "Disable the virtual keyboard"); // "-K" no virtual keyboard
    m_commandLine->AddOption("enable-url-filter", 'f', true, "Enable the URL filtering"); // "-f" enable url filter
    m_commandLine->AddOption("tvstore-ua-string", 'a', true, "A suffix to add to the User Agent string, (eg \"Model/Foo_123\")", true); // Have argument "-a" for ua string
    m_commandLine->AddOption("ait", 'A', true, "Load raw AIT data from binary file", true);
    m_commandLine->AddOption("dsmcc", 'D', true, "Specify directory to mock DSM-CC from", true);
    m_commandLine->AddOption("config-file", 'C', true, "WebEngine config file", true);
    ///TODO:Other command line
    m_commandLine->Parse();
    std::string default_config_file = "browser.config";

    std::string config_file = m_commandLine->GetOptionValue("config-file");
    if (config_file.length() > 0) {
        m_configFile = config_file;
        BROWSER_LOG(ALWAYS, "Hisense browser config file from command line : %s", m_configFile.c_str());
    } else {
        std::string opera_dir = getenv("OPERA_DIR");
        if (!opera_dir.empty()) {
            m_configFile = opera_dir + "/" + default_config_file;
            BROWSER_LOG(ALWAYS, "Hisense browser config file from operadir : %s", m_configFile.c_str());
        } else {
            BROWSER_LOG(ERROR, "Failed to get Hisense Browser config file");
        }
    }

    readJsonFile();
}

Configuration::~Configuration()
{
    BROWSER_LOG(INFO, "");
}

BrowserStatus Configuration::start()
{
    BrowserStatus result = BrowserOK;



    opera::components::system_clock::VerifyClock(opera::components::system_clock::ATTEMPT_ADJUSTING_FROM_NTP
                                                 | opera::components::system_clock::SET_TO_EXE_DATE
                                                 | opera::components::system_clock::TERMINATE_IF_INVALID);

    return result;
}

BrowserStatus Configuration::stop()
{
    BROWSER_LOG(INFO, "");
    return BrowserOK;
}

opera::components::CommandLine& Configuration::getCommandLine()
{
    if (m_commandLine)
        return *m_commandLine;
}

void Configuration::readJsonFile()
{
    Json::Reader reader;
    //Json::Value root;

    std::ifstream is;
    is.open(m_configFile, std::ios::binary);

    if (!reader.parse(is, m_root, false))
        BROWSER_LOG(ERROR, "Json::Reader parse error");

    is.close();
}

std::string Configuration::getValueByKey(std::string key)
{
    std::string value = m_root[key].asString();
    return value;
}

template < typename FUNTYPE >
static std::string getStringValueDTVACCESS(FUNTYPE fun, const char * debugName)
{
    std::string result = "";
    lo::dtvaccess::HSDTVAccess *hsDTVAccess = lo::dtvaccess::HSDTVAccess::getInstance();
    if (!hsDTVAccess) {
        BLOGE("Get %s failed! ,No DTVAccess ", debugName);
        return result;
    }
    int ret = (hsDTVAccess->*fun)(result);
    if (ret < 0) {
        BLOGE("Get %s failed! ret=%d", debugName, ret);
        return result;
    }
    BLOGV("The %s is %s", debugName, result.c_str());
    return result;
}

std::string Configuration::getTVDeviceNameForYoutube() const
{

    return "Hisense-Device";
}

std::string Configuration::getTVCapibalitiesForHbbTV() const
{

    return "+DRM";
}

std::string Configuration::getTVFamilyName() const
{

    return "SmartTV_DB2017";
}

std::string Configuration::getTVSoftwareVersion() const
{
    TRACE_HERE();
    std::string result = getStringValueDTVACCESS(
                                                 &lo::dtvaccess::HSDTVAccess::getSoftwareVersion,
                                                 "TVSoftwareVersion");

    if (result.empty()) {
        BLOGE("Unknown Software Version");
        result = "V0000";
    }
    return result;
}

std::string Configuration::getTVFirmwareVersion() const
{
    TRACE_HERE();
    std::string result = getStringValueDTVACCESS(
                                                 &lo::dtvaccess::HSDTVAccess::getFirmwareVersion,
                                                 "TVFirmwareVersion");

    if (result.empty()) {
        BLOGE("Unknown Firmware Version");
        result = "H0000";
    }
    return result;


}

std::string Configuration::getTVModelName() const
{
    TRACE_HERE();
    std::string result = getStringValueDTVACCESS(
                                                 &lo::dtvaccess::HSDTVAccess::getModelName,
                                                 "TVModelName");

    if (result.empty()) {
        BLOGE("Unknown ModelName.");
        result = "HE000000";
    }
    return result;
}

std::string Configuration::getTVChipName() const
{
    TRACE_HERE();
    std::string result = getStringValueDTVACCESS(
                                                 &lo::dtvaccess::HSDTVAccess::getChipType,
                                                 "TVChipName");

    if (result.empty()) {
        BLOGE("Unknown TVChipName.");
        result = "MST0000";
    }
    return result;

}

std::string Configuration::getTVBrandName() const
{
    TRACE_HERE();
    std::string result = getStringValueDTVACCESS(
                                                 &lo::dtvaccess::HSDTVAccess::getTvBrand,
                                                 "TVBrandName");

    if (result.empty()) {
        BLOGE("Unknown TVBrandName.");
        result = "Hisense";
    }
    return result;
}

std::string Configuration::getTVVendorName() const
{

    return "Hisense";
}

short Configuration::getApplicationProfileForHbbTV() const
{
    return 0x0000; //Basic Profile
}

std::string Configuration::getTVHardwareVersion() const
{
    TRACE_HERE();

    return getTVModelName();
}

std::string Configuration::getTVConnectionType() const
{
    TRACE_HERE();

    std::string result = getStringValueDTVACCESS(
                                                 &lo::dtvaccess::HSDTVAccess::getNetworkType,
                                                 "ConnectionType");

    if (result.empty()) {
        BLOGE("Unknown ConnectionType.");
        result = "No-net";
    }
    return result;

}

std::string Configuration::getTVCountryID() const
{

    TRACE_HERE();
    std::string result = getStringValueDTVACCESS(
                                                 &lo::dtvaccess::HSDTVAccess::getCountryId,
                                                 "CountryID");

    if (result.empty()) {
        BLOGE("Unknown TVCountryID.");
        result = "UK";
    }
    return result;

}

BROWSER_LANG_ID Configuration::getTVLanguageID() const
{
    TRACE_HERE();
    //Get the language name obtained from biz which contains three characters
    std::string result = getStringValueDTVACCESS(
                                                 &lo::dtvaccess::HSDTVAccess::getPreferredUILanguage,
                                                 "PreferredUILanguage");

    BLOGI("TV Language: %s", result.c_str());
    //transform the above languagename to id through Language-MAP
    return BrowserLanguage::Name3Toid(result);

}

int Configuration::setTVToFullScreen()
{
    lo::dtvaccess::HSDTVAccess *hsDTVAccess = lo::dtvaccess::HSDTVAccess::getInstance();
    if (hsDTVAccess == NULL) {
        BROWSER_LOG(WARNING, "Get HSDTVAcces  failed!");
        return -1;
    }
    int screen_width = lo::dtvaccess::HSDTVAccess::SCREEN_RES_X;
    int screen_height = lo::dtvaccess::HSDTVAccess::SCREEN_RES_Y;

    int res = hsDTVAccess->setDTVWindowSize(0, 0, screen_width, screen_height);
    if (res < 0) {
        BROWSER_LOG(WARNING, "HSDTVAccess set videowindow failed!");
        return -1;
    }
    return 0;
}

int Configuration::getParentalRatingvalue()
{
    int value;
    std::string region, name, scheme;
    lo::dtvaccess::HSDTVAccess *hsDTVAccess = lo::dtvaccess::HSDTVAccess::getInstance();
    if (hsDTVAccess == NULL) {
        BROWSER_LOG(WARNING, "Get HSDTVAcces  failed!");
        return -1;
    }
    int res = hsDTVAccess->getParentalRatingData(name, region, scheme, value);
    if (res < 0) {
        BROWSER_LOG(WARNING, "HSDTVAccess get ParentalRatingvalue failed!");
        return -1;
    }
    BROWSER_LOG(INFO, "ParentalRatingvalue is:%d", value);
    return value;
}

static std::string getFirstLineOfStrContents(const char * filename)
{
    std::string content;
    std::ifstream file(filename);
    if (file) {
        std::getline(file, content);
    }
    file.close();
    return content;
}

std::string Configuration::getUAForApp(AppType type)
{
    std::string ua_str = "";
    BLOGV("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    std::string device_name = getTVDeviceNameForYoutube();
    std::string firmwareVersion = getTVSoftwareVersion();
    std::string softwareVersion = firmwareVersion;
    std::string brandName = getTVBrandName();
    std::string modelName = getTVModelName();
    std::string connectionType = getTVConnectionType();
    std::string vendorName = getTVVendorName();
    std::string cap = getTVCapibalitiesForHbbTV();
    std::string hardwareVersion = getTVHardwareVersion();
    std::string familyName = getTVFamilyName();
    std::string chipName = getTVChipName();

    BLOGV("device_name:[%s]\n"
          "firmwareVersion:[%s]\n"
          "softwareVersion:[%s]\n"
          "brandName:[%s]\n"
          "modelName:[%s]\n"
          "connectionType:[%s]\n"
          "vendorName:[%s]\n"
          "cap:[%s]\n"
          "hardwareVersion:[%s]\n"
          "familyName:[%s]\n"
          "chipName: [%s]\n",
          device_name.c_str(),
          firmwareVersion.c_str(),
          softwareVersion.c_str(),
          brandName.c_str(),
          modelName.c_str(),
          connectionType.c_str(),
          vendorName.c_str(),
          cap.c_str(),
          hardwareVersion.c_str(),
          familyName.c_str(),
          chipName.c_str());

    BLOGV("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");


    //TODO: Need to redesign a way to get these values.
    switch (type)
    {
    case FOXXUM:
    {
        const char * filename = "/tmp/foxxum_ua.ini";
        ua_str = getFirstLineOfStrContents(filename);
        if (ua_str.size() != 0) {
            std::stringstream builder;
            builder << " VK_PATCH/1.0 CE-HTML/1.0 HBBTV/2.0 MST6586 (;Hisense;SmartTV;" << getTVFirmwareVersion() << ";" << getTVModelName() << ")";
            builder << ua_str;
            ua_str = builder.str();
        } else {
            std::stringstream builder;
            builder << " VK_PATCH/1.0 CE-HTML/1.0 HBBTV/2.0 MST6586";
            ua_str = builder.str();
        }
        break;
    }
    case NETRANGE:
    {
        std::stringstream builder;
        builder << " (Hisense;" << getTVModelName() << ";" << getTVFirmwareVersion() << ")";
        builder << " CE-HTML/1.0 NETRANGEMMH HbbTV/1.2.1 MST6586";
        ua_str = builder.str();
        break;
    }
    case OPERA:
    case ACCEDO:
    case HISENSE:
    case HBBTV:
    case OIPF:
    case HIBROWSER:
    case YOUTUBE:
    {
        std::stringstream builder;
        builder << " _TV_" << getTVChipName() << "/" << getTVFirmwareVersion() << " (";
        builder << getTVBrandName() << "," << getTVModelName() << "," << getTVConnectionType() << ")";
        ua_str = builder.str();
        break;
    }
    case OTHER:
    default:
    {
        std::stringstream builder;
        builder << " (Hisense;" << getTVModelName() << ";" << getTVFirmwareVersion() << ")";
        builder << " MST6586";
        ua_str = builder.str();
        break;
    }

    }
    return ua_str;

}

void Configuration::hbbtvOSDStatusChange()
{
    lo::dtvaccess::HSDTVAccess *hsDTVAccess = lo::dtvaccess::HSDTVAccess::getInstance();
    if (hsDTVAccess == NULL) {
        BROWSER_LOG(ERROR, "Get hsDTVAccess failed!");
        return;
    }
    hsDTVAccess->updateHbbTVOSDSTatus();
}

bool Configuration::getTTSStatus()
{
    static bool inited = false;
    TTSAPI_RET ret = E_TTSAPI_RET_OK;

    if (!inited) {
        ret = TTSAPI_Init((char*) TTS_CUSTOMER_INI);
        /*
        if(E_TTSAPI_RET_OK != ret)
        {
            return false;
        }
         */
        inited = true;
    }
    bool enable = false;
    TTSAPI_GetGlobalConfig(EN_TTSAPI_GCONF_ENABLE, (void*) &enable, sizeof (enable));
    return enable;
}

bool Configuration::getTTSEnable()
{
    bool enable = false;

    lo::dtvaccess::HSDTVAccess * dtv_access = lo::dtvaccess::HSDTVAccess::getInstance();
    if (dtv_access && dtv_access->getTTSEnable()) {
        enable = true;
    }
    return enable;
}
