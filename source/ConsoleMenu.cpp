#include "ConsoleMenu.hpp"


int CConsoleMenu::GetSubmenuID(int menuid,int pos){
	int menuidnum = 0;
	for(unsigned int i=0;i<Menu.size();i++){
	    if(Menu[i].menuid == menuid){
			menuidnum = i;
		}
	 }
	return Menu[menuidnum].submenuid[pos];
}

bool CConsoleMenu::HaveSubmenu(int menuid,int pos){
	int menuidnum = 0;
	for(unsigned int i=0;i<Menu.size();i++){
	    if(Menu[i].menuid == menuid){
			menuidnum = i;
		}
	 }
    if(Menu[menuidnum].havesubmenu.size() > 0){
	 return Menu[menuidnum].havesubmenu[pos];
    }
    else{
        return false;
    }

}

int CConsoleMenu::MenuItems(int menuid){
	int menuidnum = 0;
	for(unsigned int i=0;i<Menu.size();i++){
	    if(Menu[i].menuid == menuid){
			menuidnum = i;
		}
	 }
	 return Menu[menuidnum].menudesc.size();

}

int * CConsoleMenu::PrevMenuID(int menuid){
	int * ret = (int *)malloc(2);
	if(menuid <10){
		ret[0] = MAINMENU;
		ret[1] = 0;
	}
	if(menuid == REMOTEMENU){
		ret[0] = MAINMENU;
		ret[1] = 0;

	}
	if(menuid == LIBRARYMENU){
		ret[0] = MAINMENU;
		ret[1] = 1;
	}

    if(menuid == SETTINGSMENU){
        ret[0] = MAINMENU;
        ret[1] = 2;
    }

	if(menuid == AUDIOLIBRARYMENU){
		ret[0] = LIBRARYMENU;
		ret[1] = 0;
	}

	if(menuid == AUDIOLIBRARYLISTMENU){
		ret[0] = AUDIOLIBRARYMENU;
		ret[1] = 0;
	}

	if(menuid == AUDIOLIBRARYALBUMMENU){
		ret[0] = AUDIOLIBRARYLISTMENU;
		ret[1] = audioartistpos;
	}

	if(menuid == AUDIOLIBRARYSONGMENU){
		ret[0] = AUDIOLIBRARYALBUMMENU;
		ret[1] = audioalbumpos;
	}

	if(menuid == VIDEOLIBRARYMENU){
		ret[0] = LIBRARYMENU;
		ret[1] = 1;
	}
	if(menuid == VIDEOLIBRARYLISTMENU){
		ret[0] = VIDEOLIBRARYMENU;
		ret[1] = 0;
	}

	if(menuid == TVSHOWLIBRARYMENU){
		ret[0] = LIBRARYMENU;
		ret[1] = 2;
	}

	if(menuid == TVSHOWLIBRARYLISTMENU){
		ret[0] = TVSHOWLIBRARYMENU;
		ret[1] = 0;
	}
	if(menuid == TVSHOWLIBRARYSEASONMENU){
		ret[0] = TVSHOWLIBRARYLISTMENU;
		ret[1] = tvshowlistpos;
	}
    if(menuid == TVSHOWLIBRARYEPISODEMENU){
        ret[0] = TVSHOWLIBRARYSEASONMENU;
        ret[1] = tvshowseasonpos;
    }


	return ret;
}

