//  Created by proconsule on 20/10/2019.
//  Copyright © 2019 proconsule. All rights reserved.
//

#include "KodiRPC.hpp"

void CKodiRPC::Init(){
    currvideolibid = -1;
    playerid = -1;
    SocketFree = true;



}



void CKodiRPC::InitThread(s32 prio){

    thread = threadCreate((THREADFUNCPTR)&CKodiRPC::PlayerStatusThread, this, STACKSIZE, prio-1, -2, false);


}



void *CKodiRPC::PlayerStatusThread(void *arg){

    while(*runThreads){


        if((*currmenuid == 10 || *currmenuid == 0)&& SocketFree){

             char output[160];
             sprintf(output,"{\"jsonrpc\": \"2.0\", \"method\": \"Player.GetActivePlayers\", \"id\": %d}",QUERYPLAYER);
             send(*kodisock,output,strlen(output),0);
             usleep(200000);
             if(playerid >0){
                usleep(200000);
                char output2[512];
                sprintf(output2,"{\"jsonrpc\": \"2.0\", \"method\": \"Player.GetItem\", \"params\": { \"properties\": [\"title\", \"season\", \"episode\", \"duration\", \"showtitle\", \"tvshowid\", \"thumbnail\"], \"playerid\": %d }, \"id\": %d}",playerid,QUERYVIDEOPLAYERSTATUS);
                send(*kodisock,output2,strlen(output2),0);

             }

             if(playerid >0){
                usleep(200000);
                char output2[512];
                sprintf(output2,"{\"jsonrpc\": \"2.0\",\"id\": %d,\"method\": \"Player.GetProperties\",\"params\": {\"playerid\": %d,\"properties\": [\"speed\",\"position\",\"time\",\"playlistid\",\"totaltime\",\"percentage\"]}}",QUERYVIDEOPLAYERTIME,playerid);
                send(*kodisock,output2,strlen(output2),0);

             }

             if(playerid ==0){
                usleep(200000);
                char output2[512];
                sprintf(output2,"{\"jsonrpc\": \"2.0\",\"id\": %d,\"method\": \"Player.GetProperties\",\"params\": {\"playerid\": %d,\"properties\": [\"speed\",\"position\",\"time\",\"playlistid\",\"totaltime\",\"percentage\"]}}",QUERYAUDIOPLAYERTIME,playerid);
                send(*kodisock,output2,strlen(output2),0);

             }

             if(playerid ==0){
                usleep(200000);
                char output2[512];
                sprintf(output2,"{\"jsonrpc\": \"2.0\", \"method\": \"Player.GetItem\", \"params\": { \"properties\": [\"title\", \"album\", \"artist\", \"duration\", \"streamdetails\", \"year\"], \"playerid\": %d }, \"id\": %d}",playerid,QUERYAUDIOPLAYERSTATUS);
                send(*kodisock,output2,strlen(output2),0);

             }








        }
        usleep(400000);


    }


}

struct json_object * find_something(struct json_object *jobj, const char *key) {
	struct json_object *tmp;

	json_object_object_get_ex(jobj, key, &tmp);

	return tmp;
}



std::string CKodiRPC::wrap(const char *text, size_t line_length)
{
    std::istringstream words(text);
    std::ostringstream wrapped;
    std::string word;

    if (words >> word) {
        wrapped << word;
        size_t space_left = line_length - word.length();
        while (words >> word) {
            if (space_left < word.length() + 1) {
                wrapped << '\n' << word;
                space_left = line_length - word.length();
            } else {
                wrapped << ' ' << word;
                space_left -= word.length() + 1;
            }
        }
    }
    return wrapped.str();
}






void CKodiRPC::PlayerPlayPayse(){

    char output[160];
    sprintf(output,"{\"jsonrpc\": \"2.0\", \"method\": \"Player.PlayPause\", \"params\": { \"playerid\": 1 }, \"id\": %d}",PLAYER);
    SocketFree = false;
    send(*kodisock,output,strlen(output),0);
    SocketFree = true;

}

void CKodiRPC::PlayerStop(){

    char output[160];
    sprintf(output,"{\"jsonrpc\": \"2.0\", \"method\": \"Player.Stop\", \"params\": { \"playerid\": 1 }, \"id\": %d}",PLAYER);
    SocketFree = false;
    send(*kodisock,output,strlen(output),0);
    SocketFree = true;

}

