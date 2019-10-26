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
	 return Menu[menuidnum].havesubmenu[pos];

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

void CConsoleMenu::UpdateMenu(int menuid){
	int menuidnum;
	 for(unsigned int i=0;i<Menu.size();i++){
	    if(Menu[i].menuid == menuid){
			menuidnum = i;
		}
	 }





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


	menu20.menudesc.push_back("Video");
	menu20.havesubmenu.push_back(true);
	menu20.submenuid.push_back(VIDEOLIBRARYMENU);

	Menu.push_back(menu20);

	menupage_struct menu210;

	menu210.menutitle = "Audio Library";
	menu210.menuid = AUDIOLIBRARYMENU;

	Menu.push_back(menu210);

	menupage_struct menu220;

	menu220.menutitle = "Video Library";
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