void CConsoleMenu::Init(){

	menupos = 0;

	currmenuid = 0;

	movielistpos = 0;

    tvshowlistpos = 0;
    tvshowseasonpos = 0;
    tvshowepisodepos = 0;

    audioartistpos = 0;
    audioalbumpos = 0;
    audiosongpos = 0;

	menupage_struct menu0;

	menu0.menutitle = "KodiRemote 3DS";
	menu0.menuid = MAINMENU;
	menu0.menudesc.push_back("Remote");
	menu0.havesubmenu.push_back(true);
	menu0.submenuid.push_back(REMOTEMENU);

	menu0.menudesc.push_back("Library");
	menu0.havesubmenu.push_back(true);
	menu0.submenuid.push_back(LIBRARYMENU);

    menu0.menudesc.push_back("Settings");
    menu0.havesubmenu.push_back(true);
    menu0.submenuid.push_back(SETTINGSMENU);

	menu0.menudesc.push_back("Exit");
	menu0.havesubmenu.push_back(false);
	menu0.submenuid.push_back(-1);

	Menu.push_back(menu0);

	menupage_struct menu10;

	menu10.menutitle = "Remote Control";
	menu10.menuid = REMOTEMENU;


	Menu.push_back(menu10);


	menupage_struct menu20;

	menu20.menutitle = "Library";
	menu20.menuid = LIBRARYMENU;


	menu20.menudesc.push_back("Audio");
	menu20.havesubmenu.push_back(true);
	menu20.submenuid.push_back(AUDIOLIBRARYMENU);


	menu20.menudesc.push_back("Movie");
	menu20.havesubmenu.push_back(true);
	menu20.submenuid.push_back(VIDEOLIBRARYMENU);

	menu20.menudesc.push_back("TV Show");
	menu20.havesubmenu.push_back(true);
	menu20.submenuid.push_back(TVSHOWLIBRARYMENU);

	Menu.push_back(menu20);

    menupage_struct menu30;

    menu30.menutitle = "Settings";
    menu30.menuid = SETTINGSMENU;

    Menu.push_back(menu30);





	menupage_struct menu210;

	menu210.menutitle = "Audio Library";
	menu210.menuid = AUDIOLIBRARYMENU;

	menu210.menudesc.push_back("List");
	menu210.havesubmenu.push_back(true);
	menu210.submenuid.push_back(AUDIOLIBRARYLISTMENU);

	menu210.menudesc.push_back("Refresh");
	menu210.havesubmenu.push_back(false);
	menu210.submenuid.push_back(-1);


	Menu.push_back(menu210);

	menupage_struct menu220;

	menu220.menutitle = "Movie Library";
	menu220.menuid = VIDEOLIBRARYMENU;


	menu220.menudesc.push_back("List");
	menu220.havesubmenu.push_back(true);
	menu220.submenuid.push_back(VIDEOLIBRARYLISTMENU);

	menu220.menudesc.push_back("Refresh");
	menu220.havesubmenu.push_back(false);
	menu220.submenuid.push_back(-1);


	Menu.push_back(menu220);


	menupage_struct menu320;

	menu320.menutitle = "TV Show Library";
	menu320.menuid = TVSHOWLIBRARYMENU;


	menu320.menudesc.push_back("List");
	menu320.havesubmenu.push_back(true);
	menu320.submenuid.push_back(TVSHOWLIBRARYLISTMENU);

	menu320.menudesc.push_back("Refresh");
	menu320.havesubmenu.push_back(false);
	menu320.submenuid.push_back(-1);


	Menu.push_back(menu320);


	menupage_struct menu1220;

	menu1220.menutitle = "";
	menu1220.menuid = VIDEOLIBRARYLISTMENU;

	Menu.push_back(menu1220);

	menupage_struct menu1320;

	menu1320.menutitle = "";
	menu1320.menuid = TVSHOWLIBRARYLISTMENU;

	Menu.push_back(menu1320);

	menupage_struct menu11320;

	menu11320.menutitle = "";
	menu11320.menuid = TVSHOWLIBRARYSEASONMENU;

	Menu.push_back(menu11320);

    menupage_struct menu111320;
    menu111320.menutitle = "";
    menu111320.menuid = TVSHOWLIBRARYEPISODEMENU;

    Menu.push_back(menu111320);

    menupage_struct menu1120;
    menu1120.menutitle = "";
    menu1120.menuid = AUDIOLIBRARYLISTMENU;

    Menu.push_back(menu1120);

    menupage_struct menu11120;
    menu11120.menutitle = "";
    menu11120.menuid = AUDIOLIBRARYALBUMMENU;

    Menu.push_back(menu11120);

    menupage_struct menu111120;
    menu111120.menutitle = "";
    menu111120.menuid = AUDIOLIBRARYSONGMENU;

    Menu.push_back(menu111120);



}

void CConsoleMenu::GUIInit(){
    menudynbuffer = C2D_TextBufNew(4096);
    menudynbuffer2 = C2D_TextBufNew(4096);
    menudynbuffer3 = C2D_TextBufNew(4096);

    topmenudynbuffer = C2D_TextBufNew(4096);
    topmenudynbuffer2 = C2D_TextBufNew(4096);

}

void CConsoleMenu::GUIMenu(C3D_RenderTarget* target,int menuid){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    if(Parameters->connected == 1){
        C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    }else
    {
        C2D_TargetClear(target, C2D_Color32(0xFF, 0x00, 0x00, 0xFF));
    }
    C2D_SceneBegin(target);
    int menuidnum = 0;
    for(unsigned int i=0;i<Menu.size();i++){
       if(Menu[i].menuid == menuid){
           menuidnum = i;
       }
    }
    C2D_TextBufClear(menudynbuffer);
    C2D_TextBufClear(menudynbuffer2);
    C2D_TextBufClear(menudynbuffer3);
    std::string menutext;
    for(unsigned int i=0;i<Menu[menuidnum].menudesc.size();i++){
        menutext.append(Menu[menuidnum].menudesc[i]);
        menutext.append("\n",1);

    }
    char buf[menutext.size()];
    char buf2[Menu[menuidnum].menutitle.size()+1];
    char buf3[160];
    C2D_Text dynText;
    C2D_Text dynText2;
    C2D_Text dynText3;
    snprintf(buf, sizeof(buf),"%s",menutext.c_str());
    snprintf(buf2, sizeof(buf2),"%s",Menu[menuidnum].menutitle.c_str());
    if(Parameters->connected == 1){
        snprintf(buf3, sizeof(buf3),"Kodi Version %d.%d %s",Parameters->KodiRPC->kodiversion.major,Parameters->KodiRPC->kodiversion.minor,Parameters->KodiRPC->kodiversion.tag);
    }
    else
    {
        snprintf(buf3, sizeof(buf3),"Not Connected");
    }
    C2D_TextParse(&dynText, menudynbuffer, buf);
    C2D_TextParse(&dynText2, menudynbuffer2, buf2);
    C2D_TextParse(&dynText3, menudynbuffer3, buf3);
    C2D_TextOptimize(&dynText);
    C2D_TextOptimize(&dynText2);
    C2D_TextOptimize(&dynText3);

    float startw,starth;
    float textw,texth,titlew,titleh,footerw,footerh;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&textw,&texth);
    C2D_TextGetDimensions(&dynText2,0.5f,0.5f,&titlew,&titleh);
    C2D_TextGetDimensions(&dynText3,0.5f,0.5f,&footerw,&footerh);
    startw = 160.0f - (textw/2.0);
    starth = 120.0f - (texth/2.0);
    float startwtitle = 160.0f - (titlew/2);
    float starthtitle = 10.0f;

    float startwfooter = 160.0f - (footerw/2);
    float starthfooter = 240.0 - (footerh) -10.0f;

    C2D_DrawText(&dynText, 0, startw, starth, 0.5f, 0.5f, 0.5f);
    C2D_DrawText(&dynText2, 0, startwtitle, starthtitle, 0.5f, 0.5f, 0.5f);
    C2D_DrawText(&dynText3, 0, startwfooter, starthfooter, 0.5f, 0.5f, 0.5f);
    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);

    float incpos = 15.0f;

    C2D_DrawRectSolid(startw-1.0,starth-1.0+(incpos*menupos),0.6f,textw+1.0,15.0f,clrBlack);

    C3D_FrameEnd(0);
}

