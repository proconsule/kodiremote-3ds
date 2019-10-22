//
//  main.cpp
//  kodiclient-3ds
//
//  Created by Ceco on 20/10/2019.
//  Copyright © 2019 Ceco. All rights reserved.
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

#include "Parameters.hpp"
#include "KodiSocket.hpp"

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000

static u32 *SOC_buffer = NULL;

void failExit(const char *fmt, ...);

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
		//printf("\x1b[2J");
		printf("\x1b[1;7HKodi Connect Menu\n");
		printf("\x1b[2;3HKodi Address: %s\n",Parameters.kodiaddress);
		printf("\x1b[3;3HKodi port: %d\n",Parameters.kodiport);
		printf("\x1b[4;3HSave & Continue\n");
		printf("\x1b[5;3HContinue\n");
		menu->currmenumax = 4;
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
		//static SwkbdStatusData swkbdStatus;
		//static SwkbdLearningData swkbdLearning;
		SwkbdButton button = SWKBD_BUTTON_NONE;
		bool didit = false;

		gspWaitForVBlank();

		hidScanInput();
		u32 kDown = hidKeysDown();
		printStartMenu(&menu);

		if (kDown & KEY_A){
			if(menu.menupos == 0){
				didit = true;
				swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
				swkbdSetInitialText(&swkbd, mybuf);
				swkbdSetHintText(&swkbd, "Enter RX Address");
				swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancel", false);
				swkbdSetButton(&swkbd, SWKBD_BUTTON_MIDDLE, "OK", true);
				//swkbdSetFeatures(&swkbd, SWKBD_PREDICTIVE_INPUT);
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

		if (didit)
		{
			if (button != SWKBD_BUTTON_NONE)
			{
				if(menu.menupos == 0){
				   sscanf(mybuf,"%s",Parameters.kodiaddress);
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
    consoleSelect(&Parameters.bottomScreen);
    printf("\x1b[28;1HD-Pad to move, A = Ok , B = Back\n");
    printf("\x1b[29;1HStart to exit!\n");


}



int main(){

    gfxInitDefault();



    //KodiParameters.Init();

    Parameters.Init();
    Parameters.Read_Config(KODICONFFILE);

    consoleInit(GFX_TOP, &Parameters.topScreen);
    consoleInit(GFX_BOTTOM, &Parameters.bottomScreen);

    startMenu();
    printf("\x1b[2J");




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



    KodiSocket.Init(prio);



    while (aptMainLoop())
    {




        hidScanInput();
        u32 kDown = hidKeysDown();

        HelpMenu();

        if (kDown & KEY_START)break;

        if (kDown & KEY_LEFT){
            char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYLEFT);
            send(KodiSocket.sock,test,strlen(test),0);

        }

        if (kDown & KEY_RIGHT){
            char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYRIGHT);
            send(KodiSocket.sock,test,strlen(test),0);

        }

        if (kDown & KEY_UP){
            char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYUP);
            send(KodiSocket.sock,test,strlen(test),0);

        }

        if (kDown & KEY_DOWN){
            char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYDOWN);
            send(KodiSocket.sock,test,strlen(test),0);

        }

        if (kDown & KEY_A){
            char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYSELECT);
            send(KodiSocket.sock,test,strlen(test),0);

        }

        if (kDown & KEY_B){
            char *test = Parameters.KodiRPC->CreateMessageInputKey(KODI_KEYBACK);
            send(KodiSocket.sock,test,strlen(test),0);

        }





        gfxFlushBuffers();
        gfxSwapBuffers();

        //Wait for VBlank
        gspWaitForVBlank();


    }

    Parameters.runThreads = false;
    printf("Exit Main Thread LOOP\n");
    sleep(1);
    //threadJoin(threads, U64_MAX);
    //threadFree(threads);


    threadJoin(KodiSocket.thread, U64_MAX);
    threadFree(KodiSocket.thread);


    socShutdown();
    gfxExit();
    return 0;
}


//---------------------------------------------------------------------------------
void failExit(const char *fmt, ...) {
    //---------------------------------------------------------------------------------

    //if(sock>0) close(sock);
    //if(csock>0) close(csock);

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
