/*
 * File:   KodiRPC.hpp
 * Author: proconsule
 *
 * Created on 21 ottobre 2019, 10.27
 */

#ifndef KODIRPC_HPP
#define KODIRPC_HPP

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <curl/curl.h>

#include <turbojpeg.h>

#include <jpeglib.h>

#include <citro2d.h>

#include <3ds.h>

#include "Library.hpp"

typedef void  (*THREADFUNCPTR)(void *);
#define STACKSIZE (10 * 1024)


enum MSGIDS{
    INPUTKEY = 1000,
    VOLUME = 101,
    VERSION = 102,
    PLAYMOVIE = 201,
    PLAYEPISODE = 202,
    PLAYSONG = 203,
    PLAYER = 302,
    QUERYPLAYER = 303,
    QUERYVIDEOPLAYERSTATUS = 304,
    QUERYVIDEOPLAYERTIME = 305,
    QUERYAUDIOPLAYERSTATUS = 306,
    QUERYAUDIOPLAYERTIME = 307



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

typedef struct{
    int hour;
    int minutes;
    int seconds;
    int millisec;

}playtime_struct;

typedef struct{
    char *showtitle;
    char *label;
    char *type;
    int tvshowid;
    int season;
    int episode;
    int id;
    playtime_struct currenttime;
    playtime_struct totaltime;
    double perc;


}videostatus_struct;

typedef struct{
    char *title;
    char *album;
    char *artist;
    char *type;
    int year;
    int track;
    playtime_struct currenttime;
    playtime_struct totaltime;
    double perc;

}audiostatus_struct;


class CKodiRPC{
public:
    void Init();

    Thread thread;
    void *PlayerStatusThread(void *arg);
    bool *runThreads;
    void InitThread(s32 prio);
    bool SocketFree;
    int *currmenuid;
    int playerid;

    videostatus_struct player_viseostatus;
    audiostatus_struct player_audiostatus;



    void CreateMessageInputKey(InputKeys Key);


    int volume;
    kodiversion_struct kodiversion;

    int currvideolibid;

    bool done;
    void ParseJson(char *buffer);
    //bool ParseGetMovies(char *buffer);



    void PlayerPlayPayse();
    void PlayerStop();
    void PlayerSeekFF();
    void PlayerSeekRev();


    std::string wrap(const char *text, size_t line_length);

    int *kodisock;

    char * kodiaddress;
    int * kodihttpport;


    CAudioLib *AudioLib;
    CMovieLib *MovieLib;
    CTVShowLib *TVShowLib;


};


#endif /* KODIRPC_HPP */