void CConsoleMenu::GUIVideoRefresh(C3D_RenderTarget* target,std::string label,std::string debugmsg,int current,int max){

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    C2D_TextBufClear(menudynbuffer2);
    C2D_TextBufClear(menudynbuffer3);
    char buf[label.size()+1];
    char buf2[30];
    char bufDebug[64];
    C2D_Text dynText;
    C2D_Text dynText2;
    C2D_Text dynText3;
    snprintf(buf, sizeof(buf),"%s",label.c_str());
    float currperc = ((current+1)*1.0)*100/(max*1.0);

    snprintf(buf2, sizeof(buf2),"%.2f%%",currperc);

    snprintf(bufDebug, sizeof(bufDebug),"%s",debugmsg.c_str());



    C2D_TextParse(&dynText, menudynbuffer, buf);
    C2D_TextParse(&dynText2, menudynbuffer2, buf2);
    C2D_TextParse(&dynText3, menudynbuffer3, bufDebug);
    C2D_TextOptimize(&dynText);
    C2D_TextOptimize(&dynText2);
    C2D_TextOptimize(&dynText3);
    float textw,texth;
    float perctextw,perctexth;
    float debugw,debugh;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&textw,&texth);
    C2D_TextGetDimensions(&dynText2,0.5f,0.5f,&perctextw,&perctexth);
    C2D_TextGetDimensions(&dynText3,0.5f,0.5f,&debugw,&debugh);
    float textstartw = (textw/2.0);

    C2D_DrawText(&dynText, 0, 160.0-textstartw, 60.0f, 0.5f, 0.5f, 0.5f);



    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);
    u32 clrWhite = C2D_Color32(0xff, 0xff, 0xff, 0xff);

    float startw,starth,endw;
    startw = 8.0f;
    starth = 100.0f;
    endw = 310.0f;
    float singlestep = (endw-startw)/(max*1.0);

    C2D_DrawRectSolid(startw,starth,0.5f,endw,18.0f,clrBlack);
    C2D_DrawRectSolid(startw,starth+1.0,0.5f,startw+(singlestep*((current+1)*1.0)),16.0f,clrWhite);
    C2D_DrawText(&dynText2, 0, 160.0-(perctextw/2.0), 102.0f, 0.5f, 0.5f, 0.5f);

    C2D_DrawText(&dynText3, 0,  160.0-(debugw/2.0), 140.0f, 0.5f, 0.5f, 0.5f);

    C3D_FrameEnd(0);


}


void CConsoleMenu::startMenu(C3D_RenderTarget* target,C3D_RenderTarget* top,CConsoleMenu *Menu){

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
				swkbdSetInitialText(&swkbd, Parameters->kodiaddress);
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

            if(menu.menupos == 3){
                Parameters->downloadimages = !Parameters->downloadimages;

                std::vector<kodivideolib_struct>().swap(Parameters->MovieLib->kodivideolib);
                std::vector<koditvshowlib_struct>().swap(Parameters->TVShowLib->koditvshowlib);
            }


			if(menu.menupos == menu.currmenumax-2){
				Parameters->Save_Config(KODICONFFILE);
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
				   sscanf(mybuf,"%s",Parameters->kodiaddress);
				}



			} else{

			}

		}
        if (diditwsport)
        {
            if (button != SWKBD_BUTTON_NONE)
            {
                if(menu.menupos == 1){
                   sscanf(mybuf,"%d",&Parameters->kodiport);
                }



            } else{

            }

        }
        if (didithttpport)
        {
            if (button != SWKBD_BUTTON_NONE)
            {
                if(menu.menupos == 2){
                   sscanf(mybuf,"%d",&Parameters->kodihttpport);
                }



            } else{

            }
        }

	}




}



