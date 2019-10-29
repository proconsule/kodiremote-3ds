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

	if(menuid == AUDIOLIBRARYMENU){
		ret[0] = LIBRARYMENU;
		ret[1] = 0;
	}

	if(menuid == VIDEOLIBRARYMENU){
		ret[0] = LIBRARYMENU;
		ret[1] = 1;
	}
	if(menuid == VIDEOLIBRARYLISTMENU){
		ret[0] = VIDEOLIBRARYMENU;
		ret[1] = 0;
	}


	return ret;
}


void CConsoleMenu::Init(){

	menupos = 0;
	currmenuid = 0;

	movielistpos = 0;


	menupage_struct menu0;

	menu0.menutitle = "KodiRemote 3DS";
	menu0.menuid = MAINMENU;
	menu0.menudesc.push_back("Remote");
	menu0.havesubmenu.push_back(true);
	menu0.submenuid.push_back(REMOTEMENU);

	menu0.menudesc.push_back("Library");
	menu0.havesubmenu.push_back(true);
	menu0.submenuid.push_back(LIBRARYMENU);

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

	Menu.push_back(menu20);

	menupage_struct menu210;

	menu210.menutitle = "Audio Library (TODO)";
	menu210.menuid = AUDIOLIBRARYMENU;

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


	menupage_struct menu1220;

	menu1220.menutitle = "";
	menu1220.menuid = VIDEOLIBRARYLISTMENU;

	Menu.push_back(menu1220);


}

char * CConsoleMenu::PrintMenu(int menuid){
	 int menuidnum = 0;
	 for(unsigned int i=0;i<Menu.size();i++){
	    if(Menu[i].menuid == menuid){
			menuidnum = i;
		}
	 }
	 char * output = (char *)malloc(1024);
	 memset(output,0,1024);
	 sprintf(output,"%s\x1b[%d;%dH%s\n",output,VMENUPOS-1,7,Menu[menuidnum].menutitle.c_str());
	 for(unsigned int i=0;i<Menu[menuidnum].menudesc.size();i++){
		 sprintf(output,"%s\x1b[%d;%dH%s\n",output,VMENUPOS+i,HMENUPOS,Menu[menuidnum].menudesc[i].c_str());
		 if((unsigned int)menupos == i){
			 if(Menu[menuidnum].havesubmenu[i] == true){
				sprintf(output,"%s\x1b[%d;%dH>\n",output,VMENUPOS+i,HMENUPOS-2);
			 }else
			 {
				sprintf(output,"%s\x1b[%d;%dH*\n",output,VMENUPOS+i,HMENUPOS-2);
			 }
		 }
		 else
		 {
			 sprintf(output,"%s\x1b[%d;%dH \n",output,VMENUPOS+i,HMENUPOS-2);
		 }
	 }


	sprintf(output,"%s\x1b[%d;%dHKodi Version %d.%d %s\n",output,27,HMENUPOS,Parameters->KodiRPC->kodiversion.major,Parameters->KodiRPC->kodiversion.minor,Parameters->KodiRPC->kodiversion.tag);
	return output;
}

