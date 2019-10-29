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

#define KODICONFFILE "kodiremote-3ds.conf"


void socShutdown() {
    //---------------------------------------------------------------------------------
    printf("waiting for socExit...\n");
    socExit();

}



void startMenu(C3D_RenderTarget* target,C3D_RenderTarget* top,CConsoleMenu *Menu){

	menu_struct menu;
	menu.menupos = 0;
	menu.currmenuid = 0;

	while (aptMainLoop())
	{
		static SwkbdState swkbd;
		static char mybuf[60];
		SwkbdButton button = SWKBD_BUTTON_NONE;
		bool diditip = false;
        bool diditwsport = false;
        bool didithttpport = false;


        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
        C2D_SceneBegin(top);
        C3D_FrameEnd(0);
		hidScanInput();
		u32 kDown = hidKeysDown();
        Menu->GUIStartMenu(target,&menu);

		if (kDown & KEY_A){
			if(menu.menupos == 0){
				diditip = true;
				swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
				swkbdSetInitialText(&swkbd, Parameters.kodiaddress);
				swkbdSetHintText(&swkbd, "Enter Kodi Address");
				swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancel", false);
				swkbdSetButton(&swkbd, SWKBD_BUTTON_MIDDLE, "OK", true);

				button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
			}
            if(menu.menupos == 1){
                diditwsport = true;
                swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 8);
                swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
                swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
                swkbdSetNumpadKeys(&swkbd, L'ツ', L'益');
                swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancel", false);
                swkbdSetButton(&swkbd, SWKBD_BUTTON_MIDDLE, "OK", true);
                button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
            }

            if(menu.menupos == 2){
                didithttpport = true;
                swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 8);
                swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
                swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
                swkbdSetNumpadKeys(&swkbd, L'ツ', L'益');
                button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
            }



			if(menu.menupos == menu.currmenumax-2){
				Parameters.Save_Config(KODICONFFILE);
				break;
			}

			if(menu.menupos == menu.currmenumax-1){
				break;
			}
		}

		if (kDown & KEY_DOWN){
			if(menu.menupos >= menu.currmenumax-1){
			   menu.menupos = 0;
			}else{
				menu.menupos++;
			}
		}

		if (kDown & KEY_UP){
			if(menu.menupos < 1){
			   menu.menupos = menu.currmenumax-1;
			}else
			{
				menu.menupos--;
			}
		}

		if (diditip)
		{
			if (button != SWKBD_BUTTON_NONE)
			{
				if(menu.menupos == 0){
				   sscanf(mybuf,"%s",Parameters.kodiaddress);
				}



			} else{

			}

		}
        if (diditwsport)
        {
            if (button != SWKBD_BUTTON_NONE)
            {
                if(menu.menupos == 1){
                   sscanf(mybuf,"%d",&Parameters.kodiport);
                }



            } else{

            }

        }
        if (didithttpport)
        {
            if (button != SWKBD_BUTTON_NONE)
            {
                if(menu.menupos == 2){
                   sscanf(mybuf,"%d",&Parameters.kodihttpport);
                }



            } else{

            }
        }

	}




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






    startMenu(bottom,top,&Menu);

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

    KodiSocket.Init(prio);



    while (aptMainLoop())
    {




        hidScanInput();
        u32 kDown = hidKeysDown();

        touchPosition touch;
        hidTouchRead(&touch);




        if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
            Menu.GUIMenuVideoLibrary(bottom);
        }
        else if(Menu.currmenuid == REMOTEMENU){
            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            C2D_TargetClear(top, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
            C2D_SceneBegin(top);
            C3D_FrameEnd(0);
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
            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            C2D_TargetClear(top, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
            C2D_SceneBegin(top);
            C3D_FrameEnd(0);


            Menu.GUIMenu(bottom,Menu.currmenuid);
        }


		if (kDown & KEY_SELECT){
            Parameters.KodiRPC->RequestMovieList();

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
                            Menu.movielistpos = Parameters.KodiRPC->kodivideolib.size()-1;
                        }
                        else
                        {
                            Menu.movielistpos--;
                        }
                        if(Parameters.KodiRPC->kodivideolib.size()>0){
                            Parameters.KodiRPC->CreateThumbTexture(Menu.movielistpos);
                        }


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
                        if((unsigned int)Menu.movielistpos >= Parameters.KodiRPC->kodivideolib.size()-1){
                            Menu.movielistpos = 0;
                        }
                        else
                        {
                            Menu.movielistpos++;
                        }
                    if(Parameters.KodiRPC->kodivideolib.size()>0){
                        Parameters.KodiRPC->CreateThumbTexture(Menu.movielistpos);
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

        if (kDown & KEY_A){
                if(Menu.currmenuid == MAINMENU && Menu.menupos == Menu.currmenumax-1){
                    break;
                }

                if(Menu.currmenuid == REMOTEMENU){
                    Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYSELECT);

                }

                if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
                    Parameters.KodiRPC->PlayMovie(Parameters.KodiRPC->kodivideolib[Menu.movielistpos].movieid);

                }


                if(Menu.currmenuid == VIDEOLIBRARYMENU && Menu.menupos == 1){

                    Parameters.KodiRPC->RequestMovieList();
                    sleep(1);
                    for(unsigned int i=0;i<Parameters.KodiRPC->kodivideolib.size();i++){
                        Menu.GUIVideoRefresh(bottom,Parameters.KodiRPC->kodivideolib[i].label,i,Parameters.KodiRPC->kodivideolib.size());

                        Parameters.KodiRPC->DownloadMovieThumb(i);
                    }

                }

                if(Menu.currmenuid == VIDEOLIBRARYMENU && Menu.menupos == 0){

                    Parameters.KodiRPC->currvideolibid = 0;
                    if(Parameters.KodiRPC->kodivideolib.size()>0){
                        Parameters.KodiRPC->CreateThumbTexture(Menu.movielistpos);
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



    }

    Parameters.runThreads = false;
    printf("Exit Main Thread LOOP\n");
    sleep(1);


    threadJoin(KodiSocket.thread, U64_MAX);
    threadFree(KodiSocket.thread);


    socShutdown();
    C2D_SpriteSheetFree(GFX->RemoteMenuspriteSheet);
    C2D_Fini();
	C3D_Fini();
    gfxExit();
    romfsExit();
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
