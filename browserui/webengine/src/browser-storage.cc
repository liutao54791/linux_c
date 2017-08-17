#include "browser-storage.h"
#include "browser_debug.h"

BrowserStorage::BrowserStorage( BrowserImpl* browserImpl )
    : BrowserComponent( browserImpl )
{
    BROWSER_LOG( INFO, "" );
}

BrowserStorage::~BrowserStorage()
{
    BROWSER_LOG( INFO, "" );
}

BrowserStatus BrowserStorage::start()
{
    BROWSER_LOG( INFO, "" );
    return BrowserOK;
}

BrowserStatus BrowserStorage::stop()
{
    BROWSER_LOG( INFO, "" );
    return BrowserOK;
}