void CConsoleMenu::GUIStartMenu(C3D_RenderTarget* target,menu_struct *menu){



    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    if(menu->currmenuid == 0){
        menu->currmenumax = 6;
        std::string menutext;
        char stringbuf[64];
        memset(stringbuf,0,64);
        sprintf(stringbuf,"Kodi Address: %s\n",Parameters->kodiaddress);
        menutext.append(stringbuf,strlen(stringbuf));
        memset(stringbuf,0,64);
        sprintf(stringbuf,"Kodi WS port: %d\n",Parameters->kodiport);
        menutext.append(stringbuf,strlen(stringbuf));
        memset(stringbuf,0,64);
        sprintf(stringbuf,"Kodi HTTP port: %d\n",Parameters->kodihttpport);
        menutext.append(stringbuf,strlen(stringbuf));
        memset(stringbuf,0,64);
        sprintf(stringbuf,"Download Images: %s\n",Parameters->downloadimages ? "true" : "false");
        menutext.append(stringbuf,strlen(stringbuf));
        memset(stringbuf,0,64);
        sprintf(stringbuf,"Save & Continue\n");
        menutext.append(stringbuf,strlen(stringbuf));
        memset(stringbuf,0,64);
        sprintf(stringbuf,"Continue\n");
        menutext.append(stringbuf,strlen(stringbuf));


        char buf[menutext.size()];
        C2D_Text dynText;
        snprintf(buf, sizeof(buf),"%s",menutext.c_str());
        C2D_TextParse(&dynText, menudynbuffer, buf);
        C2D_TextOptimize(&dynText);
        float startw,starth;
        startw = 8.0f;
        starth = 10.0f;
        C2D_DrawText(&dynText, 0, startw, starth, 0.5f, 0.5f, 0.5f);
        u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);
        float textw,texth;
        C2D_TextGetDimensions(&dynText,0.5f,0.5f,&textw,&texth);
        float incpos = 15.0f;

        C2D_DrawRectSolid(startw-1.0,starth-1.0+(incpos*menu->menupos),0.6f,textw+1.0,15.0f,clrBlack);

        C3D_FrameEnd(0);

    }

}

void CConsoleMenu::DebugTop(C3D_RenderTarget* target){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x00, 0x00, 0x00, 0xFF));
    C2D_SceneBegin(target);


    C2D_TextBufClear(topmenudynbuffer);
    C2D_TextBufClear(topmenudynbuffer2);
    C2D_Text dynText;
    C2D_Text dynText2;

    char buf[256];
    char buf2[128];
    memset(buf,0,sizeof(buf));
    memset(buf2,0,sizeof(buf2));
    if(Parameters->KodiRPC->playerid == -1){
        snprintf(buf, sizeof(buf),"Player Inactive");
        snprintf(buf2, sizeof(buf2)," ");
    }
    else if(Parameters->KodiRPC->playerid ==1)
    {

    if(Parameters->KodiRPC->player_viseostatus.type != NULL){
    if(strcmp(Parameters->KodiRPC->player_viseostatus.type,"episode") == 0){
        if(Parameters->KodiRPC->player_viseostatus.showtitle == NULL){
            snprintf(buf, sizeof(buf)," ");
        }else{
            snprintf(buf, sizeof(buf), "%s\n%s [S%02d E%02d]\n", Parameters->KodiRPC->player_viseostatus.showtitle,Parameters->KodiRPC->player_viseostatus.label,Parameters->KodiRPC->player_viseostatus.season,Parameters->KodiRPC->player_viseostatus.episode);
        }
    }
    if(strcmp(Parameters->KodiRPC->player_viseostatus.type,"movie") == 0){
        if(Parameters->KodiRPC->player_viseostatus.label == NULL){
            snprintf(buf, sizeof(buf)," ");
        }else{
            snprintf(buf, sizeof(buf), "%s\n", Parameters->KodiRPC->player_viseostatus.label);
        }
    }

    snprintf(buf2, sizeof(buf2), "%02d:%02d:%02d / %02d:%02d:%02d", Parameters->KodiRPC->player_viseostatus.currenttime.hour,Parameters->KodiRPC->player_viseostatus.currenttime.minutes,Parameters->KodiRPC->player_viseostatus.currenttime.seconds,Parameters->KodiRPC->player_viseostatus.totaltime.hour,Parameters->KodiRPC->player_viseostatus.totaltime.minutes,Parameters->KodiRPC->player_viseostatus.totaltime.seconds);
    }
    else
    {
        snprintf(buf, sizeof(buf)," ");
    }
    }
    else if(Parameters->KodiRPC->playerid ==0)
    {
     if(Parameters->KodiRPC->player_audiostatus.title == NULL){
         snprintf(buf, sizeof(buf)," ");
     }else
     {
         snprintf(buf, sizeof(buf), "%s\n%s (%d)\n%s\n", Parameters->KodiRPC->player_audiostatus.title,Parameters->KodiRPC->player_audiostatus.album,Parameters->KodiRPC->player_audiostatus.year,Parameters->KodiRPC->player_audiostatus.artist);

     }
     snprintf(buf2, sizeof(buf2), "%02d:%02d:%02d / %02d:%02d:%02d", Parameters->KodiRPC->player_audiostatus.currenttime.hour,Parameters->KodiRPC->player_audiostatus.currenttime.minutes,Parameters->KodiRPC->player_audiostatus.currenttime.seconds,Parameters->KodiRPC->player_audiostatus.totaltime.hour,Parameters->KodiRPC->player_audiostatus.totaltime.minutes,Parameters->KodiRPC->player_audiostatus.totaltime.seconds);

    }


    C2D_TextParse(&dynText, topmenudynbuffer, buf);
    C2D_TextParse(&dynText2, topmenudynbuffer2, buf2);
    C2D_TextOptimize(&dynText);
    C2D_TextOptimize(&dynText2);


    float timew,timeh;
    C2D_TextGetDimensions(&dynText2,0.5f,0.5f,&timew,&timeh);


    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);
    u32 clrBlue = C2D_Color32(0x0F, 0x0F, 0x0F, 0xFF);
    u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);


    C2D_DrawText(&dynText, C2D_WithColor , 8.0f, 8.0f, 0.5f, 0.5f, 0.5f,clrWhite);
    C2D_DrawText(&dynText2, C2D_WithColor , 390.0f-(timew), 220.0f-timeh, 0.5f, 0.5f, 0.5f,clrWhite);



    if(Parameters->KodiRPC->playerid == 1){
        float percsize = (380.0*Parameters->KodiRPC->player_viseostatus.perc)/100.0f;
        C2D_DrawRectSolid(10.0f,220.0f,0.5f,380.0f,12.0f,clrBlue);
        C2D_DrawRectSolid(10.0f,222.0f,0.6f,percsize,8.0f,clrWhite);
    }
    if(Parameters->KodiRPC->playerid == 0){
        float percsize = (380.0*Parameters->KodiRPC->player_audiostatus.perc)/100.0f;
        C2D_DrawRectSolid(10.0f,220.0f,0.5f,380.0f,12.0f,clrBlue);
        C2D_DrawRectSolid(10.0f,222.0f,0.6f,percsize,8.0f,clrWhite);
    }


    C3D_FrameEnd(0);
}