char * CConsoleMenu::PrintVideoLibrary(){
    char * output = (char *)malloc(1024);
    memset(output,0,1024);
    int x=0;
    //int i = movielistpos;
    for(int i = movielistpos;i<movielistpos+10;i++){
        if((unsigned int)i<Parameters->KodiRPC->kodivideolib.size()){
        sprintf(output,"%s\x1b[%d;%dH%s\n",output,VMENUPOS+x,HMENUPOS,Parameters->KodiRPC->kodivideolib[i].label.c_str());
        if(movielistpos == i){
            sprintf(output,"%s\x1b[%d;%dH*\n",output,VMENUPOS+x,HMENUPOS-2);
        }
        else
		{
			 sprintf(output,"%s\x1b[%d;%dH \n",output,VMENUPOS+x,HMENUPOS-2);
		}
		x++;
    }
    }

    sprintf(output,"%s\x1b[%d;%dHMovie Count:%d\n",output,27,HMENUPOS,Parameters->KodiRPC->kodivideolib.size());

    return output;
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

void CConsoleMenu::GUIVideoRefresh(C3D_RenderTarget* target,std::string label,int current,int max){

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    C2D_TextBufClear(menudynbuffer2);
    char buf[label.size()+1];
    char buf2[30];
    C2D_Text dynText;
    C2D_Text dynText2;
    snprintf(buf, sizeof(buf),"%s",label.c_str());
    float currperc = (current*1.0)*100/(max*1.0);

    snprintf(buf2, sizeof(buf2),"%.2f%%",currperc);

    C2D_TextParse(&dynText, menudynbuffer, buf);
    C2D_TextParse(&dynText2, menudynbuffer2, buf2);
    C2D_TextOptimize(&dynText);
    C2D_TextOptimize(&dynText2);
    float textw,texth;
    float perctextw,perctexth;
    C2D_TextGetDimensions(&dynText,0.5f,0.5f,&textw,&texth);
    C2D_TextGetDimensions(&dynText2,0.5f,0.5f,&perctextw,&perctexth);
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
    C3D_FrameEnd(0);


}


void CConsoleMenu::GUIStartMenu(C3D_RenderTarget* target,menu_struct *menu){



    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    if(menu->currmenuid == 0){
        menu->currmenumax = 5;
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


        /*
        printf("\x1b[1;7HKodi Connect Menu\n");
        printf("\x1b[2;3HKodi Address: %s\n",Parameters->kodiaddress);
        printf("\x1b[3;3HKodi port: %d\n",Parameters->kodiport);
        printf("\x1b[4;3HKodi HTTP port: %d\n",Parameters->kodihttpport);
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
         */

    }

}

void CConsoleMenu::GUIMenuVideoLibrary(C3D_RenderTarget* target){
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);
    C2D_TextBufClear(menudynbuffer);
    std::string menutext;
    for(int i = movielistpos;i<movielistpos+10;i++){
        if((unsigned int)i<Parameters->KodiRPC->kodivideolib.size()){
            menutext.append(Parameters->KodiRPC->kodivideolib[i].label.c_str());
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
    C2D_TargetClear(target, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
    C2D_SceneBegin(target);

    if(Parameters->KodiRPC->kodivideolib.size() == 0){
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
    std::string wrappedplot = Parameters->KodiRPC->wrap(Parameters->KodiRPC->kodivideolib[videoid].plot.c_str(),60);
    char buf[160];
    char buf2[wrappedplot.size()];
    C2D_Text dynText;
    C2D_Text dynText2;
    snprintf(buf, sizeof(buf), "%s (%d)", Parameters->KodiRPC->kodivideolib[videoid].label.c_str(),Parameters->KodiRPC->kodivideolib[videoid].year);
    snprintf(buf2, sizeof(buf2), "%s",wrappedplot.c_str());
    C2D_TextParse(&dynText, topmenudynbuffer, buf);
    C2D_TextParse(&dynText2, topmenudynbuffer2, buf2);
    C2D_TextOptimize(&dynText);
    C2D_TextOptimize(&dynText2);
    C2D_DrawText(&dynText, 0, 8.0f, 10.0f, 0.5f, 0.5f, 0.5f);
    C2D_DrawText(&dynText2, 0, 120.0f, 30.0f, 0.5f, 0.35f, 0.35f);

    u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0x3F);

    float textw,texth;
    C2D_TextGetDimensions(&dynText2,0.35f,0.35f,&textw,&texth);
    C2D_DrawRectSolid(117.0f,28.0f,0.6f,textw+2.0,texth+2.0,clrBlack);

    if(Parameters->KodiRPC->thumbimage.thumbtext.tex != NULL){
    float scaleratio = (115.0-8.0)/(Parameters->KodiRPC->thumbimage.width*1.0);
    C2D_DrawImageAt(Parameters->KodiRPC->thumbimage.thumbtext, 8.0f, 30.0f, 0.5f, NULL, scaleratio, scaleratio);
    }

     C3D_FrameEnd(0);

}

