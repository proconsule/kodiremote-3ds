#ifndef LIBRARY_HPP
#define LIBRARY_HPP


#include <stdio.h>
#include <vector>
#include <algorithm>
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
    std::string fanarturl;
    std::string plot;
    MemoryStruct jpegref;
    MemoryStruct fanartjpegref;


}kodivideolib_struct;

typedef struct{
    std::string label;
    std::string plot;
    int episodeid;
    int episode;

}tvepisode_struct;

typedef struct{
    int seasonid;
    int season;
    std::vector<tvepisode_struct> episodes;

}tvseason_struct;


typedef struct{
    std::string label;
    int showid;
    int year;
    std::string thumburl;
    std::string fanarturl;
    std::string plot;
    MemoryStruct jpegref;
    MemoryStruct fanartjpegref;
    std::vector<tvseason_struct> seasons;


}koditvshowlib_struct;



typedef struct{
    C2D_Image thumbtext;
    int width;
    int height;

}thumbimage_struct;


typedef struct{
    std::string title;
    int duration;
    int songid;
    int track;



}audiosong_struct;

typedef struct{
    std::string title;
    int year;
    int albumid;
    std::vector<audiosong_struct> songs;


}audioalbum_struct;


typedef struct{
    std::string name;
    std::vector<audioalbum_struct> albums;
    int artistid;


}audioartist_struct;

typedef struct{
    std::string artistname;
    int artistid;
    std::string albumtitle;
    int year;
    int albumid;
    std::string songtitle;
    int duration;
    int songid;
    int track;


}songparse_struct;



class CMovieLib{
public:
    void Init();
    void DownloadThumb(int movienum);
    void DownloadFanArt(int movienum);
    void CreateThumbTexture(int movienum);
    void CreateFanArtTexture(int movienum);
    bool Draw_LoadImageMemory(C2D_Image *texture, ImageMemoryStruct *source);
    //void TurboJpegDecompress(MemoryStruct *source,ImageMemoryStruct *dest);
    std::vector<kodivideolib_struct> kodivideolib;
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    thumbimage_struct thumbimage;
    thumbimage_struct fanartimage;
    void PlayMovie(int movieid);
    bool ParseGetMovies(char *buffer);
    void RequestMovieList();


    int *kodisock;
    char * kodiaddress;
    int * kodihttpport;

};

class CTVShowLib{
public:
    void Init();
    void DownloadThumb(int shownum);
    void DownloadFanArt(int shownum);

    void CreateThumbTexture(int shownum);
    void CreateFanArtTexture(int shownum);
    bool Draw_LoadImageMemory(C2D_Image *texture, ImageMemoryStruct *source);
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

    int SearchTVShowID(int tvshowid);
    int SearchTVSeasonID(int seasonid,std::vector<tvseason_struct> *seasons);
    bool ParseGetTVShow(char *buffer);
    bool ParseGetTVShowEpisodes(char *buffer);
    void RequestTVShowList();
    void RequestTVShowEpisodesList(int tvshowid);
    void PlayEpisode(int episodeid);
    void PlaySeason(int seasonid);
    std::vector<koditvshowlib_struct> koditvshowlib;

    int *kodisock;
    char * kodiaddress;
    int * kodihttpport;
    thumbimage_struct thumbimage;
    thumbimage_struct fanartimage;

};




class CAudioLib{
public:
    void Init();
    void RequestAudioList();
    bool ParseAudio(char *buffer);

    void PlaySong(int songid);

    std::vector<audioartist_struct> artistslib;

    int SearchArtistID(int artistid);
    int SearchAlbumID(int albumid,std::vector<audioalbum_struct> *albums);


    int *kodisock;
    char * kodiaddress;
    int * kodihttpport;


};



#endif