void CConsoleMenu::StatusMsg(C3D_RenderTarget* target,std::string line1,std::string line2,std::string line3){

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    //C2D_TextBufClear(menudynbuffer2);
    //C2D_TextBufClear(menudynbuffer3);


    char buf[line1.size()+1];
    //char buf2[line2.size()];
    //char buf3[line3.size()];

    C2D_Text dynText;
    //C2D_Text dynText2;
    //C2D_Text dynText3;
    snprintf(buf, sizeof(buf), "%s", line1.c_str());
    //snprintf(buf2, sizeof(buf2), "%s", line2.c_str());
    //snprintf(buf3, sizeof(buf3), "%s", line3.c_str());

    C2D_TextParse(&dynText, menudynbuffer, buf);
    //C2D_TextParse(&dynText2, menudynbuffer2, buf2);
    //C2D_TextParse(&dynText3, menudynbuffer3, buf3);
    C2D_TextOptimize(&dynText);
    //C2D_TextOptimize(&dynText2);
    //C2D_TextOptimize(&dynText3);

    float line1w,line1h,line2w,line2h,line3w,line3h;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&line1w,&line1h);

    float startline1w,startline1h,startline2w,startline2h,startline3w,startline3h;

    startline1w = 160.0f-(line1w/2.0);
    startline1h = 120.0f-line1h;

    C2D_DrawText(&dynText, 0, startline1w, startline1h, 0.5f, 0.5f, 0.5f);


    C3D_FrameEnd(0);
}


void CConsoleMenu::GUIMenuVideoLibrary(C3D_RenderTarget* target){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    std::string menutext;
    for(int i = movielistpos;i<movielistpos+10;i++){
        if((unsigned int)i<Parameters->MovieLib->kodivideolib.size()){
            menutext.append(Parameters->MovieLib->kodivideolib[i].label.c_str());
            menutext.append("\n",1);
        }

    }
    char buf[menutext.size()];
    C2D_Text dynText;
    snprintf(buf, sizeof(buf),"%s",menutext.c_str());
    C2D_TextParse(&dynText, menudynbuffer, buf);
    C2D_TextOptimize(&dynText);
    float startw,starth;
    float textw,texth;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&textw,&texth);
    startw = 8.0f;
    starth = 10.0f;
    C2D_DrawText(&dynText, 0, startw, starth, 0.5f, 0.5f, 0.5f);
    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);

    float incpos = 15.0f;

    C2D_DrawRectSolid(startw-1.0,starth-1.0+(incpos*0),0.6f,textw+1.0,15.0f,clrBlack);

    C3D_FrameEnd(0);



}

void CConsoleMenu::GUIMenuTVShowLibrary(C3D_RenderTarget* target){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    std::string menutext;
    for(int i = tvshowlistpos;i<tvshowlistpos+10;i++){
        if((unsigned int)i<Parameters->TVShowLib->koditvshowlib.size()){
            menutext.append(Parameters->TVShowLib->koditvshowlib[i].label.c_str());
            menutext.append("\n",1);
        }

    }
    char buf[menutext.size()];
    C2D_Text dynText;
    snprintf(buf, sizeof(buf),"%s",menutext.c_str());
    C2D_TextParse(&dynText, menudynbuffer, buf);
    C2D_TextOptimize(&dynText);
    float startw,starth;
    float textw,texth;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&textw,&texth);
    startw = 8.0f;
    starth = 10.0f;
    C2D_DrawText(&dynText, 0, startw, starth, 0.5f, 0.5f, 0.5f);
    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);

    float incpos = 15.0f;

    C2D_DrawRectSolid(startw-1.0,starth-1.0+(incpos*0),0.6f,textw+1.0,15.0f,clrBlack);

    C3D_FrameEnd(0);



}

void CConsoleMenu::GUIMenuTVShowSeason(C3D_RenderTarget* target){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    std::string menutext;
    for(int i = tvshowseasonpos;i<tvshowseasonpos+10;i++){
        if((unsigned int)i<Parameters->TVShowLib->koditvshowlib[tvshowlistpos].seasons.size()){
            char output[30];
            sprintf(output,"Season %d",Parameters->TVShowLib->koditvshowlib[tvshowlistpos].seasons[i].season);
            menutext.append(output,strlen(output));
            menutext.append("\n",1);
        }

    }
    char buf[menutext.size()];
    C2D_Text dynText;
    snprintf(buf, sizeof(buf),"%s",menutext.c_str());
    C2D_TextParse(&dynText, menudynbuffer, buf);
    C2D_TextOptimize(&dynText);
    float startw,starth;
    float textw,texth;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&textw,&texth);
    startw = 8.0f;
    starth = 10.0f;
    C2D_DrawText(&dynText, 0, startw, starth, 0.5f, 0.5f, 0.5f);
    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);

    float incpos = 15.0f;

    C2D_DrawRectSolid(startw-1.0,starth-1.0+(incpos*0),0.6f,textw+1.0,15.0f,clrBlack);

    C3D_FrameEnd(0);



}

