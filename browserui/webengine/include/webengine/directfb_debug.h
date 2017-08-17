#ifndef BROWSER_DIRECTFB_DEBUG
#define BROWSER_DIRECTFB_DEBUG

#include <directfb.h>
#include "base/log/browser_log.h"





#define DFBCHECK(x...) \
do { \
    DFBResult err = (DFBResult)(x); \
    if (err != (DFBResult)DFB_OK) \
    { \
        fprintf( stderr, LIGHT_RED "[%s:%s L:%u] DFB Error" COLOR_NONE, _SHORT_FILE_NAME_, __FUNCTION__, __LINE__ ); \
        DirectFBErrorFatal( #x, err ); \
    } \
} while (0)

#endif