void CKodiRPC::PlayerSeekFF(){
    char output[160];
    sprintf(output,"{\"jsonrpc\":\"2.0\", \"method\":\"Player.Seek\", \"params\": { \"playerid\":1, \"value\": \"smallforward\" }, \"id\":%d}",PLAYER);
    SocketFree = false;
    send(*kodisock,output,strlen(output),0);
    SocketFree = true;
}

void CKodiRPC::PlayerSeekRev(){
    char output[160];
    sprintf(output,"{\"jsonrpc\":\"2.0\", \"method\":\"Player.Seek\", \"params\": { \"playerid\":1, \"value\": \"smallbackward\" }, \"id\":%d}",PLAYER);
    SocketFree = false;
    send(*kodisock,output,strlen(output),0);
    SocketFree = true;
}






void CKodiRPC::ParseJson(char* buffer){

    struct json_object *jobj;
    jobj = json_tokener_parse(buffer);

    struct json_object *idobj =  find_something(jobj,"id");
    if(idobj == NULL){
        return;
    }
    enum json_type type;
    type = json_object_get_type(idobj);
    if(type == json_type_int){
        int id =  json_object_get_int(idobj);


        if(id == VOLUME){
            struct json_object *resobj =  find_something(jobj,"result");
            struct json_object *volobj =  find_something(resobj,"volume");

            volume = json_object_get_int(volobj);
        }
         if(id == VERSION){
             struct json_object *resobj =  find_something(jobj,"result");
             struct json_object *verobj =  find_something(resobj,"version");
             struct json_object *majorobj =  find_something(verobj,"major");
             struct json_object *minorobj =  find_something(verobj,"minor");
             struct json_object *revisionobj =  find_something(verobj,"revision");
             struct json_object *tagobj =  find_something(verobj,"tag");

             kodiversion.major = json_object_get_int(majorobj);
             kodiversion.minor = json_object_get_int(minorobj);
             kodiversion.revision = (char *)json_object_get_string(revisionobj);
             kodiversion.tag = (char *)json_object_get_string(tagobj);
             printf("Kodi Version %d.%d %s %s\r\n",kodiversion.major,kodiversion.minor,kodiversion.revision,kodiversion.tag);
         }

         if(id == QUERYPLAYER){
             struct json_object *resobj =  find_something(jobj,"result");
             int numplayers = json_object_array_length(resobj);
             if(numplayers<1){
                 playerid = -1;
             }
             else
             {
                 struct json_object *playerobj = json_object_array_get_idx(resobj, 0);
                 struct json_object *playeridobj;
                 json_object_object_get_ex(playerobj, "playerid", &playeridobj);
                 playerid = json_object_get_int(playeridobj);

             }


         }
         if(id == QUERYVIDEOPLAYERSTATUS){


            //memset(&player_viseostatus, 0, sizeof(player_viseostatus));

            struct json_object *resobj =  find_something(jobj,"result");
            struct json_object *itemobj;
            json_object_object_get_ex(resobj, "item", &itemobj);
            struct json_object *typeobj;
            json_object_object_get_ex(itemobj, "type", &typeobj);

            struct json_object *episodeobj;
            json_object_object_get_ex(itemobj, "episode", &episodeobj);
            struct json_object *seasonobj;
            json_object_object_get_ex(itemobj, "season", &seasonobj);
            struct json_object *tvshowidobj;
            json_object_object_get_ex(itemobj, "tvshowid", &tvshowidobj);
            struct json_object *idobj;
            json_object_object_get_ex(itemobj, "id", &idobj);
            struct json_object *showtitleobj;
            json_object_object_get_ex(itemobj, "showtitle", &showtitleobj);
            struct json_object *labelobj;
            json_object_object_get_ex(itemobj, "label", &labelobj);


            player_viseostatus.type = (char *)json_object_get_string(typeobj);

            player_viseostatus.tvshowid = json_object_get_int(tvshowidobj);
            player_viseostatus.season = json_object_get_int(seasonobj);
            player_viseostatus.episode = json_object_get_int(episodeobj);
            player_viseostatus.id = json_object_get_int(idobj);
            player_viseostatus.label = (char *)json_object_get_string(labelobj);
            player_viseostatus.showtitle = (char *)json_object_get_string(showtitleobj);



         }
         if(id == QUERYAUDIOPLAYERSTATUS){


            //memset(&player_viseostatus, 0, sizeof(player_viseostatus));

            struct json_object *resobj =  find_something(jobj,"result");
            struct json_object *itemobj;
            json_object_object_get_ex(resobj, "item", &itemobj);
            struct json_object *typeobj;
            json_object_object_get_ex(itemobj, "type", &typeobj);

            struct json_object *titleobj;
            json_object_object_get_ex(itemobj, "title", &titleobj);

            struct json_object *yearobj;
            json_object_object_get_ex(itemobj, "year", &yearobj);

            struct json_object *artistsobj;
            json_object_object_get_ex(itemobj, "artist", &artistsobj);
            struct json_object *artistobj;


            artistobj = json_object_array_get_idx(artistsobj, 0);

            struct json_object *albumobj;
            json_object_object_get_ex(itemobj, "album", &albumobj);




            player_audiostatus.type = (char *)json_object_get_string(typeobj);
            player_audiostatus.album = (char *)json_object_get_string(albumobj);
            player_audiostatus.title = (char *)json_object_get_string(titleobj);
            player_audiostatus.artist = (char *)json_object_get_string(artistobj);
            player_audiostatus.year = json_object_get_int(yearobj);





         }
         if(id == QUERYVIDEOPLAYERTIME){
             //player_audiostatus.totaltime.hour = -1;
             struct json_object *resobj =  find_something(jobj,"result");
             struct json_object *timeobj;
             struct json_object *totaltimeobj;
             struct json_object *timehourobj;
             struct json_object *timeminutesobj;
             struct json_object *timesecondobj;
             struct json_object *timemsecobj;
             struct json_object *totaltimehourobj;
             struct json_object *totaltimeminutesobj;
             struct json_object *totaltimesecondobj;
             struct json_object *totaltimemsecobj;
             struct json_object *percobj;


             json_object_object_get_ex(resobj, "time", &timeobj);
             json_object_object_get_ex(resobj, "totaltime", &totaltimeobj);

             json_object_object_get_ex(timeobj, "hours", &timehourobj);
             json_object_object_get_ex(timeobj, "minutes", &timeminutesobj);
             json_object_object_get_ex(timeobj, "seconds", &timesecondobj);
             json_object_object_get_ex(timeobj, "milliseconds", &timemsecobj);

             json_object_object_get_ex(totaltimeobj, "hours", &totaltimehourobj);
             json_object_object_get_ex(totaltimeobj, "minutes", &totaltimeminutesobj);
             json_object_object_get_ex(totaltimeobj, "seconds", &totaltimesecondobj);
             json_object_object_get_ex(totaltimeobj, "milliseconds", &totaltimemsecobj);
             json_object_object_get_ex(resobj, "percentage", &percobj);


             player_viseostatus.currenttime.hour = json_object_get_int(timehourobj);
             player_viseostatus.currenttime.minutes = json_object_get_int(timeminutesobj);
             player_viseostatus.currenttime.seconds = json_object_get_int(timesecondobj);
             player_viseostatus.currenttime.millisec = json_object_get_int(timemsecobj);

             player_viseostatus.totaltime.hour = json_object_get_int(totaltimehourobj);
             player_viseostatus.totaltime.minutes = json_object_get_int(totaltimeminutesobj);
             player_viseostatus.totaltime.seconds = json_object_get_int(totaltimesecondobj);
             player_viseostatus.totaltime.millisec = json_object_get_int(totaltimemsecobj);
             player_viseostatus.perc = json_object_get_double(percobj);

         }
         if(id == QUERYAUDIOPLAYERTIME){

             struct json_object *resobj =  find_something(jobj,"result");
             struct json_object *timeobj;
             struct json_object *totaltimeobj;
             struct json_object *timehourobj;
             struct json_object *timeminutesobj;
             struct json_object *timesecondobj;
             struct json_object *timemsecobj;
             struct json_object *totaltimehourobj;
             struct json_object *totaltimeminutesobj;
             struct json_object *totaltimesecondobj;
             struct json_object *totaltimemsecobj;
             struct json_object *percobj;


             json_object_object_get_ex(resobj, "time", &timeobj);
             json_object_object_get_ex(resobj, "totaltime", &totaltimeobj);

             json_object_object_get_ex(timeobj, "hours", &timehourobj);
             json_object_object_get_ex(timeobj, "minutes", &timeminutesobj);
             json_object_object_get_ex(timeobj, "seconds", &timesecondobj);
             json_object_object_get_ex(timeobj, "milliseconds", &timemsecobj);

             json_object_object_get_ex(totaltimeobj, "hours", &totaltimehourobj);
             json_object_object_get_ex(totaltimeobj, "minutes", &totaltimeminutesobj);
             json_object_object_get_ex(totaltimeobj, "seconds", &totaltimesecondobj);
             json_object_object_get_ex(totaltimeobj, "milliseconds", &totaltimemsecobj);
             json_object_object_get_ex(resobj, "percentage", &percobj);


             player_audiostatus.currenttime.hour = json_object_get_int(timehourobj);
             player_audiostatus.currenttime.minutes = json_object_get_int(timeminutesobj);
             player_audiostatus.currenttime.seconds = json_object_get_int(timesecondobj);
             player_audiostatus.currenttime.millisec = json_object_get_int(timemsecobj);

             player_audiostatus.totaltime.hour = json_object_get_int(totaltimehourobj);
             player_audiostatus.totaltime.minutes = json_object_get_int(totaltimeminutesobj);
             player_audiostatus.totaltime.seconds = json_object_get_int(totaltimesecondobj);
             player_audiostatus.totaltime.millisec = json_object_get_int(totaltimemsecobj);
             player_audiostatus.perc = json_object_get_double(percobj);



         }


    }

    if(type == json_type_string){
        char *stringid = (char *)json_object_get_string(idobj);

        if(strcmp(stringid,"libSongs") == 0){
           done = AudioLib->ParseAudio(buffer);
        }
        if(strcmp(stringid,"libMovies") == 0){
           done =  MovieLib->ParseGetMovies(buffer);
        }
        if(strcmp(stringid,"libTvShows") == 0){
           done =  TVShowLib->ParseGetTVShow(buffer);
        }
        if(strcmp(stringid,"libTvShowsEpisodes") == 0){
           done =  TVShowLib->ParseGetTVShowEpisodes(buffer);
        }

    }




}