void CConsoleMenu::GUIMenuTVShowEpisode(C3D_RenderTarget* target){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    std::string menutext;
    for(int i = tvshowepisodepos;i<tvshowepisodepos+10;i++){
        if((unsigned int)i<Parameters->TVShowLib->koditvshowlib[tvshowlistpos].seasons[tvshowseasonpos].episodes.size()){
            char output[128];
            sprintf(output,"%s",Parameters->TVShowLib->koditvshowlib[tvshowlistpos].seasons[tvshowseasonpos].episodes[i].label.c_str());
            menutext.append(output,strlen(output));
            menutext.append("\n",1);
        }

    }
    char buf[menutext.size()];
    C2D_Text dynText;
    snprintf(buf, sizeof(buf),"%s",menutext.c_str());
    C2D_TextParse(&dynText, menudynbuffer, buf);
    C2D_TextOptimize(&dynText);
    float startw,starth;
    float textw,texth;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&textw,&texth);
    startw = 8.0f;
    starth = 10.0f;
    C2D_DrawText(&dynText, 0, startw, starth, 0.5f, 0.5f, 0.5f);
    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);

    float incpos = 15.0f;

    C2D_DrawRectSolid(startw-1.0,starth-1.0+(incpos*0),0.6f,textw+1.0,15.0f,clrBlack);

    C3D_FrameEnd(0);



}

void CConsoleMenu::VideoLibRender(C3D_RenderTarget* target,int videoid){

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x00, 0x00, 0x00, 0xFF));
    C2D_SceneBegin(target);

    if(Parameters->MovieLib->kodivideolib.size() == 0){
        C2D_TextBufClear(topmenudynbuffer);
        char buf[160];
        C2D_Text dynText;
        snprintf(buf, sizeof(buf), "Refresh Library First!");
        C2D_TextParse(&dynText, topmenudynbuffer, buf);
        C2D_DrawText(&dynText, 0, 150.0f, 100.0f, 0.5f, 0.5f, 0.5f);
        return;
    }


    if(videoid == -1)return;
    C2D_TextBufClear(topmenudynbuffer);
    C2D_TextBufClear(topmenudynbuffer2);
    std::string wrappedplot = Parameters->KodiRPC->wrap(Parameters->MovieLib->kodivideolib[videoid].plot.c_str(),55);
    char buf[160];
    char buf2[wrappedplot.size()];
    C2D_Text dynText;
    C2D_Text dynText2;
    snprintf(buf, sizeof(buf), "%s (%d)", Parameters->MovieLib->kodivideolib[videoid].label.c_str(),Parameters->MovieLib->kodivideolib[videoid].year);
    snprintf(buf2, sizeof(buf2), "%s",wrappedplot.c_str());
    C2D_TextParse(&dynText, topmenudynbuffer, buf);
    C2D_TextParse(&dynText2, topmenudynbuffer2, buf2);
    C2D_TextOptimize(&dynText);
    C2D_TextOptimize(&dynText2);
    u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
    C2D_DrawText(&dynText, C2D_WithColor, 8.0f, 10.0f, 0.5f, 0.5f, 0.5f,clrWhite);
    C2D_DrawText(&dynText2, C2D_WithColor, 120.0f, 30.0f, 0.5f, 0.38f, 0.38f,clrWhite);

    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0xFF);

    float textw,texth,titlew,titleh;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&titlew,&titleh);
    C2D_TextGetDimensions(&dynText2,0.38f,0.38f,&textw,&texth);
    C2D_DrawRectSolid(6.0,8.0f,0.4f,titlew+2.0,titleh+2.0,clrBlack);
    C2D_DrawRectSolid(117.0f,28.0f,0.4f,textw+2.0,texth+2.0,clrBlack);

    if(Parameters->MovieLib->thumbimage.thumbtext.tex != NULL){
    float scaleratio = (115.0-8.0)/(Parameters->MovieLib->thumbimage.thumbtext.subtex->width*1.0);
    C2D_DrawImageAt(Parameters->MovieLib->thumbimage.thumbtext, 8.0f, 30.0f, 0.5f, NULL, scaleratio, scaleratio);
    }
    if(Parameters->MovieLib->fanartimage.thumbtext.tex != NULL){
    float scaleratio = (400.0)/(Parameters->MovieLib->fanartimage.thumbtext.subtex->width*1.0);
        float heightstart =(240.0-Parameters->MovieLib->fanartimage.thumbtext.subtex->height)/2.0;
    //    float scaleratio = 1.0;
    C2D_DrawImageAt(Parameters->MovieLib->fanartimage.thumbtext, 0.0, heightstart, 0.3f, NULL, scaleratio, scaleratio);
    }

     C3D_FrameEnd(0);

}

