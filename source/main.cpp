//
//  main.cpp
//  kodiclient-3ds
//
//  Created by proconsule on 20/10/2019.
//  Copyright © 2019 proconsule. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <3ds.h>

#include <citro2d.h>

#include "Parameters.hpp"
#include "KodiSocket.hpp"
#include "ConsoleMenu.hpp"
#include "GFX.hpp"

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000

static u32 *SOC_buffer = NULL;

void failExit(const char *fmt, ...);

int onetime = 0;
u32 *imgsize;
u8 *imgbuf;
Result myret;


CParameters Parameters;




void socShutdown() {

    socExit();

}







int main(){

    romfsInit();
    gfxInitDefault();

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_RIGHT);





    Parameters.Init();
    Parameters.Read_Config(KODICONFFILE);

    CConsoleMenu Menu;
    Menu.Parameters = &Parameters;
    Menu.Init();
    Menu.GUIInit();




    CGFX *GFX = new CGFX();
    GFX->Init();






    Menu.startMenu(bottom,top,&Menu);

    CKodiSocket KodiSocket;


    KodiSocket.Parameters = &Parameters;

    SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

    int ret=0;

    if(SOC_buffer == NULL) {
        failExit("memalign: failed to allocate\n");
    }

    // Now intialise soc:u service
    if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0) {
        failExit("socInit: 0x%08X\n", (unsigned int)ret);
    }else
    {
        printf("Socket INIT ok\n");
    }


    s32 prio = 0;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);


    Parameters.KodiRPC->kodisock = &KodiSocket.sock;
    Parameters.AudioLib->kodisock = &KodiSocket.sock;
    Parameters.MovieLib->kodisock = &KodiSocket.sock;
    Parameters.TVShowLib->kodisock = &KodiSocket.sock;
    Parameters.KodiRPC->Init();

    Parameters.KodiRPC->currmenuid = &Menu.currmenuid;


    KodiSocket.Init(prio);

    Parameters.KodiRPC->InitThread(prio);


    while (aptMainLoop())
    {




        hidScanInput();
        u32 kDown = hidKeysDown();

        touchPosition touch;
        hidTouchRead(&touch);





        if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
            Menu.GUIMenuVideoLibrary(bottom);
        }
        else if(Menu.currmenuid == TVSHOWLIBRARYEPISODEMENU){
            Menu.GUIMenuTVShowEpisode(bottom);
        }
        else if(Menu.currmenuid == TVSHOWLIBRARYSEASONMENU){
            Menu.GUIMenuTVShowSeason(bottom);
        }
        else if(Menu.currmenuid == TVSHOWLIBRARYLISTMENU){
            Menu.GUIMenuTVShowLibrary(bottom);
        }
        else if(Menu.currmenuid == AUDIOLIBRARYLISTMENU){
            Menu.GUIMenuAudioLibrary(bottom);
        }
        else if(Menu.currmenuid == AUDIOLIBRARYALBUMMENU){
            Menu.GUIMenuAudioAlbum(bottom);
        }
        else if(Menu.currmenuid == AUDIOLIBRARYSONGMENU){
            Menu.GUIMenuAudioSong(bottom);
        }

        else if(Menu.currmenuid == REMOTEMENU){

            Menu.DebugTop(top);

            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            C2D_TargetClear(bottom, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
            C2D_SceneBegin(bottom);
            //u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
            //u32 clrRed = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);

            for(unsigned int i=0;i<GFX->remotemenuspritevector.size();i++){
                C2D_DrawSprite(&GFX->remotemenuspritevector[i].spr);
            }

            int RemoteMenuTouch = GFX->RemoteMenutouchSprite(&touch);

            if(RemoteMenuTouch >-1){
                //C2D_DrawRectSolid(touch.px,touch.py,0.6f,20.0f,20.0f,clrRed);
                if(RemoteMenuTouch == REMOTEREV){
                    Parameters.KodiRPC->PlayerSeekRev();
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTESTOP){
                    Parameters.KodiRPC->PlayerStop();
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTEPLAY){
                    Parameters.KodiRPC->PlayerPlayPayse();
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTEPAUSE){
                    Parameters.KodiRPC->PlayerPlayPayse();
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTEFF){
                    Parameters.KodiRPC->PlayerSeekFF();
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTEDOWN){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYDOWN);
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTELEFT){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYLEFT);
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTEUP){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYUP);
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTERIGHT){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYRIGHT);
                    usleep(500000);
                }

                if(RemoteMenuTouch == REMOTEOK){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYSELECT);
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTEBACK){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYBACK);
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTEINFO){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYINFO);
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTEHOME){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYHOME);
                    usleep(500000);
                }
                if(RemoteMenuTouch == REMOTECONTEXT){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYCONTEXT);
                    usleep(500000);
                }


            }
            else
            {


                //C2D_DrawRectSolid(touch.px,touch.py,0.6f,20.0f,20.0f,clrWhite);

            }


            C3D_FrameEnd(0);
            //HelpMenu();
        }
        else
        {
            //char *myout = Menu.PrintMenu(Menu.currmenuid);
            Menu.currmenumax = Menu.MenuItems(Menu.currmenuid);
            //printf(myout);
            //free(myout);

            if(Menu.currmenuid == MAINMENU){
                Menu.DebugTop(top);
            }else
            {
                C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            C2D_TargetClear(top, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
            C2D_SceneBegin(top);
            C3D_FrameEnd(0);
            }
            /*

            */

            Menu.GUIMenu(bottom,Menu.currmenuid);
        }


		if (kDown & KEY_SELECT){
            //Parameters.KodiRPC->RequestMovieList();

		}

        if (kDown & KEY_START){

                if(Menu.currmenuid == REMOTEMENU){
                    int *menuget = Menu.PrevMenuID(Menu.currmenuid);
                    Menu.currmenuid = menuget[0];
                    Menu.menupos = menuget[1];
                    free(menuget);

                }

        }

        if (kDown & KEY_LEFT){

                if(Menu.currmenuid == REMOTEMENU){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYLEFT);

                }

        }

        if (kDown & KEY_RIGHT){
                if(Menu.currmenuid == REMOTEMENU){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYRIGHT);

                }
        }

        if (kDown & KEY_UP){
                if(Menu.currmenuid == REMOTEMENU){
                        Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYUP);

                }
                else if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
                        if(Menu.movielistpos <1){
                            Menu.movielistpos = Parameters.MovieLib->kodivideolib.size()-1;
                        }
                        else
                        {
                            Menu.movielistpos--;
                        }
                        if(Parameters.MovieLib->kodivideolib.size()>0){
                            if(Parameters.downloadimages){
                                Parameters.MovieLib->CreateThumbTexture(Menu.movielistpos);
                                Parameters.MovieLib->CreateFanArtTexture(Menu.movielistpos);
                            }
                        }


                }
                else if(Menu.currmenuid == AUDIOLIBRARYLISTMENU){
                        if(Menu.audioartistpos <1){
                            Menu.audioartistpos = Parameters.AudioLib->artistslib.size()-1;
                        }
                        else
                        {
                            Menu.audioartistpos--;
                        }
                        if(Parameters.AudioLib->artistslib.size()>0){

                        }


                }
                else if(Menu.currmenuid == AUDIOLIBRARYALBUMMENU){
                        if(Menu.audioalbumpos <1){
                            Menu.audioalbumpos = Parameters.AudioLib->artistslib[Menu.audioartistpos].albums.size()-1;
                        }
                        else
                        {
                            Menu.audioalbumpos--;
                        }
                        if(Parameters.AudioLib->artistslib[Menu.audioartistpos].albums.size()>0){

                        }


                }
                else if(Menu.currmenuid == AUDIOLIBRARYSONGMENU){
                        if(Menu.audiosongpos <1){
                            Menu.audiosongpos = Parameters.AudioLib->artistslib[Menu.audioartistpos].albums[Menu.audioalbumpos].songs.size()-1;
                        }
                        else
                        {
                            Menu.audiosongpos--;
                        }
                        if(Parameters.AudioLib->artistslib[Menu.audioartistpos].albums[Menu.audioalbumpos].songs.size()>0){

                        }


                }
                else if(Menu.currmenuid == TVSHOWLIBRARYLISTMENU){
                        if(Menu.tvshowlistpos <1){
                            Menu.tvshowlistpos = Parameters.TVShowLib->koditvshowlib.size()-1;
                        }
                        else
                        {
                            Menu.tvshowlistpos--;
                        }
                        if(Parameters.TVShowLib->koditvshowlib.size()>0){
                            if(Parameters.downloadimages){
                            Parameters.TVShowLib->CreateThumbTexture(Menu.tvshowlistpos);
                            Parameters.TVShowLib->CreateFanArtTexture(Menu.tvshowlistpos);
                        }
                        }


                }
                else if(Menu.currmenuid == TVSHOWLIBRARYSEASONMENU){
                        if(Menu.tvshowseasonpos <1){
                            Menu.tvshowseasonpos = Parameters.TVShowLib->koditvshowlib[Menu.tvshowlistpos].seasons.size()-1;
                        }
                        else
                        {
                            Menu.tvshowseasonpos--;
                        }



                }
                else if(Menu.currmenuid == TVSHOWLIBRARYEPISODEMENU){
                        if(Menu.tvshowepisodepos <1){
                            Menu.tvshowepisodepos = Parameters.TVShowLib->koditvshowlib[Menu.tvshowlistpos].seasons[Menu.tvshowseasonpos].episodes.size()-1;
                        }
                        else
                        {
                            Menu.tvshowepisodepos--;
                        }
                        //if(Parameters.TVShowLib->koditvshowlib.size()>0){
                        //    Parameters.TVShowLib->CreateThumbTexture(Menu.tvshowlistpos);
                        //}


                }
                else
                {
                    if(Menu.menupos < 1){
                        Menu.menupos = Menu.currmenumax-1;
                    }else
                    {
                        Menu.menupos--;
                    }
                }

        }

        if (kDown & KEY_DOWN){
                if(Menu.currmenuid == REMOTEMENU){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYDOWN);

                }
                else if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
                        if((unsigned int)Menu.movielistpos >= Parameters.MovieLib->kodivideolib.size()-1){
                            Menu.movielistpos = 0;
                        }
                        else
                        {
                            Menu.movielistpos++;
                        }
                    if(Parameters.MovieLib->kodivideolib.size()>0){
                        if(Parameters.downloadimages){
                            Parameters.MovieLib->CreateThumbTexture(Menu.movielistpos);
                            Parameters.MovieLib->CreateFanArtTexture(Menu.movielistpos);
                        }
                    }


                }
                else if(Menu.currmenuid == AUDIOLIBRARYLISTMENU){
                        if((unsigned int)Menu.audioartistpos >= Parameters.AudioLib->artistslib.size()-1){
                            Menu.audioartistpos = 0;
                        }
                        else
                        {
                            Menu.audioartistpos++;
                        }
                    if(Parameters.AudioLib->artistslib.size()>0){

                    }


                }
                else if(Menu.currmenuid == AUDIOLIBRARYALBUMMENU){
                        if((unsigned int)Menu.audioalbumpos >= Parameters.AudioLib->artistslib[Menu.audioartistpos].albums.size()-1){
                            Menu.audioalbumpos = 0;
                        }
                        else
                        {
                            Menu.audioalbumpos++;
                        }
                    if(Parameters.AudioLib->artistslib[Menu.audioartistpos].albums.size()>0){

                    }


                }
                else if(Menu.currmenuid == AUDIOLIBRARYSONGMENU){
                        if((unsigned int)Menu.audiosongpos >= Parameters.AudioLib->artistslib[Menu.audioartistpos].albums[Menu.audioalbumpos].songs.size()-1){
                            Menu.audiosongpos = 0;
                        }
                        else
                        {
                            Menu.audiosongpos++;
                        }
                    if(Parameters.AudioLib->artistslib[Menu.audioartistpos].albums[Menu.audioalbumpos].songs.size()>0){

                    }


                }
                else if(Menu.currmenuid == TVSHOWLIBRARYLISTMENU){
                        if((unsigned int)Menu.tvshowlistpos >= Parameters.TVShowLib->koditvshowlib.size()-1){
                            Menu.tvshowlistpos = 0;
                        }
                        else
                        {
                            Menu.tvshowlistpos++;
                        }
                    if(Parameters.TVShowLib->koditvshowlib.size()>0){
                        if(Parameters.downloadimages){
                            Parameters.TVShowLib->CreateThumbTexture(Menu.tvshowlistpos);
                            Parameters.TVShowLib->CreateFanArtTexture(Menu.tvshowlistpos);
                        }
                    }


                }
                else if(Menu.currmenuid == TVSHOWLIBRARYSEASONMENU){
                        if((unsigned int)Menu.tvshowseasonpos >= Parameters.TVShowLib->koditvshowlib[Menu.tvshowlistpos].seasons.size()-1){
                            Menu.tvshowseasonpos = 0;
                        }
                        else
                        {
                            Menu.tvshowseasonpos++;
                        }



                }
                else if(Menu.currmenuid == TVSHOWLIBRARYEPISODEMENU){
                        if((unsigned int)Menu.tvshowepisodepos >= Parameters.TVShowLib->koditvshowlib[Menu.tvshowlistpos].seasons[Menu.tvshowseasonpos].episodes.size()-1){
                            Menu.tvshowepisodepos = 0;
                        }
                        else
                        {
                            Menu.tvshowepisodepos++;
                        }



                }
                else{
                   if(Menu.menupos >= Menu.currmenumax-1){
                        Menu.menupos = 0;
                    }else{
                        Menu.menupos++;
                    }

                }

        }

        if (kDown & KEY_X){
            if(Menu.currmenuid == TVSHOWLIBRARYSEASONMENU){
               Parameters.TVShowLib->PlaySeason(Parameters.TVShowLib->koditvshowlib[Menu.tvshowlistpos].seasons[Menu.tvshowseasonpos].seasonid);
            }

        }

        if (kDown & KEY_A){
                if(Menu.currmenuid == MAINMENU && Menu.menupos == Menu.currmenumax-1){
                    break;
                }
                if(Menu.currmenuid == MAINMENU && Menu.menupos == Menu.currmenumax-2){
                    Menu.startMenu(bottom,top,&Menu);
                    continue;

                }


                if(Menu.currmenuid == REMOTEMENU){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYSELECT);

                }

                if(Menu.currmenuid == AUDIOLIBRARYALBUMMENU){
                    Menu.audiosongpos = 0;
                    Menu.currmenuid = AUDIOLIBRARYSONGMENU;
                }

                if(Menu.currmenuid == AUDIOLIBRARYLISTMENU){
                    Menu.audioalbumpos = 0;
                    Menu.currmenuid = AUDIOLIBRARYALBUMMENU;
                }


                if(Menu.currmenuid == AUDIOLIBRARYSONGMENU){
                    Parameters.AudioLib->PlaySong(Parameters.AudioLib->artistslib[Menu.audioartistpos].albums[Menu.audioalbumpos].songs[Menu.audiosongpos].songid);

                }


                if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
                    Parameters.MovieLib->PlayMovie(Parameters.MovieLib->kodivideolib[Menu.movielistpos].movieid);

                }

                if(Menu.currmenuid == TVSHOWLIBRARYEPISODEMENU){
                    Parameters.TVShowLib->PlayEpisode(Parameters.TVShowLib->koditvshowlib[Menu.tvshowlistpos].seasons[Menu.tvshowseasonpos].episodes[Menu.tvshowepisodepos].episodeid);

                }

                if(Menu.currmenuid == TVSHOWLIBRARYSEASONMENU){
                    Menu.tvshowepisodepos = 0;
                       Menu.currmenuid = TVSHOWLIBRARYEPISODEMENU;
                }

                if(Menu.currmenuid == TVSHOWLIBRARYLISTMENU){
                    Menu.tvshowseasonpos = 0;
                   Menu.currmenuid = TVSHOWLIBRARYSEASONMENU;
                }


                if(Menu.currmenuid == AUDIOLIBRARYMENU && Menu.menupos == 0){
                    if(Parameters.AudioLib->artistslib.size()>0){

                    }else{
                    std::string statusmsg("Requesting Audio List");
                    Menu.StatusMsg(bottom,statusmsg,statusmsg,statusmsg);
                    Parameters.AudioLib->RequestAudioList();
                    sleep(1);
                    }
                }

                if(Menu.currmenuid == AUDIOLIBRARYMENU && Menu.menupos == 1){
                    std::string statusmsg("Requesting Audio List");
                    Menu.StatusMsg(bottom,statusmsg,statusmsg,statusmsg);
                    Parameters.AudioLib->RequestAudioList();
                    sleep(1);
                }


                if(Menu.currmenuid == TVSHOWLIBRARYMENU && Menu.menupos == 1){
                    std::string statusmsg("Requesting TV Shows List");
                    Menu.StatusMsg(bottom,statusmsg,statusmsg,statusmsg);

                    Parameters.TVShowLib->RequestTVShowList();
                    sleep(1);
                    for(unsigned int i=0;i<Parameters.TVShowLib->koditvshowlib.size();i++){

                        if(Parameters.downloadimages){
                            std::string debugstring("Downloading Poster"); Menu.GUIVideoRefresh(bottom,Parameters.TVShowLib->koditvshowlib[i].label,debugstring,i,Parameters.TVShowLib->koditvshowlib.size());
                            Parameters.TVShowLib->DownloadThumb(i);
                            std::string debugstring2("Downloading and Resizing Fanart");
                            Menu.GUIVideoRefresh(bottom,Parameters.TVShowLib->koditvshowlib[i].label,debugstring2,i,Parameters.TVShowLib->koditvshowlib.size());
                            Parameters.TVShowLib->DownloadFanArt(i);
                        }

                    }
                    for(unsigned int i=0;i<Parameters.TVShowLib->koditvshowlib.size();i++){
                        Parameters.TVShowLib->RequestTVShowEpisodesList(Parameters.TVShowLib->koditvshowlib[i].showid);
                        //sleep(1);
                        usleep(100000);
                    }

                }

                if(Menu.currmenuid == VIDEOLIBRARYMENU && Menu.menupos == 1){

                    std::string statusmsg("Requesting Movies List");
                    Menu.StatusMsg(bottom,statusmsg,statusmsg,statusmsg);
                    Parameters.MovieLib->RequestMovieList();
                    sleep(1);
                    for(unsigned int i=0;i<Parameters.MovieLib->kodivideolib.size();i++){


                        if(Parameters.downloadimages){
                            std::string debugstring("Downloading Poster"); Menu.GUIVideoRefresh(bottom,Parameters.MovieLib->kodivideolib[i].label,debugstring,i,Parameters.MovieLib->kodivideolib.size());
                            Parameters.MovieLib->DownloadThumb(i);
                            std::string debugstring2("Downloading and Resizing Fanart");
                            Menu.GUIVideoRefresh(bottom,Parameters.MovieLib->kodivideolib[i].label,debugstring2,i,Parameters.MovieLib->kodivideolib.size());
                            Parameters.MovieLib->DownloadFanArt(i);
                        }
                    }

                }

                if(Menu.currmenuid == VIDEOLIBRARYMENU && Menu.menupos == 0){

                    Parameters.KodiRPC->currvideolibid = 0;
                    if(Parameters.MovieLib->kodivideolib.size()>0){


                    }else{
                        std::string statusmsg("Requesting Movies List");
                        Menu.StatusMsg(bottom,statusmsg,statusmsg,statusmsg);
                        Parameters.MovieLib->RequestMovieList();
                        sleep(1);
                        for(unsigned int i=0;i<Parameters.MovieLib->kodivideolib.size();i++){


                            if(Parameters.downloadimages){
                                std::string debugstring("Downloading Poster"); Menu.GUIVideoRefresh(bottom,Parameters.MovieLib->kodivideolib[i].label,debugstring,i,Parameters.MovieLib->kodivideolib.size());
                                Parameters.MovieLib->DownloadThumb(i);
                                std::string debugstring2("Downloading and Resizing Fanart");
                                Menu.GUIVideoRefresh(bottom,Parameters.MovieLib->kodivideolib[i].label,debugstring2,i,Parameters.MovieLib->kodivideolib.size());
                                Parameters.MovieLib->DownloadFanArt(i);
                            }
                        }
                    }
                    if(Parameters.downloadimages){
                        Parameters.MovieLib->CreateThumbTexture(Menu.movielistpos);
                        Parameters.MovieLib->CreateFanArtTexture(Menu.movielistpos);
                    }
                }

                if(Menu.currmenuid == TVSHOWLIBRARYMENU && Menu.menupos == 0){

                    Parameters.KodiRPC->currvideolibid = 0;
                    if(Parameters.TVShowLib->koditvshowlib.size()>0){


                    }else
                    {
                        std::string statusmsg("Requesting TV Shows List");
                        Menu.StatusMsg(bottom,statusmsg,statusmsg,statusmsg);
                        Parameters.TVShowLib->RequestTVShowList();
                         sleep(1);
                         for(unsigned int i=0;i<Parameters.TVShowLib->koditvshowlib.size();i++){

                             if(Parameters.downloadimages){
                                std::string debugstring("Downloading Poster"); Menu.GUIVideoRefresh(bottom,Parameters.TVShowLib->koditvshowlib[i].label,debugstring,i,Parameters.TVShowLib->koditvshowlib.size());
                                Parameters.TVShowLib->DownloadThumb(i);
                                std::string debugstring2("Downloading and Resizing Fanart");
                                Menu.GUIVideoRefresh(bottom,Parameters.TVShowLib->koditvshowlib[i].label,debugstring2,i,Parameters.TVShowLib->koditvshowlib.size());
                                Parameters.TVShowLib->DownloadFanArt(i);
                             }
                         }
                         for(unsigned int i=0;i<Parameters.TVShowLib->koditvshowlib.size();i++){
                             Parameters.TVShowLib->RequestTVShowEpisodesList(Parameters.TVShowLib->koditvshowlib[i].showid);
                             //sleep(1);
                             usleep(100000);
                         }


                    }
                    if(Parameters.downloadimages){
                        Parameters.TVShowLib->CreateThumbTexture(Menu.tvshowlistpos);
                        Parameters.TVShowLib->CreateFanArtTexture(Menu.tvshowlistpos);
                    }

                }

                if(Menu.HaveSubmenu(Menu.currmenuid,Menu.menupos) == false){

                }else
                {
                    Menu.currmenuid =  Menu.GetSubmenuID(Menu.currmenuid,Menu.menupos);
                    Menu.menupos = 0;

                }



        }

        if (kDown & KEY_B){
                if(Menu.currmenuid == REMOTEMENU){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYBACK);

                }else
                {
                    int *menuget = Menu.PrevMenuID(Menu.currmenuid);
                    Menu.currmenuid = menuget[0];
                    Menu.menupos = menuget[1];
                    free(menuget);
                }

        }


		if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){

            Menu.VideoLibRender(top,Menu.movielistpos);
		}
		if(Menu.currmenuid == TVSHOWLIBRARYLISTMENU || Menu.currmenuid == TVSHOWLIBRARYSEASONMENU){

            Menu.TVShowLibRender(top,Menu.tvshowlistpos);
		}
        if(Menu.currmenuid == TVSHOWLIBRARYEPISODEMENU){
            Menu.TVShowEpisodeRender(top,Menu.tvshowepisodepos);
        }



    }

    Parameters.runThreads = false;
    sleep(1);

/*
    threadJoin(Parameters.KodiRPC->thread, U64_MAX);
    threadFree(Parameters.KodiRPC->thread);

    threadJoin(KodiSocket.thread, U64_MAX);
    threadFree(KodiSocket.thread);
*/

    socShutdown();
    romfsExit();
    C2D_SpriteSheetFree(GFX->RemoteMenuspriteSheet);
    C2D_Fini();
	C3D_Fini();
    gfxExit();
    return 0;
}


//---------------------------------------------------------------------------------
void failExit(const char *fmt, ...) {
    //---------------------------------------------------------------------------------

    va_list ap;

    printf(CONSOLE_RED);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf(CONSOLE_RESET);
    printf("\nPress B to exit\n");

    while (aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();

        u32 kDown = hidKeysDown();
        if (kDown & KEY_B) exit(0);
    }
}
