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

typedef struct{
	int menupos;
	int currmenuid;
	int prevmenuid;
	int currmenumax;
	bool havesubmenu;

}menu_struct;


void printStartMenu(menu_struct *menu){

	if(menu->currmenuid == 0){
		printf("\x1b[1;7HKodi Connect Menu\n");
		printf("\x1b[2;3HKodi Address: %s\n",Parameters.kodiaddress);
		printf("\x1b[3;3HKodi port: %d\n",Parameters.kodiport);
        printf("\x1b[4;3HKodi HTTP port: %d\n",Parameters.kodihttpport);
		printf("\x1b[5;3HSave & Continue\n");
		printf("\x1b[6;3HContinue\n");
		menu->currmenumax = 5;
		for(int i=0;i<menu->currmenumax;i++){
		  if(i==menu->menupos){
			  printf("\x1b[%d;1H*",i+2);

		  }else
		  {
			printf("\x1b[%d;1H ",i+2);
		  }
		}


	}

}



void startMenu(){

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

        
		gspWaitForVBlank();

		hidScanInput();
		u32 kDown = hidKeysDown();
		printStartMenu(&menu);

		if (kDown & KEY_A){
			if(menu.menupos == 0){
				diditip = true;
				swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
				swkbdSetInitialText(&swkbd, mybuf);
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
                if(menu.menupos == 0){
                   sscanf(mybuf,"%d",&Parameters.kodiport);
                }



            } else{

            }
                
        }
        if (didithttpport)
        {
            if (button != SWKBD_BUTTON_NONE)
            {
                if(menu.menupos == 0){
                   sscanf(mybuf,"%d",&Parameters.kodihttpport);
                }



            } else{

            }
                //printf("swkbd event: %d\n", swkbdGetResult(&swkbd));
        }
        
        gfxFlushBuffers();
        gfxSwapBuffers();

	}




}

void HelpMenu(){

    printf("\x1b[28;1HD-Pad to move, A = Ok , B = Back\n");
    printf("\x1b[29;1HStart to exit!\n");


}

C2D_TextBuf g_dynamicBuf;
C2D_TextBuf g_dynamicBuf2;

void SceneInit(){
    g_dynamicBuf = C2D_TextBufNew(4096);
    g_dynamicBuf2 = C2D_TextBufNew(4096);

}

void VideoLibRender(int videoid){

    if(Parameters.KodiRPC->kodivideolib.size() == 0){
        C2D_TextBufClear(g_dynamicBuf);
        char buf[160];
        C2D_Text dynText;
        snprintf(buf, sizeof(buf), "Refresh Library First!");
        C2D_TextParse(&dynText, g_dynamicBuf, buf);
        C2D_DrawText(&dynText, 0, 150.0f, 100.0f, 0.5f, 0.5f, 0.5f);
        return;
    }
    
    
    if(videoid == -1)return;
    C2D_TextBufClear(g_dynamicBuf);
    C2D_TextBufClear(g_dynamicBuf2);
    std::string wrappedplot = Parameters.KodiRPC->wrap(Parameters.KodiRPC->kodivideolib[videoid].plotoutline.c_str(),60);
    char buf[160];
    char buf2[wrappedplot.size()];
	C2D_Text dynText;
    C2D_Text dynText2;
	snprintf(buf, sizeof(buf), "%s (%d)", Parameters.KodiRPC->kodivideolib[videoid].label.c_str(),Parameters.KodiRPC->kodivideolib[videoid].year);
    snprintf(buf2, sizeof(buf2), "%s",wrappedplot.c_str());
	C2D_TextParse(&dynText, g_dynamicBuf, buf);
    C2D_TextParse(&dynText2, g_dynamicBuf2, buf2);
	C2D_TextOptimize(&dynText);
    C2D_TextOptimize(&dynText2);
	C2D_DrawText(&dynText, 0, 8.0f, 10.0f, 0.5f, 0.5f, 0.5f);
    C2D_DrawText(&dynText2, 0, 115.0f, 30.0f, 0.5f, 0.35f, 0.35f);

	if(Parameters.KodiRPC->thumbtext.tex != NULL){
    C2D_DrawImageAt(Parameters.KodiRPC->thumbtext, 8.0f, 30.0f, 0.5f, NULL, 0.29f, 0.29f);
	}


}