void CConsoleMenu::TVShowLibRender(C3D_RenderTarget* target,int showid){

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x00, 0x00, 0x00, 0xFF));
    C2D_SceneBegin(target);

    if(Parameters->TVShowLib->koditvshowlib.size() == 0){
        C2D_TextBufClear(topmenudynbuffer);
        char buf[160];
        C2D_Text dynText;
        snprintf(buf, sizeof(buf), "Refresh Library First!");
        C2D_TextParse(&dynText, topmenudynbuffer, buf);
        C2D_DrawText(&dynText, 0, 150.0f, 100.0f, 0.5f, 0.5f, 0.5f);
        return;
    }


    if(showid == -1)return;
    C2D_TextBufClear(topmenudynbuffer);
    C2D_TextBufClear(topmenudynbuffer2);
    std::string wrappedplot = Parameters->KodiRPC->wrap(Parameters->TVShowLib->koditvshowlib[showid].plot.c_str(),60);
    char buf[160];
    char buf2[wrappedplot.size()];
    C2D_Text dynText;
    C2D_Text dynText2;
    snprintf(buf, sizeof(buf), "%s (%d)", Parameters->TVShowLib->koditvshowlib[showid].label.c_str(),Parameters->TVShowLib->koditvshowlib[showid].year);
    snprintf(buf2, sizeof(buf2), "%s",wrappedplot.c_str());
    C2D_TextParse(&dynText, topmenudynbuffer, buf);
    C2D_TextParse(&dynText2, topmenudynbuffer2, buf2);
    C2D_TextOptimize(&dynText);
    C2D_TextOptimize(&dynText2);

    u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
    C2D_DrawText(&dynText, C2D_WithColor, 8.0f, 10.0f, 0.5f, 0.5f, 0.5f,clrWhite);
    C2D_DrawText(&dynText2, C2D_WithColor, 120.0f, 30.0f, 0.5f, 0.35f, 0.35f,clrWhite);

    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x0F);

    float textw,texth,titlew,titleh;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&titlew,&titleh);
    C2D_TextGetDimensions(&dynText2,0.35f,0.35f,&textw,&texth);
    C2D_DrawRectSolid(6.0,8.0f,0.4f,titlew+2.0,titleh+2.0,clrBlack);
    C2D_DrawRectSolid(117.0f,28.0f,0.4f,textw+2.0,texth+2.0,clrBlack);

    if(Parameters->TVShowLib->thumbimage.thumbtext.tex != NULL){
        float scaleratio = (115.0-8.0)/(Parameters->TVShowLib->thumbimage.width*1.0);
        C2D_DrawImageAt(Parameters->TVShowLib->thumbimage.thumbtext, 8.0f, 30.0f, 0.5f, NULL, scaleratio, scaleratio);
    }
    if(Parameters->TVShowLib->fanartimage.thumbtext.tex != NULL){
        float scaleratio = (400.0)/(Parameters->TVShowLib->fanartimage.thumbtext.subtex->width*1.0);
        float heightstart =(240.0-Parameters->TVShowLib->fanartimage.thumbtext.subtex->height)/2.0;
        C2D_DrawImageAt(Parameters->TVShowLib->fanartimage.thumbtext, 0.0, heightstart, 0.3f, NULL, scaleratio, scaleratio);
    }

     C3D_FrameEnd(0);

}

void CConsoleMenu::TVShowEpisodeRender(C3D_RenderTarget* target,int episodenum){

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x00, 0x00, 0x00, 0xFF));
    C2D_SceneBegin(target);

    if(Parameters->TVShowLib->koditvshowlib.size() == 0){
        C2D_TextBufClear(topmenudynbuffer);
        char buf[160];
        C2D_Text dynText;
        snprintf(buf, sizeof(buf), "Refresh Library First!");
        C2D_TextParse(&dynText, topmenudynbuffer, buf);
        C2D_DrawText(&dynText, 0, 150.0f, 100.0f, 0.5f, 0.5f, 0.5f);
        return;
    }


    if(episodenum == -1)return;
    C2D_TextBufClear(topmenudynbuffer);
    C2D_TextBufClear(topmenudynbuffer2);
    std::string wrappedplot = Parameters->KodiRPC->wrap(Parameters->TVShowLib->koditvshowlib[tvshowlistpos].seasons[tvshowseasonpos].episodes[episodenum].plot.c_str(),60);
    char buf[160];
    char buf2[wrappedplot.size()];
    C2D_Text dynText;
    C2D_Text dynText2;
    snprintf(buf, sizeof(buf), "%s", Parameters->TVShowLib->koditvshowlib[tvshowlistpos].seasons[tvshowseasonpos].episodes[episodenum].label.c_str());
    snprintf(buf2, sizeof(buf2), "%s",wrappedplot.c_str());
    C2D_TextParse(&dynText, topmenudynbuffer, buf);
    C2D_TextParse(&dynText2, topmenudynbuffer2, buf2);
    C2D_TextOptimize(&dynText);
    C2D_TextOptimize(&dynText2);
    u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
    C2D_DrawText(&dynText, C2D_WithColor, 8.0f, 10.0f, 0.5f, 0.5f, 0.5f,clrWhite);
    C2D_DrawText(&dynText2, C2D_WithColor, 120.0f, 30.0f, 0.5f, 0.35f, 0.35f,clrWhite);

    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x0F);

    float textw,texth,titlew,titleh;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&titlew,&titleh);
    C2D_TextGetDimensions(&dynText2,0.35f,0.35f,&textw,&texth);
    C2D_DrawRectSolid(6.0,8.0f,0.4f,titlew+2.0,titleh+2.0,clrBlack);
    C2D_DrawRectSolid(117.0f,28.0f,0.4f,textw+2.0,texth+2.0,clrBlack);

    if(Parameters->TVShowLib->thumbimage.thumbtext.tex != NULL){
    float scaleratio = (115.0-8.0)/(Parameters->TVShowLib->thumbimage.width*1.0);
    C2D_DrawImageAt(Parameters->TVShowLib->thumbimage.thumbtext, 8.0f, 30.0f, 0.5f, NULL, scaleratio, scaleratio);
    }
    if(Parameters->TVShowLib->fanartimage.thumbtext.tex != NULL){
    float scaleratio = (400.0)/(Parameters->TVShowLib->fanartimage.thumbtext.subtex->width*1.0);
        float heightstart =(240.0-Parameters->TVShowLib->fanartimage.thumbtext.subtex->height)/2.0;
    C2D_DrawImageAt(Parameters->TVShowLib->fanartimage.thumbtext, 0.0, heightstart, 0.3f, NULL, scaleratio, scaleratio);
    }

     C3D_FrameEnd(0);

}


