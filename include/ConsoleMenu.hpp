#ifndef CONSOLEMENU_H
#define CONSOLEMENU_H

#include <stdio.h>
#include <vector>
#include <cstring>
#include <string>
#include "Parameters.hpp"
#include "KodiRPC.hpp"

using namespace std;

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
    VIDEOLIBRARYMENU = 220,
    AUDIOLIBRARYMENU = 120,
    VIDEOLIBRARYLISTMENU = 1220



};


class CConsoleMenu{
	public:
	void Init();
	char * PrintMenu(int menuid);
    char * PrintVideoLibrary();
	void UpdateMenu(int menuid);
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

	CParameters *Parameters;
    CKodiRPC *KodiRPC;


};

#endif