void CKodiRPC::CreateMessageInputKey(InputKeys Key){
    json_object * jobj = json_object_new_object();
    json_object *jstring = json_object_new_string("2.0");
    json_object *jid = json_object_new_int(INPUTKEY);
    json_object *jstringkey = NULL;
    if(Key == KODI_KEYUP){
        jstringkey = json_object_new_string("Input.Up");
    }
    if(Key == KODI_KEYDOWN){
        jstringkey = json_object_new_string("Input.Down");
    }
    if(Key == KODI_KEYLEFT){
        jstringkey = json_object_new_string("Input.Left");
    }
    if(Key == KODI_KEYRIGHT){
        jstringkey = json_object_new_string("Input.Right");
    }
    if(Key == KODI_KEYBACK){
        jstringkey = json_object_new_string("Input.Back");
    }
    if(Key == KODI_KEYSELECT){
        jstringkey = json_object_new_string("Input.Select");
    }
    if(Key == KODI_KEYCONTEXT){
        jstringkey = json_object_new_string("Input.ContextMenu");
    }
    if(Key == KODI_KEYINFO){
        jstringkey = json_object_new_string("Input.Info");
    }
    if(Key == KODI_KEYHOME){
        jstringkey = json_object_new_string("Input.Home");
    }
    if(Key == KODI_KEYOSD){
        jstringkey = json_object_new_string("Input.ShowOSD");
    }
    if(Key == KODI_KEYSHOWCODEC){
        jstringkey = json_object_new_string("Input.ShowCodec");
    }




    json_object_object_add(jobj,"jsonrpc", jstring);
    json_object_object_add(jobj,"id", jid);
    json_object_object_add(jobj,"method", jstringkey);


    char *output = (char *)json_object_to_json_string(jobj);
    SocketFree = false;
    send(*kodisock,output,strlen(output),0);
    SocketFree = true;

}