void CConsoleMenu::GUIMenuAudioLibrary(C3D_RenderTarget* target){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    std::string menutext;
    for(int i = audioartistpos;i<audioartistpos+10;i++){
        if((unsigned int)i<Parameters->AudioLib->artistslib.size()){
            menutext.append(Parameters->AudioLib->artistslib[i].name.c_str());
            menutext.append("\n",1);
        }

    }
    char buf[menutext.size()];
    C2D_Text dynText;
    snprintf(buf, sizeof(buf),"%s",menutext.c_str());
    C2D_TextParse(&dynText, menudynbuffer, buf);
    C2D_TextOptimize(&dynText);
    float startw,starth;
    float textw,texth;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&textw,&texth);
    startw = 8.0f;
    starth = 10.0f;
    C2D_DrawText(&dynText, 0, startw, starth, 0.5f, 0.5f, 0.5f);
    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);

    float incpos = 15.0f;

    C2D_DrawRectSolid(startw-1.0,starth-1.0+(incpos*0),0.6f,textw+1.0,15.0f,clrBlack);

    C3D_FrameEnd(0);



}

void CConsoleMenu::GUIMenuAudioAlbum(C3D_RenderTarget* target){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    std::string menutext;
    for(int i = audioalbumpos;i<audioalbumpos+10;i++){
        if((unsigned int)i<Parameters->AudioLib->artistslib[audioartistpos].albums.size()){
            menutext.append(Parameters->AudioLib->artistslib[audioartistpos].albums[i].title.c_str());
            char yearstring[30];
            sprintf(yearstring," (%d)",Parameters->AudioLib->artistslib[audioartistpos].albums[i].year);
            menutext.append(yearstring,strlen(yearstring));
            menutext.append("\n",1);
        }

    }
    char buf[menutext.size()];
    C2D_Text dynText;
    snprintf(buf, sizeof(buf),"%s",menutext.c_str());
    C2D_TextParse(&dynText, menudynbuffer, buf);
    C2D_TextOptimize(&dynText);
    float startw,starth;
    float textw,texth;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&textw,&texth);
    startw = 8.0f;
    starth = 10.0f;
    C2D_DrawText(&dynText, 0, startw, starth, 0.5f, 0.5f, 0.5f);
    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);

    float incpos = 15.0f;

    C2D_DrawRectSolid(startw-1.0,starth-1.0+(incpos*0),0.6f,textw+1.0,15.0f,clrBlack);

    C3D_FrameEnd(0);



}

void CConsoleMenu::GUIMenuAudioSong(C3D_RenderTarget* target){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    std::string menutext;
    for(int i = audiosongpos;i<audiosongpos+10;i++){
        if((unsigned int)i<Parameters->AudioLib->artistslib[audioartistpos].albums[audioalbumpos].songs.size()){
            char songstring[128];

            int minutes,seconds;
            minutes = Parameters->AudioLib->artistslib[audioartistpos].albums[audioalbumpos].songs[i].duration / 60;
            seconds = Parameters->AudioLib->artistslib[audioartistpos].albums[audioalbumpos].songs[i].duration % 60;

            sprintf(songstring,"%d %s %02d:%02d\n",Parameters->AudioLib->artistslib[audioartistpos].albums[audioalbumpos].songs[i].track,Parameters->AudioLib->artistslib[audioartistpos].albums[audioalbumpos].songs[i].title.c_str(),minutes,seconds);
            menutext.append(songstring,strlen(songstring));
        }

    }
    char buf[menutext.size()];
    C2D_Text dynText;
    snprintf(buf, sizeof(buf),"%s",menutext.c_str());
    C2D_TextParse(&dynText, menudynbuffer, buf);
    C2D_TextOptimize(&dynText);
    float startw,starth;
    float textw,texth;
    C2D_TextGetDimensions(&dynText,0.4f,0.4f,&textw,&texth);
    startw = 8.0f;
    starth = 10.0f;
    C2D_DrawText(&dynText, 0, startw, starth, 0.4f, 0.4f, 0.5f);
    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);

    float incpos = 15.0f;

    C2D_DrawRectSolid(startw-1.0,starth-1.0+(incpos*0),0.6f,textw+1.0,15.0f,clrBlack);

    C3D_FrameEnd(0);



}




void CConsoleMenu::SettingsMenu(C3D_RenderTarget* target){

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);



     C3D_FrameEnd(0);

}
