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

     char *server_msg = NULL;
     char *lastbuf = (char *)malloc(10);
     int mysize = 0;
     struct sockaddr_in server;
     sock = socket(AF_INET , SOCK_STREAM , 0);
     if (sock == -1)
     {

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


    char vercmd[] = "{\"jsonrpc\": \"2.0\", \"method\": \"Application.GetProperties\", \"params\": {\"properties\": [\"version\"]}, \"id\": 102}\r\n";

    send(sock,vercmd,strlen(vercmd),0);
     while(Parameters->runThreads){
         int n_bytes = recv(sock,lastbuf,10,0);

         if(n_bytes >0 ){

             mysize += n_bytes;
             if(n_bytes <10){


                server_msg = (char *)realloc(server_msg,mysize);
                memcpy(&server_msg[mysize-n_bytes],lastbuf,n_bytes);
                Parameters->KodiRPC->ParseJson(server_msg);

                mysize = 0;


             }
             else
             {
                if(server_msg==NULL)server_msg=(char *)malloc(10);
                server_msg = (char *)realloc(server_msg,mysize);
                memcpy(&server_msg[mysize-10],lastbuf,10);
             }
             memset(lastbuf,0,10);
         }
         else{
             int error = errno;
             switch (error)
                {
                    //case EAGAIN:
                    case EWOULDBLOCK:
                    // Socket is O_NONBLOCK and there is no data available
                    break;
                    case EINTR:
                    // an interrupt (signal) has been catched
                    // should be ingore in most cases
                    break;
                    default:
                    Parameters->connected = 0;
                    close(sock);
                    return 0;
                    // socket has an error, no valid anymore

                }

         }

     }

     Parameters->connected = 0;
     close(sock);
     return 0;


}

