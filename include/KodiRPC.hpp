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


enum MSGIDS{
    INPUTKEY = 1000,
    VOLUME = 101,
    VERSION = 102,
    PLAYMOVIE = 201,
    PLAYER = 202



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

struct MemoryStruct {
  char *memory;
  size_t size;
};

struct ImageMemoryStruct {
  char *memory;
  size_t size;
  int width;
  int height;
};

typedef struct{
    std::string label;
    int movieid;
    int year;
    std::string thumburl;
    std::string plot;
    MemoryStruct jpegref;


}kodivideolib_struct;

typedef struct{
    C2D_Image thumbtext;
    int width;
    int height;

}thumbimage_struct;



class CKodiRPC{
public:
    void Init();


    void CreateMessageInputKey(InputKeys Key);


    int volume;
    kodiversion_struct kodiversion;

    int currvideolibid;

    bool done;
    void ParseJson(char *buffer);
    bool ParseGetMovies(char *buffer);
    void RequestMovieList();
    void PlayMovie(int movieid);

    void PlayerPlayPayse();
    void PlayerStop();
    void PlayerSeekFF();
    void PlayerSeekRev();


    std::vector<kodivideolib_struct> kodivideolib;

    void DownloadMovieThumb(int movienum);
    void CreateThumbTexture(int movienum);
    void JpegDecompress(MemoryStruct *source,ImageMemoryStruct *dest);

    bool Draw_LoadImageMemory(C2D_Image *texture, ImageMemoryStruct *source);
    void TurboJpegDecompress(MemoryStruct *source,ImageMemoryStruct *dest);

    //C2D_Image thumbtext;
    thumbimage_struct thumbimage;

    std::string wrap(const char *text, size_t line_length);

    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

    int *kodisock;

    char * kodiaddress;
    int * kodihttpport;


};


#endif /* KODIRPC_HPP */

