//  Created by proconsule on 20/10/2019.
//  Copyright � 2019 proconsule. All rights reserved.
//

#include "KodiRPC.hpp"

void CKodiRPC::Init(){
    currvideolibid = -1;
}

struct json_object * find_something(struct json_object *jobj, const char *key) {
	struct json_object *tmp;

	json_object_object_get_ex(jobj, key, &tmp);

	return tmp;
}

std::string urlEncode(std::string str){
    std::string new_str = "";
    char c;
    int ic;
    const char* chars = str.c_str();
    char bufHex[10];
    int len = strlen(chars);

    for(int i=0;i<len;i++){
        c = chars[i];
        ic = c;
        // uncomment this if you want to encode spaces with +
        /*if (c==' ') new_str += '+';
        else */if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') new_str += c;
        else {
            sprintf(bufHex,"%X",c);
            if(ic < 16)
                new_str += "%0";
            else
                new_str += "%";
            new_str += bufHex;
        }
    }
    return new_str;
 }

std::string urlDecode(std::string str){
    std::string ret;
    char ch;
    int i, ii, len = str.length();

    for (i=0; i < len; i++){
        if(str[i] != '%'){
            if(str[i] == '+')
                ret += ' ';
            else
                ret += str[i];
        }else{
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
    }
    return ret;
}

size_t CKodiRPC::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

void CKodiRPC::TurboJpegDecompress(MemoryStruct *source,ImageMemoryStruct *dest){



    int width, height, subSamp;
    tjhandle jpegHnd = tjInitDecompress();

     if (tjDecompressHeader2 (jpegHnd, (unsigned char *)source->memory, source->size, &width, &height, &subSamp) != 0) {


     }

     dest->memory = (char *)malloc(width*height*3);

     tjDecompress2(jpegHnd, (unsigned char *)source->memory, source->size, (unsigned char *)dest->memory, width, 0/*pitch*/, height, TJPF_RGB, TJFLAG_FASTDCT);
     dest->width = width;
     dest->height = height;
     dest->size = width*height*3;


}


void CKodiRPC::JpegDecompress(MemoryStruct *source,ImageMemoryStruct *dest){

    struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;



	int row_stride, width, height, pixel_size;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);


	jpeg_mem_src(&cinfo, (const unsigned char *)source->memory, source->size);


	int rc = jpeg_read_header(&cinfo, TRUE);

    if (rc != 1) {
     printf("Strange Header\n");
    }

	jpeg_start_decompress(&cinfo);

	width = cinfo.output_width;
	height = cinfo.output_height;
	pixel_size = cinfo.output_components;

    dest->size = width * height * pixel_size;
	dest->memory = (char*) malloc(dest->size);

    dest->width = width;
    dest->height = height;

    row_stride = width * pixel_size;


    while (cinfo.output_scanline < cinfo.output_height) {
		unsigned char *buffer_array[1];
		buffer_array[0] = (unsigned char *)dest->memory + \
						   (cinfo.output_scanline) * row_stride;

		jpeg_read_scanlines(&cinfo, buffer_array, 1);

	}

	jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);


}

void CKodiRPC::CreateThumbTexture(int movienum){
    if(kodivideolib[movienum].jpegref.size>0){
        if(thumbtext.tex != NULL){
            C3D_TexDelete(thumbtext.tex);
            linearFree((Tex3DS_SubTexture *)thumbtext.subtex);
        }
        struct ImageMemoryStruct dest;
        //JpegDecompress(&chunk,&dest);
        TurboJpegDecompress(&kodivideolib[movienum].jpegref,&dest);
        Draw_LoadImageMemory(&thumbtext,&dest);
        free(dest.memory);
    }
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


void CKodiRPC::DownloadMovieThumb(int movienum){

  CURL *curl_handle;
  CURLcode res;
   
  
  kodivideolib[movienum].jpegref.memory = (char *)malloc(1);
  kodivideolib[movienum].jpegref.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  curl_handle = curl_easy_init();

  curl_easy_setopt(curl_handle, CURLOPT_URL, kodivideolib[movienum].thumburl.c_str());

  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,CKodiRPC::WriteMemoryCallback);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&kodivideolib[movienum].jpegref);

  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  res = curl_easy_perform(curl_handle);

  if(res != CURLE_OK) {
    
  }
  else {

    /*
    if(thumbtext.tex != NULL){
        C3D_TexDelete(thumbtext.tex);
        linearFree((Tex3DS_SubTexture *)thumbtext.subtex);
    }
    struct ImageMemoryStruct dest;
    //JpegDecompress(&chunk,&dest);
    TurboJpegDecompress(&chunk,&dest);
    Draw_LoadImageMemory(&thumbtext,&dest);
    free(dest.memory);
 
    //printf("\x1b[24;1HJpeg Size: %lu W: %d H: %d", (unsigned long)dest.size,dest.width,dest.height);
     */
  }

  curl_easy_cleanup(curl_handle);

  curl_global_cleanup();




}

