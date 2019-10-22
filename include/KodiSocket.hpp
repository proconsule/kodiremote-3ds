//
//  KodiSocket.hpp
//  kodiclient-3ds
//
//  Created by proconsule on 20/10/2019.
//  Copyright © 2019 proconsule. All rights reserved.
//

#ifndef KodiSocket_hpp
#define KodiSocket_hpp

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

#define STACKSIZE (10 * 1024)

class CKodiSocket{
public:
    void Init(s32 prio);
    void *SockThread(void *arg);
    int sock;
    Thread thread;

    CParameters * Parameters;

};


#endif /* KodiSocket_hpp */
