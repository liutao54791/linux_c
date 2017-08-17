
#ifndef __BROWSER_HISENSE_WEBENGINE_H__
#define __BROWSER_HISENSE_WEBENGINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define  WE_MODE_OFF   0x00
#define  WE_MODE_STORE 0x01
#define  WE_MODE_OPERA_STORE 0x02
#define  WE_MODE_HBBTV 0x03
#define  WE_MODE_BROWSER 0x04
#define  WE_MODE_OIPF 0x05
#define  WE_MODE_YOUTUBE 0x06
#define  WE_MODE_COUNT  0x07

#define KEY_FLAG_DOWN 0x00
#define KEY_FLAG_UP   0x01
#define KEY_FLAG_REPEAT 0x02
#define KEY_FLAG_CTRL 0x10
#define KEY_FLAG_ALT  0x20
#define KEY_FLAG_SHIFT 0x40

    //Used to notify messages to the client from webengine
typedef enum WeEventType
{
    KeySetChanged = 0x1011,
    APPStarted = 0x1012,
    APPStopped = 0x1013,
    APPMessage = 0x1014
} WeEventType;

//FIXME:We are going to need better way to passing parameters,
//Right now we are using meaningless parameters.
typedef struct
{
    WeEventType type;

    union
    {
        int asInt;
        unsigned int asUint;
        void * asPointer;
    } param;
   union
    {
        int asInt;
        unsigned int asUint;
        void * asPointer;

    } param1;
} WeEvent;



typedef int EventListenerFunc(void * context, const WeEvent & event);

 int hisense_webengine_start(int argc, char* argv[],EventListenerFunc cb, void * ctx);
 int hisense_webengine_load_url(int we_mode, const char * url, std::string type="");
 int hisense_webengine_inject_keyevent(int we_mode, unsigned int type,unsigned symbol,unsigned int id,unsigned int modifier);
 int hisense_webengine_send_message(int we_mode,const char* src, const char * message);
 int hisense_webengine_cancel_pending_request();
#ifdef __cplusplus
}
#endif

#endif

