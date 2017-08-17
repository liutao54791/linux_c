#include <getopt.h>

#include "browser-util.h"
#include "browser_debug.h"

#define MAC2STR(X) (char*)#X
char* appNameSTR[END] = {
    MAC2STR(FOXXUM),
    MAC2STR(NETRANGE),
    MAC2STR(OPERA),
    MAC2STR(ACCEDO),
    MAC2STR(HISENSE),
    MAC2STR(HBBTV),
    MAC2STR(OIPF),
    MAC2STR(HIBROWSER),
    MAC2STR(OTHER)
};

AppType BrowserUtil::m_storeType = NETRANGE;

//static
std::vector<std::string> BrowserUtil::parse2vector(std::string argument) {
    const std::string splitFlag = ",";
    std::vector<std::string> argArray;
    argArray.push_back( "./bin/hisense_browser" );
    if ( argument.empty() ) {
        BROWSER_LOG(ERROR, "The argument is NULL.");
        argArray.push_back( "" );
    } else {
        // If the argument has "[", the ''[" and "]" must come in pairs in the argument from AppManager
        if ( argument.find( "[" ) != std::string::npos )
            argument = argument.substr( 1, argument.size() - 2 );
        else
            argArray.push_back( "-u" );


        int index = std::string::npos, last_position = 0;
        while( (index = argument.find( splitFlag, last_position ) ) != std::string::npos ) {
            if (index == last_position)
                argArray.push_back( "" );
            else
                argArray.push_back( argument.substr( last_position, index - last_position ) );

            //"1" for size of ","
            last_position = index + 1;
         }

        std::string last_string = argument.substr( last_position );
        argArray.push_back( last_string.empty()? "" : last_string );
    }

    return argArray;
}


//static
void BrowserUtil::parseArguments(std::vector<std::string> argument, AppType& type, std::string& url, BrowserModeType& mode) {
    int argc = argument.size();
    char* argv[argc + 1];

    for ( int i = 0; i < argc; i++ ) {
        char* data = (char*)argument.at( i ).c_str();
        argv[i] = ( char* ) malloc( strlen( data ) + 1 );
        if ( !argv[i] )
            return;
        memset( argv[i], '\0', strlen( data ) + 1 );
        memcpy( argv[i], data, strlen( data ));
    }
    argv[argc] = NULL;

    for ( int i = 0; i < argc + 1; i++ ) {
        BROWSER_LOG( ALWAYS, "argv[%d] : %s", i, argv[i] );
    }

    const char *short_options = "u:s:";
    const struct option long_options[] =
    {
        {"url",       required_argument, NULL, 'u'},
        {"store_type",required_argument, NULL, 's'},
        {NULL,        0,                 NULL,  0 }
    };

    int option_index = 0;
    int current_option = 0;
    optind = 1;
    while (current_option != -1) {
        current_option = getopt_long(argc, argv, short_options, long_options, &option_index);
        switch (current_option) {
            case 's':
                if ( optarg ) {
                    BROWSER_LOG(INFO, "RUN_TYPE : %s", optarg);
                    if ( !strcasecmp( optarg, appNameSTR[FOXXUM] ) )
                    {
                        type = FOXXUM;
                        mode = BROWSER_MODE_STORE;
                    }
                    else if ( !strcasecmp( optarg, appNameSTR[NETRANGE] ) )
                    {
                        type = NETRANGE;
                        mode = BROWSER_MODE_STORE;
                    }
                    else if ( !strcasecmp( optarg, appNameSTR[HISENSE] ) )
                    {
                        type = HISENSE;
                        mode = BROWSER_MODE_STORE;
                    }
                    else if ( !strcasecmp( optarg, appNameSTR[ACCEDO] ) )
                    {
                        type = ACCEDO;
                        mode = BROWSER_MODE_STORE;
                    }
                    else if ( !strcasecmp( optarg, appNameSTR[OPERA] ) )
                    {
                        type = OPERA;
                        mode = BROWSER_MODE_OPERA_STORE;
                    }
                    else if ( !strcasecmp( optarg, appNameSTR[HBBTV] ) )
                    {
                        type = HBBTV;
                        mode = BROWSER_MODE_HBBTV;
                    }
                    else if ( !strcasecmp( optarg, appNameSTR[OIPF]) )
                    {
                        type = OIPF;
                        mode = BROWSER_MODE_OIPF;
                    }
                    else if ( !strcasecmp( optarg, appNameSTR[HIBROWSER]) )
                    {
                        type = HIBROWSER;
                        mode = BROWSER_MODE_HIBROWSER;
                    }
                    else
                    {
                        type = OTHER;
                        mode = BROWSER_MODE_STORE;
                    }
                    m_storeType = type;
                }
                break;
            case 'u':
                if ( optarg ) {
                    BROWSER_LOG( ALWAYS, "url : %s", optarg );
                    url = optarg;
                }
                break;
            default:
                break;
        }
    }
}
