#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <memory>

#include <opera/components/command_line.h>
#include "browser_component.h"
#include "browser_type.h"
#include "json/json.h"
#include "language.h"

class Configuration : public BrowserComponent
{
public:
    Configuration(BrowserImpl* browser, int argc, char** argv);
    ~Configuration();

    BrowserStatus start();
    BrowserStatus stop();

    opera::components::CommandLine& getCommandLine();

    std::string getValueByKey(std::string key);

    std::string getTVDeviceNameForYoutube() const;
    std::string getTVFirmwareVersion() const;
    std::string getTVSoftwareVersion() const;
    std::string getTVBrandName() const;
    std::string getTVModelName() const;
    std::string getTVConnectionType() const;
    std::string getTVCapibalitiesForHbbTV() const;
    std::string getTVVendorName() const;
    std::string getTVHardwareVersion() const;
    std::string getTVFamilyName() const;
    std::string getTVChipName() const;
    std::string getTVCountryID() const;
    short getApplicationProfileForHbbTV() const;
    BROWSER_LANG_ID getTVLanguageID() const;
public:



    std::string getUAForApp(AppType);

    bool getTTSStatus();
    bool getTTSEnable();

    int setTVToFullScreen();
    int getParentalRatingvalue();
    void hbbtvOSDStatusChange();
private:
    void readJsonFile();

    Json::Value m_root;

    std::shared_ptr<opera::components::CommandLine> m_commandLine;

    std::string m_configFile;
};


#endif
