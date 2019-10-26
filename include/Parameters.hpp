//
//  Parameters.hpp
//  kodiclient-3ds
//
//  Created by proconsule on 20/10/2019.
//  Copyright © 2019 proconsule. All rights reserved.
//

#ifndef Parameters_hpp
#define Parameters_hpp

#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <3ds.h>
#include <json-c/json.h>
#include "KodiRPC.hpp"




class CParameters{
public:

    void Init();
    void Save_Config(const char* configfile);
	int Read_Config(const char* configfile);
	ssize_t getline(char **lineptr, size_t *n, FILE *stream);

    int kodiport;
    int kodihttpport;
    char kodiaddress[40];

    int connected;
    bool runThreads;



    PrintConsole topScreen, bottomScreen;

    CKodiRPC *KodiRPC;



};





#endif /* Parameters_hpp */
