/*
 * File:   KodiRPC.hpp
 * Author: proconsule
 *
 * Created on 21 ottobre 2019, 10.27
 */

#ifndef KODIRPC_HPP
#define KODIRPC_HPP

#include <stdio.h>
#include <json-c/json.h>


enum MSGIDS{
    INPUTKEY = 1000,
    VOLUME = 101,
    VERSION = 102



};

enum InputKeys{
    KODI_KEYUP = 1,
    KODI_KEYDOWN = 2,
    KODI_KEYLEFT = 3,
    KODI_KEYRIGHT = 4,
    KODI_KEYSELECT = 5,
    KODI_KEYINFO = 6,
    KODI_KEYCONTEXT = 7,
    KODI_KEYBACK = 8,
    KODI_KEYHOME = 9,
    KODI_KEYOSD = 10,
    KODI_KEYSHOWCODEC = 11

};


typedef struct{
    int major;
    int minor;
    char *revision;
    char *tag;

}kodiversion_struct;


class CKodiRPC{
public:
    void Init();

    //json_object * CreateMessageId(int id);
    char * CreateMessageInputKey(InputKeys Key);


    int volume;
    kodiversion_struct kodiversion;

    void ParseJson(char *buffer);



};


#endif /* KODIRPC_HPP */