int main(){

    gfxInitDefault();

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screen
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);



  

    Parameters.Init();
    Parameters.Read_Config(KODICONFFILE);
    consoleInit(GFX_BOTTOM, NULL);

    startMenu();
    printf("\x1b[2J");




    CKodiSocket KodiSocket;


    KodiSocket.Parameters = &Parameters;


    CConsoleMenu Menu;
    Menu.Parameters = &Parameters;
    Menu.Init();


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

    sleep(1);
    printf("\x1b[2J");

    SceneInit();

    while (aptMainLoop())
    {




        hidScanInput();
        u32 kDown = hidKeysDown();


       
        if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
            char *myout = Menu.PrintVideoLibrary();
            printf(myout);
            free(myout);
        }
        else
        {
            char *myout = Menu.PrintMenu(Menu.currmenuid);
            Menu.currmenumax = Menu.MenuItems(Menu.currmenuid);
            printf(myout);
            free(myout);
        }


		if (kDown & KEY_SELECT){
            Parameters.KodiRPC->RequestMovieList();

		}

        if (kDown & KEY_START){

                if(Menu.currmenuid == REMOTEMENU){
                    printf("\x1b[2J");
                    int *menuget = Menu.PrevMenuID(Menu.currmenuid);
                    Menu.currmenuid = menuget[0];
                    Menu.menupos = menuget[1];
                    free(menuget);

                }

        }

        if (kDown & KEY_LEFT){

                if(Menu.currmenuid == REMOTEMENU){
                    char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYLEFT);
                    send(KodiSocket.sock,test,strlen(test),0);
                }

        }

        if (kDown & KEY_RIGHT){
                if(Menu.currmenuid == REMOTEMENU){
                    char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYRIGHT);
                    send(KodiSocket.sock,test,strlen(test),0);
                }
        }

        if (kDown & KEY_UP){
                if(Menu.currmenuid == REMOTEMENU){
                        char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYUP);
                        send(KodiSocket.sock,test,strlen(test),0);
                }
                else if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
                        printf("\x1b[2J");
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
                    char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYDOWN);
                    send(KodiSocket.sock,test,strlen(test),0);
                }
                else if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
                        printf("\x1b[2J");
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


                if(Menu.currmenuid == VIDEOLIBRARYMENU && Menu.menupos == 1){
                    
                    Parameters.KodiRPC->RequestMovieList();
                    
                }


                printf("\x1b[2J");

                if(Menu.HaveSubmenu(Menu.currmenuid,Menu.menupos) == false){

                }else
                {
                    Menu.currmenuid =  Menu.GetSubmenuID(Menu.currmenuid,Menu.menupos);
                    Menu.UpdateMenu(Menu.currmenuid);
                    Menu.menupos = 0;

                }

                if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
                    
                    Parameters.KodiRPC->currvideolibid = 0;
                    if(Parameters.KodiRPC->kodivideolib.size()>0){
                        Parameters.KodiRPC->CreateThumbTexture(Menu.movielistpos);
                    }
                }

                if(Menu.currmenuid == REMOTEMENU){
                    char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYSELECT);
                    send(KodiSocket.sock,test,strlen(test),0);
                }

        }

        if (kDown & KEY_B){
                if(Menu.currmenuid == REMOTEMENU){
                    char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYBACK);
                    send(KodiSocket.sock,test,strlen(test),0);
                }else
                {
                    printf("\x1b[2J");
                    int *menuget = Menu.PrevMenuID(Menu.currmenuid);
                    Menu.currmenuid = menuget[0];
                    Menu.menupos = menuget[1];
                    free(menuget);
                }

        }






        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
		C2D_SceneBegin(top);
		if(Menu.currmenuid == VIDEOLIBRARYLISTMENU){
            VideoLibRender(Menu.movielistpos);


		}

		C3D_FrameEnd(0);


    }

    Parameters.runThreads = false;
    printf("Exit Main Thread LOOP\n");
    sleep(1);
  

    threadJoin(KodiSocket.thread, U64_MAX);
    threadFree(KodiSocket.thread);


    socShutdown();
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
