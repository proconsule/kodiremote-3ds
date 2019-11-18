//
//  Parameters.cpp
//  kodiclient-3ds
//
//  Created by proconsule on 20/10/2019.
//  Copyright © 2019 proconsule. All rights reserved.
//

#include "Parameters.hpp"

ssize_t CParameters::getline(char **lineptr, size_t *n, FILE *stream) {
    size_t pos;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = (char *)malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while(c != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = (char *)realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos ++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}


void CParameters::Save_Config(const char* configfile){
    FILE *fp;

        if((fp=fopen(configfile, "wb"))==NULL) {

        }

        fprintf(fp,"kodiaddress %s\n",kodiaddress);
		fprintf(fp,"kodiport %d\n",kodiport);
        fprintf(fp,"kodihttpport %d\n",kodihttpport);
        int bollconv = downloadimages;
        fprintf(fp,"downloadimages %d\n",bollconv);



        fclose(fp);



}


int CParameters::Read_Config(const char* configfile){
    FILE * fp;
       char * line = NULL;
       size_t len = 0;
       ssize_t read;
       char myopt[30];

       fp = fopen(configfile, "r");
       if (fp == NULL){

           return -1;
       }
       while ((read = getline(&line, &len, fp)) != -1) {

        sscanf(line,"%s %*s",myopt);


		if(strcmp(myopt,"kodiaddress") == 0){
           sscanf(line,"%*s %s",kodiaddress);
        }
		if(strcmp(myopt,"kodiport") == 0){
           sscanf(line,"%*s %d",&kodiport);
        }
        if(strcmp(myopt,"kodihttpport") == 0){
           sscanf(line,"%*s %d",&kodihttpport);
        }
        if(strcmp(myopt,"downloadimages") == 0){
            int boolconv;
            sscanf(line,"%*s %d",&boolconv);
            downloadimages = boolconv;
        }

       }

       fclose(fp);
       return 1;

}




void CParameters::Init(){


    kodiport = 9090;
    kodihttpport = 8080;
    sprintf(kodiaddress,"127.0.0.1");
    runThreads = true;
    downloadimages = true;
    KodiRPC = new CKodiRPC();
    KodiRPC->runThreads = &runThreads;
    KodiRPC->kodiaddress = kodiaddress;
    KodiRPC->kodihttpport = &kodihttpport;

    AudioLib = new CAudioLib();
    MovieLib = new CMovieLib();
    TVShowLib = new CTVShowLib();

    KodiRPC->AudioLib = AudioLib;
    KodiRPC->MovieLib = MovieLib;
    KodiRPC->TVShowLib = TVShowLib;


    AudioLib->kodiaddress = kodiaddress;
    AudioLib->kodihttpport = &kodihttpport;

    MovieLib->kodiaddress = kodiaddress;
    MovieLib->kodihttpport = &kodihttpport;

    TVShowLib->kodiaddress = kodiaddress;
    TVShowLib->kodihttpport = &kodihttpport;


    MovieLib->Init();
}


