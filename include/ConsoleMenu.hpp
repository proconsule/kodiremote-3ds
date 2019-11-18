#ifndef CONSOLEMENU_H
#define CONSOLEMENU_H

#include <stdio.h>
#include <vector>
#include <cstring>
#include <string>
#include "Parameters.hpp"
#include "KodiRPC.hpp"

using namespace std;

#define KODICONFFILE "kodiremote-3ds.conf"

typedef struct{
    int menupos;
    int currmenuid;
    int prevmenuid;
    int currmenumax;
    bool havesubmenu;

}menu_struct;

typedef struct{
	string menutitle;
	int menuid;
	std::vector<bool> havesubmenu;
	std::vector<int> submenuid;
	std::vector<string> menudesc;


}menupage_struct;

#define VMENUPOS 2
#define HMENUPOS 3

enum MID {
	MAINMENU = 0,
    LIBRARYMENU = 20,
    REMOTEMENU = 10,
    SETTINGSMENU = 30,
    VIDEOLIBRARYMENU = 220,
    AUDIOLIBRARYMENU = 120,
    AUDIOLIBRARYLISTMENU = 1120,
    AUDIOLIBRARYALBUMMENU = 11120,
    AUDIOLIBRARYSONGMENU = 111120,
    TVSHOWLIBRARYMENU = 320,
    VIDEOLIBRARYLISTMENU = 1220,
    TVSHOWLIBRARYLISTMENU = 1320,
    TVSHOWLIBRARYSEASONMENU = 11320,
    TVSHOWLIBRARYEPISODEMENU = 111320




};


class CConsoleMenu{
	public:

	void DebugTop(C3D_RenderTarget* target);

	void Init();
	char * PrintMenu(int menuid);
    char * PrintVideoLibrary();

	int * PrevMenuID(int menuid);
	int MenuItems(int menuid);
	bool HaveSubmenu(int menuid,int pos);
	int GetSubmenuID(int menuid,int pos);

	std::vector<menupage_struct> Menu;

	int menupos;
	int currmenuid;
	int prevmenuid;
	int currmenumax;
	bool havesubmenu;

	int movielistpos;
	int tvshowlistpos;
	int tvshowseasonpos;
	int tvshowepisodepos;

	int audioartistpos;
	int audioalbumpos;
	int audiosongpos;

	CParameters *Parameters;
    CKodiRPC *KodiRPC;




    C2D_TextBuf menudynbuffer;
    C2D_TextBuf menudynbuffer2;
    C2D_TextBuf menudynbuffer3;

    C2D_TextBuf topmenudynbuffer;
    C2D_TextBuf topmenudynbuffer2;


    void startMenu(C3D_RenderTarget* target,C3D_RenderTarget* top,CConsoleMenu *Menu);


    void GUIInit();


    void GUIStartMenu(C3D_RenderTarget* target,menu_struct *menu);
    void GUIMenu(C3D_RenderTarget* target,int menuid);

    void StatusMsg(C3D_RenderTarget* target,std::string line1,std::string line2,std::string line3);

    void GUIMenuAudioLibrary(C3D_RenderTarget* target);
    void GUIMenuAudioAlbum(C3D_RenderTarget* target);
    void GUIMenuAudioSong(C3D_RenderTarget* target);

    void GUIMenuVideoLibrary(C3D_RenderTarget* target);
    void VideoLibRender(C3D_RenderTarget* target,int videoid);

    void GUIMenuTVShowLibrary(C3D_RenderTarget* target);
    void GUIMenuTVShowSeason(C3D_RenderTarget* target);
    void GUIMenuTVShowEpisode(C3D_RenderTarget* target);
    void TVShowLibRender(C3D_RenderTarget* target,int showid);
    void TVShowEpisodeRender(C3D_RenderTarget* target,int episodenum);



    void GUIVideoRefresh(C3D_RenderTarget* target,std::string label,std::string debugmsg,int current,int max);


    void SettingsMenu(C3D_RenderTarget* target);
};

#endif