unsigned int Draw_GetNextPowerOf2(unsigned int v) {
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return (v >= 64 ? v : 64);
}

#define BYTES_PER_PIXEL 3
#define TRANSPARENT_COLOR 0xFFFFFFFF

static void Draw_C3DTexToC2DImage(C3D_Tex *tex, Tex3DS_SubTexture *subtex, u8 *buf, u32 size, u32 width, u32 height, GPU_TEXCOLOR format) {
	// RGBA -> ABGR
	for (u32 row = 0; row < width; row++) {
		for (u32 col = 0; col < height; col++) {
			u32 z = (row + col * width) * BYTES_PER_PIXEL;

			u8 r = *(u8 *)(buf + z);
			u8 g = *(u8 *)(buf + z + 1);
			u8 b = *(u8 *)(buf + z + 2);


			*(buf + z ) = b;
			*(buf + z + 1) = g;
			*(buf + z + 2) = r;
		}
	}

	u32 w_pow2 = Draw_GetNextPowerOf2(width);
	u32 h_pow2 = Draw_GetNextPowerOf2(height);

	subtex->width = (u16)width;
	subtex->height = (u16)height;
	subtex->left = 0.0f;
	subtex->top = 1.0f;
	subtex->right = (width / (float)w_pow2);
	subtex->bottom = 1.0 - (height / (float)h_pow2);

	C3D_TexInit(tex, (u16)w_pow2, (u16)h_pow2, format);
	C3D_TexSetFilter(tex, GPU_NEAREST, GPU_NEAREST);

	u32 pixel_size = (size / width / height);

	memset(tex->data, 0, tex->size);

	for (u32 x = 0; x < width; x++) {
		for (u32 y = 0; y < height; y++) {
			u32 dst_pos = ((((y >> 3) * (w_pow2 >> 3) + (x >> 3)) << 6) + ((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3))) * pixel_size;
			u32 src_pos = (y * width + x) * pixel_size;

			memcpy(&((u8*)tex->data)[dst_pos], &((u8*)buf)[src_pos], pixel_size);
		}
	}

	C3D_TexFlush(tex);

	tex->border = TRANSPARENT_COLOR;
	C3D_TexSetWrap(tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);
	linearFree(buf);
}


bool CKodiRPC::Draw_LoadImageMemory(C2D_Image *texture, ImageMemoryStruct *source) {


	//u8 *data = Draw_LoadExternalImageFile(path, &size);
	//image = stbi_load_from_memory((stbi_uc const *)data, (int)size, &width, &height, NULL, STBI_rgb_alpha);

	if (source->width > 1024 || source->height > 1024) {
		//stbi_image_free(image);
		return false;
	}

	C3D_Tex *tex = (C3D_Tex *)linearAlloc(sizeof(C3D_Tex));
	Tex3DS_SubTexture *subtex = (Tex3DS_SubTexture *)linearAlloc(sizeof(Tex3DS_SubTexture));
	Draw_C3DTexToC2DImage(tex, subtex, (u8 *)source->memory, (u32)(source->width * source->height * BYTES_PER_PIXEL), (u32)source->width, (u32)source->height, GPU_RGB8);
	texture->tex = tex;
	texture->subtex = subtex;
	//stbi_image_free(image);
	//free(source);
	return true;
}




