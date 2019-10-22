//
//  KodiSocket.cpp
//  kodiclient-3ds
//
//  Created by proconsule on 20/10/2019.
//  Copyright © 2019 proconsule. All rights reserved.
//

#include "KodiSocket.hpp"

typedef void  (*THREADFUNCPTR)(void *);


void CKodiSocket::Init(s32 prio){

    thread = threadCreate((THREADFUNCPTR)&CKodiSocket::SockThread, this, STACKSIZE, prio-1, -2, false);


}




void *CKodiSocket::SockThread(void *arg){

     char server_reply[2000];
     struct sockaddr_in server;
     sock = socket(AF_INET , SOCK_STREAM , 0);
     if (sock == -1)
     {
         //printf("Could not create socket");
     }


     server.sin_addr.s_addr = inet_addr(Parameters->kodiaddress);
     server.sin_family = AF_INET;
     server.sin_port = htons( Parameters->kodiport );

     if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
     {
         printf("Connect Error\n");
         return 0;
     }

     fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);

     Parameters->connected = 1;
     consoleSelect(&Parameters->topScreen);
     printf("Connected\r\n");

    char vercmd[] = "{\"jsonrpc\": \"2.0\", \"method\": \"Application.GetProperties\", \"params\": {\"properties\": [\"version\"]}, \"id\": 102}\r\n";

    send(sock,vercmd,strlen(vercmd),0);
     while(Parameters->runThreads){


         if(recv(sock,server_reply,2000,0) <0){
             //rxconfig.maxfreq = 2;
             usleep(1000);
         }else{

             //printf("%s\r\n",server_reply);
             consoleSelect(&Parameters->topScreen);
             Parameters->KodiRPC->ParseJson(server_reply);

         }

     }

     Parameters->connected = 0;
     close(sock);
     return 0;


}

