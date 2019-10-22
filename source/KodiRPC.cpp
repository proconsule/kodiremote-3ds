//  Created by proconsule on 20/10/2019.
//  Copyright © 2019 proconsule. All rights reserved.
//

#include "KodiRPC.hpp"

void CKodiRPC::Init(){

}

struct json_object * find_something(struct json_object *jobj, const char *key) {
	struct json_object *tmp;

	json_object_object_get_ex(jobj, key, &tmp);

	return tmp;
}


void CKodiRPC::ParseJson(char* buffer){

    struct json_object *jobj;
    jobj = json_tokener_parse(buffer);
    /*
    printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
    */
    struct json_object *idobj =  find_something(jobj,"id");
    if(idobj == NULL){
        return;
    }
    enum json_type type;
    type = json_object_get_type(idobj);
    if(type == json_type_int){
        int id =  json_object_get_int(idobj);
        //printf("INT ID: %d\r\n",id);
        if(id == VOLUME){
            struct json_object *resobj =  find_something(jobj,"result");
            struct json_object *volobj =  find_something(resobj,"volume");
            //printf("VOL: %d\r\n",json_object_get_int(volobj));
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


    }

    if(type == json_type_string){
        char *stringid = (char *)json_object_get_string(idobj);
         printf("STRING ID: %s\r\n",stringid);
    }


}

char * CKodiRPC::CreateMessageInputKey(InputKeys Key){
    json_object * jobj = json_object_new_object();
    json_object *jstring = json_object_new_string("2.0");
    json_object *jid = json_object_new_int(INPUTKEY);
    json_object *jstringkey;
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



    return (char *)json_object_to_json_string(jobj);


}