bool CKodiRPC::ParseGetMovies(char *buffer){

    //printf("%s\n",buffer);
    struct json_object *jobj;
    jobj = json_tokener_parse(buffer);

    struct json_object *resobj;
    json_object_object_get_ex(jobj, "result", &resobj);

    struct json_object *limitobj;
    json_object_object_get_ex(resobj, "limits", &limitobj);

    struct json_object *totalobj;
    json_object_object_get_ex(limitobj, "total", &totalobj);

    struct json_object *startobj;
    json_object_object_get_ex(limitobj, "start", &startobj);


    int total = json_object_get_int(totalobj);
    int start = json_object_get_int(startobj);

    /*
    printf("%d %d\n",start,total);
    gspWaitForVBlank();
    gfxFlushBuffers();
    gfxSwapBuffers();
    */
    if(start ==total){

        //printf("End List\n");
        return true;
    }
    else
    {
        struct json_object *moviesobj;
        json_object_object_get_ex(resobj, "movies", &moviesobj);
        int nmovies = json_object_array_length(moviesobj);
        for(int i=0;i<nmovies;i++){

            struct json_object *movieobj;
            movieobj = json_object_array_get_idx(moviesobj, i);
            struct json_object *labelobj;
            struct json_object *plotobj;
            struct json_object *movieidobj;
            struct json_object *yearobj;
            struct json_object *thumbobj;
            json_object_object_get_ex(movieobj, "label", &labelobj);
            json_object_object_get_ex(movieobj, "plotoutline", &plotobj);
            json_object_object_get_ex(movieobj, "movieid", &movieidobj);
            json_object_object_get_ex(movieobj, "year", &yearobj);
            json_object_object_get_ex(movieobj, "thumbnail", &thumbobj);



            

            kodivideolib_struct kodimovie;
          
            char *labelstring = (char *)json_object_get_string(labelobj);
            char *plotstring = (char *)json_object_get_string(plotobj);
            char *thumbstring = (char *)json_object_get_string(thumbobj);



            kodimovie.label.assign(labelstring,strlen(labelstring));
            kodimovie.movieid = json_object_get_int(movieidobj);
            kodimovie.year = json_object_get_int(yearobj);
            kodimovie.plotoutline.assign(plotstring,strlen(plotstring));
            kodimovie.thumburl.assign(thumbstring,strlen(thumbstring));
            kodimovie.thumburl = urlEncode(kodimovie.thumburl);
            char kodiurl[50];
            memset(kodiurl,0,50);
            sprintf(kodiurl,"http://%s:%d/image/",kodiaddress,*kodihttpport);
            kodimovie.thumburl.insert(0,kodiurl);
            kodivideolib.push_back(kodimovie);
            
            
            

        }


    return false;
    }



}

void CKodiRPC::RequestMovieList(){

        std::vector<kodivideolib_struct>().swap(kodivideolib);
    
        char test[1024];
        sprintf(test,"{\"jsonrpc\": \"2.0\", \"method\": \"VideoLibrary.GetMovies\", \"params\": { \"limits\": { \"start\" : 0 } ,\"properties\" : [\"rating\", \"thumbnail\", \"playcount\",\"plotoutline\",\"year\"] ,\"sort\": { \"order\": \"ascending\", \"method\": \"label\", \"ignorearticle\": true } }, \"id\": \"libMovies\"}");
        send(*kodisock,test,strlen(test),0);
        sleep(1);
    for(unsigned int i=0;i<kodivideolib.size();i++){
        gspWaitForVBlank();
        gfxFlushBuffers();
        gfxSwapBuffers();
        printf("\x1b[6;0H");
        printf("\x1b[K");
        printf("\x1b[6;1H%s\n",kodivideolib[i].label.c_str());
        printf("\x1b[7;3HDownloading Cover %d\t/\t%d\n",i+1,kodivideolib.size());
        DownloadMovieThumb(i);
    }



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


    }

    if(type == json_type_string){
        char *stringid = (char *)json_object_get_string(idobj);
        if(strcmp(stringid,"libMovies") == 0){
           done =  ParseGetMovies(buffer);
        }
         //printf("STRING ID: %s\r\n",stringid);
    }


}

char * CKodiRPC::CreateMessageInputKey(InputKeys Key){
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



    return (char *)json_object_to_json_string(jobj);


}
