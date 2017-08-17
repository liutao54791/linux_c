#ifndef BROWSER_TYPE_H
#define BROWSER_TYPE_H
#include <string>
enum BrowserStatus
{
    BrowserOK,
    BrowserError,
};

enum BrowserModeType {
    BROWSER_MODE_TYPE_MIN,
    BROWSER_MODE_OFF = BROWSER_MODE_TYPE_MIN,
    BROWSER_MODE_HBBTV,
    BROWSER_MODE_STORE,
    BROWSER_MODE_OPERA_STORE,
    BROWSER_MODE_HIBROWSER,
    BROWSER_MODE_OIPF,
    BROWSER_MODE_YOUTUBE,
    BROWSER_MODE_TYPE_MAX = BROWSER_MODE_YOUTUBE
};

static inline std::string ToString(BrowserModeType mode)
{
    switch (mode) {

        case BROWSER_MODE_OFF: return "BROWSER_MODE_OFF";
        case BROWSER_MODE_HBBTV: return "BROWSER_MODE_HBBTV";
        case BROWSER_MODE_STORE: return "BROWSER_MODE_STORE";
        case BROWSER_MODE_OPERA_STORE: return "BROWSER_MODE_OPERA_STORE";
        case BROWSER_MODE_HIBROWSER: return "BROWSER_MODE_HIBROWSER";
        case BROWSER_MODE_OIPF: return "BROWSER_MODE_OIPF";
        case BROWSER_MODE_YOUTUBE: return "BROWSER_MODE_YOUTUBE";
        default:
            return "BROWSER_MODE_UNRECOGNIZED";
    }
}
enum BrowserStoreWindowType {
    OPERA_TV_STORE_WINDOW,
    OIPF_APP_STORE_WINDOW,
    OTHER_STORE_WINDOW
};

enum AppType {
    FOXXUM,
    NETRANGE,
    OPERA,
    ACCEDO,
    HISENSE,
    HBBTV,
    OIPF,
    HIBROWSER,
    YOUTUBE,
    OTHER,
    END
};

union MultiModeArgument{
    AppType type;
    //Other argument for other mode?
};

#define HD "1280x720"
#define FHD "1920x1080"
#define ZERO_RESOLUTION "0x0"

#endif
