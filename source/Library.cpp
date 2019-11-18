#include "Library.hpp"
#include "KodiRPC.hpp"


bool Resample(ImageMemoryStruct  * source,ImageMemoryStruct  * dest, int newWidth, int newHeight)
    {
        if(source->memory == NULL) return false;

        dest->memory = new char [newWidth * newHeight * 3];

        double scaleWidth =  (double)newWidth / (double)source->width;
        double scaleHeight = (double)newHeight / (double)source->height;

        for(int cy = 0; cy < newHeight; cy++)
        {
            for(int cx = 0; cx < newWidth; cx++)
            {
                int pixel = (cy * (newWidth *3)) + (cx*3);
                int nearestMatch =  (((int)(cy / scaleHeight) * (source->width *3)) + ((int)(cx / scaleWidth) *3) );

                dest->memory[pixel    ] =  source->memory[nearestMatch    ];
                dest->memory[pixel + 1] =  source->memory[nearestMatch + 1];
                dest->memory[pixel + 2] =  source->memory[nearestMatch + 2];
            }
        }

        dest->width = newWidth;
        dest->height = newHeight;


        return true;
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

        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') new_str += c;
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



void CMovieLib::Init(){


    curl_global_init(CURL_GLOBAL_ALL);

}


void TurboJpegCompress(ImageMemoryStruct *source,MemoryStruct *dest){

    tjhandle _jpegCompressor = tjInitCompress();
    int JPEG_QUALITY = 75;

    tjCompress2(_jpegCompressor, (unsigned char *)source->memory, source->width, 0, source->height, TJPF_RGB,
          (unsigned char **)&dest->memory, (long unsigned int *)&dest->size, TJSAMP_444, JPEG_QUALITY,
          TJFLAG_FASTDCT);

    tjDestroy(_jpegCompressor);

}

bool TurboJpegDecompress(MemoryStruct *source,ImageMemoryStruct *dest){



    int width, height, subSamp;
    tjhandle jpegHnd = tjInitDecompress();

     if (tjDecompressHeader2 (jpegHnd, (unsigned char *)source->memory, source->size, &width, &height, &subSamp) != 0) {

         return false;
     }

     dest->memory = (char *)malloc(width*height*3);

     tjDecompress2(jpegHnd, (unsigned char *)source->memory, source->size, (unsigned char *)dest->memory, width, 0/*pitch*/, height, TJPF_RGB, TJFLAG_FASTDCT);
     dest->width = width;
     dest->height = height;
     dest->size = width*height*3;

    tjDestroy(jpegHnd);
    return true;

}

void CMovieLib::CreateFanArtTexture(int movienum){
    if(kodivideolib[movienum].fanartjpegref.size>0){
        if(fanartimage.thumbtext.tex != NULL){
            C3D_TexDelete(fanartimage.thumbtext.tex);
            linearFree((Tex3DS_SubTexture *)fanartimage.thumbtext.subtex);
        }
        struct ImageMemoryStruct dest;


        bool success = TurboJpegDecompress(&kodivideolib[movienum].fanartjpegref,&dest);
        if(success == false){
            return;
        }
        fanartimage.width = dest.width;
        fanartimage.height = dest.height;

        Draw_LoadImageMemory(&fanartimage.thumbtext,&dest);
        free(dest.memory);
    }
    else{

            C3D_TexDelete(fanartimage.thumbtext.tex);
            linearFree((Tex3DS_SubTexture *)fanartimage.thumbtext.subtex);

    }
}

void CMovieLib::CreateThumbTexture(int movienum){
    if(kodivideolib[movienum].jpegref.size>0){
        if(thumbimage.thumbtext.tex != NULL){
            C3D_TexDelete(thumbimage.thumbtext.tex);
            linearFree((Tex3DS_SubTexture *)thumbimage.thumbtext.subtex);
        }
        struct ImageMemoryStruct dest;

        bool success = TurboJpegDecompress(&kodivideolib[movienum].jpegref,&dest);
        if(success == false){
            return;
        }
        thumbimage.width = dest.width;
        thumbimage.height = dest.height;


        Draw_LoadImageMemory(&thumbimage.thumbtext,&dest);
        free(dest.memory);
    }
    else{

            C3D_TexDelete(thumbimage.thumbtext.tex);
            linearFree((Tex3DS_SubTexture *)thumbimage.thumbtext.subtex);

    }
}

size_t CMovieLib::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */

    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

void CMovieLib::DownloadThumb(int movienum){

  if(kodivideolib[movienum].thumburl.compare("localthumb")== 0){

        FILE *f = fopen("romfs:/localthumb.jpg", "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        kodivideolib[movienum].jpegref.memory = (char *)malloc(fsize + 1);
        fread(kodivideolib[movienum].jpegref.memory, 1, fsize, f);
        kodivideolib[movienum].jpegref.size = fsize;
        fclose(f);
        return;
    }

  CURL *curl_handle;
  CURLcode res;


  kodivideolib[movienum].jpegref.memory = (char *)malloc(1);
  kodivideolib[movienum].jpegref.size = 0;

  //curl_global_init(CURL_GLOBAL_ALL);

  curl_handle = curl_easy_init();

  curl_easy_setopt(curl_handle, CURLOPT_URL, kodivideolib[movienum].thumburl.c_str());

  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,CMovieLib::WriteMemoryCallback);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&kodivideolib[movienum].jpegref);

  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  res = curl_easy_perform(curl_handle);

  if(res != CURLE_OK) {

  }
  else {


  }



  curl_easy_cleanup(curl_handle);


}


void CMovieLib::DownloadFanArt(int movienum){

  if(kodivideolib[movienum].fanarturl.compare("localfanart") == 0){

        FILE *f = fopen("romfs:/localfanart.jpg", "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        kodivideolib[movienum].fanartjpegref.memory = (char *)malloc(fsize + 1);
        fread(kodivideolib[movienum].fanartjpegref.memory, 1, fsize, f);
        kodivideolib[movienum].fanartjpegref.size = fsize;
        fclose(f);
        return;
    }

  CURL *curl_handle;
  CURLcode res;



  kodivideolib[movienum].fanartjpegref.memory = (char *)malloc(1);
  kodivideolib[movienum].fanartjpegref.size = 0;

  curl_handle = curl_easy_init();

  curl_easy_setopt(curl_handle, CURLOPT_URL, kodivideolib[movienum].fanarturl.c_str());

  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,CMovieLib::WriteMemoryCallback);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&kodivideolib[movienum].fanartjpegref);

  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  res = curl_easy_perform(curl_handle);

  if(res != CURLE_OK) {

  }
  else {


      if(kodivideolib[movienum].fanartjpegref.size >0){
      struct ImageMemoryStruct dest;
      struct ImageMemoryStruct resampledest;

      TurboJpegDecompress(&kodivideolib[movienum].fanartjpegref,&dest);

      double scale = (400.0/dest.width*1.0);
      int neww = dest.width*scale;
      int newh = dest.height*scale;
      Resample(&dest,&resampledest,neww,newh);

      TurboJpegCompress(&resampledest,&kodivideolib[movienum].fanartjpegref);

      free(dest.memory);
      free(resampledest.memory);

      }

  }


    curl_easy_cleanup(curl_handle);

}

void CMovieLib::RequestMovieList(){

        std::vector<kodivideolib_struct>().swap(kodivideolib);

        char test[1024];
        sprintf(test,"{\"jsonrpc\": \"2.0\", \"method\": \"VideoLibrary.GetMovies\", \"params\": { \"limits\": { \"start\" : 0 } ,\"properties\" : [\"rating\", \"thumbnail\", \"fanart\", \"playcount\", \"plot\", \"year\"] ,\"sort\": { \"order\": \"ascending\", \"method\": \"label\", \"ignorearticle\": true } }, \"id\": \"libMovies\"}");
        send(*kodisock,test,strlen(test),0);





}

void CMovieLib::PlayMovie(int movieid){

    char output[160];
    sprintf(output,"{\"jsonrpc\":\"2.0\", \"method\":\"Player.Open\", \"id\":%d,\"params\":{\"item\":{\"movieid\":%d}}}",PLAYMOVIE,movieid);
    send(*kodisock,output,strlen(output),0);

}

bool CMovieLib::Draw_LoadImageMemory(C2D_Image *texture, ImageMemoryStruct *source) {



	if (source->width > 1024 ) {
        return false;

	}

	C3D_Tex *tex = (C3D_Tex *)linearAlloc(sizeof(C3D_Tex));
	Tex3DS_SubTexture *subtex = (Tex3DS_SubTexture *)linearAlloc(sizeof(Tex3DS_SubTexture));
	Draw_C3DTexToC2DImage(tex, subtex, (u8 *)source->memory, (u32)(source->width * source->height * BYTES_PER_PIXEL), (u32)source->width, (u32)source->height, GPU_RGB8);
	texture->tex = tex;
	texture->subtex = subtex;

	return true;
}

bool CMovieLib::ParseGetMovies(char *buffer){

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


    if(start ==total){


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
            struct json_object *fanartobj;
            json_object_object_get_ex(movieobj, "label", &labelobj);
            json_object_object_get_ex(movieobj, "plot", &plotobj);
            json_object_object_get_ex(movieobj, "movieid", &movieidobj);
            json_object_object_get_ex(movieobj, "year", &yearobj);
            json_object_object_get_ex(movieobj, "thumbnail", &thumbobj);
            json_object_object_get_ex(movieobj, "fanart", &fanartobj);





            kodivideolib_struct kodimovie;

            char *labelstring = (char *)json_object_get_string(labelobj);
            char *plotstring = (char *)json_object_get_string(plotobj);
            char *thumbstring = (char *)json_object_get_string(thumbobj);
            char *fanartstring = (char *)json_object_get_string(fanartobj);




            kodimovie.label.assign(labelstring,strlen(labelstring));
            kodimovie.movieid = json_object_get_int(movieidobj);
            kodimovie.year = json_object_get_int(yearobj);
            kodimovie.plot.assign(plotstring,strlen(plotstring));
            kodimovie.thumburl.assign(thumbstring,strlen(thumbstring));
            kodimovie.thumburl = urlEncode(kodimovie.thumburl);
            kodimovie.fanarturl.assign(fanartstring,strlen(fanartstring));
            kodimovie.fanarturl = urlEncode(kodimovie.fanarturl);
            char kodiurl[50];
            memset(kodiurl,0,50);
            sprintf(kodiurl,"http://%s:%d/image/",kodiaddress,*kodihttpport);
            if(kodimovie.thumburl.length()<10){
                kodimovie.thumburl.assign("localthumb",strlen("localthumb"));


            }else{
            kodimovie.thumburl.insert(0,kodiurl);
            }
            if(kodimovie.fanarturl.length()<10){

                kodimovie.fanarturl.assign("localfanart",strlen("localfanart"));


            }else{
            kodimovie.fanarturl.insert(0,kodiurl);
            }
            kodivideolib.push_back(kodimovie);




        }


    return false;
    }



}

void CTVShowLib::Init(){


}

void CTVShowLib::RequestTVShowList(){

        std::vector<koditvshowlib_struct>().swap(koditvshowlib);

        char test[1024];
        sprintf(test,"{\"jsonrpc\": \"2.0\", \"method\": \"VideoLibrary.GetTVShows\", \"params\": { \"limits\": { \"start\" : 0 }, \"properties\": [\"plot\", \"title\", \"originaltitle\", \"year\", \"rating\", \"thumbnail\",\"fanart\", \"playcount\"], \"sort\": { \"order\": \"ascending\", \"method\": \"label\" } }, \"id\": \"libTvShows\"}");
        send(*kodisock,test,strlen(test),0);




}

size_t CTVShowLib::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
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

void CTVShowLib::DownloadThumb(int shownum){





  CURL *curl_handle;
  CURLcode res;


  koditvshowlib[shownum].jpegref.memory = (char *)malloc(1);
  koditvshowlib[shownum].jpegref.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  curl_handle = curl_easy_init();

  curl_easy_setopt(curl_handle, CURLOPT_URL, koditvshowlib[shownum].thumburl.c_str());

  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,CTVShowLib::WriteMemoryCallback);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&koditvshowlib[shownum].jpegref);

  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  res = curl_easy_perform(curl_handle);

  if(res != CURLE_OK) {

  }
  else {


  }

  curl_easy_cleanup(curl_handle);

  curl_global_cleanup();




}

void CTVShowLib::DownloadFanArt(int shownum){



  CURL *curl_handle;
  CURLcode res;


  koditvshowlib[shownum].fanartjpegref.memory = (char *)malloc(1);
  koditvshowlib[shownum].fanartjpegref.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  curl_handle = curl_easy_init();

  curl_easy_setopt(curl_handle, CURLOPT_URL, koditvshowlib[shownum].fanarturl.c_str());

  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,CTVShowLib::WriteMemoryCallback);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&koditvshowlib[shownum].fanartjpegref);

  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  res = curl_easy_perform(curl_handle);

  if(res != CURLE_OK) {

  }
  else {
      if(koditvshowlib[shownum].fanartjpegref.size >0){
      struct ImageMemoryStruct dest;
      struct ImageMemoryStruct resampledest;
      TurboJpegDecompress(&koditvshowlib[shownum].fanartjpegref,&dest);

      double scale = (400.0/dest.width*1.0);
      int neww = dest.width*scale;
      int newh = dest.height*scale;
      Resample(&dest,&resampledest,neww,newh);

      TurboJpegCompress(&resampledest,&koditvshowlib[shownum].fanartjpegref);

      free(dest.memory);
      free(resampledest.memory);

      }


  }

  curl_easy_cleanup(curl_handle);

  curl_global_cleanup();




}


bool CTVShowLib::Draw_LoadImageMemory(C2D_Image *texture, ImageMemoryStruct *source) {



	if (source->width > 1024 || source->height > 1024) {
		return false;
	}

	C3D_Tex *tex = (C3D_Tex *)linearAlloc(sizeof(C3D_Tex));
	Tex3DS_SubTexture *subtex = (Tex3DS_SubTexture *)linearAlloc(sizeof(Tex3DS_SubTexture));
	Draw_C3DTexToC2DImage(tex, subtex, (u8 *)source->memory, (u32)(source->width * source->height * BYTES_PER_PIXEL), (u32)source->width, (u32)source->height, GPU_RGB8);
	texture->tex = tex;
	texture->subtex = subtex;

	return true;
}

void CTVShowLib::CreateThumbTexture(int shownum){
    if(koditvshowlib[shownum].jpegref.size>0){
        if(thumbimage.thumbtext.tex != NULL){
            C3D_TexDelete(thumbimage.thumbtext.tex);
            linearFree((Tex3DS_SubTexture *)thumbimage.thumbtext.subtex);
        }
        struct ImageMemoryStruct dest;
        TurboJpegDecompress(&koditvshowlib[shownum].jpegref,&dest);
        thumbimage.width = dest.width;
        thumbimage.height = dest.height;
        Draw_LoadImageMemory(&thumbimage.thumbtext,&dest);
        free(dest.memory);
    }
    else
    {

            C3D_TexDelete(thumbimage.thumbtext.tex);
            linearFree((Tex3DS_SubTexture *)thumbimage.thumbtext.subtex);

    }
}

void CTVShowLib::CreateFanArtTexture(int shownum){
    if(koditvshowlib[shownum].fanartjpegref.size>0){
        if(fanartimage.thumbtext.tex != NULL){
            C3D_TexDelete(fanartimage.thumbtext.tex);
            linearFree((Tex3DS_SubTexture *)fanartimage.thumbtext.subtex);
        }
        struct ImageMemoryStruct dest;
        TurboJpegDecompress(&koditvshowlib[shownum].fanartjpegref,&dest);
        fanartimage.width = dest.width;
        fanartimage.height = dest.height;
        Draw_LoadImageMemory(&fanartimage.thumbtext,&dest);
        free(dest.memory);
    }
    else
    {

            C3D_TexDelete(fanartimage.thumbtext.tex);
            linearFree((Tex3DS_SubTexture *)fanartimage.thumbtext.subtex);

    }
}


void CTVShowLib::RequestTVShowEpisodesList(int tvshowid){

        char test[1024];
        sprintf(test,"{\"jsonrpc\": \"2.0\", \"method\": \"VideoLibrary.GetEpisodes\", \"params\": { \"tvshowid\" : %d ,\"properties\":[\"episode\",\"plot\",\"season\",\"seasonid\",\"tvshowid\"],\"sort\": { \"order\": \"ascending\", \"method\": \"label\" }   }, \"id\": \"libTvShowsEpisodes\"}",tvshowid);
        send(*kodisock,test,strlen(test),0);

}

int CTVShowLib::SearchTVShowID(int tvshowid){

    for(unsigned int i=0;i<koditvshowlib.size();i++){
        if(koditvshowlib[i].showid == tvshowid)return i;

    }

    return -1;
}

int CTVShowLib::SearchTVSeasonID(int seasonid,std::vector<tvseason_struct> *seasons){
    for(unsigned int i=0;i<seasons->size();i++){
        tvseason_struct season = seasons->at(i);
        if(season.seasonid == seasonid)return i;
    }

    return -1;
}


bool CTVShowLib::ParseGetTVShowEpisodes(char *buffer){
    struct json_object *jobj;
    jobj = json_tokener_parse(buffer);

    struct json_object *resobj;
    json_object_object_get_ex(jobj, "result", &resobj);

    struct json_object *moviesobj;
    json_object_object_get_ex(resobj, "episodes", &moviesobj);
    int nmovies = json_object_array_length(moviesobj);
    std::vector<tvseason_struct> seasons;
    struct json_object *tvshowidobj;
    for(int i=0;i<nmovies;i++){
        tvseason_struct season;
        struct json_object *episodeobj;
        episodeobj = json_object_array_get_idx(moviesobj, i);
        json_object_object_get_ex(episodeobj, "tvshowid", &tvshowidobj);
        struct json_object *seasonobj;
        struct json_object *seasonidobj;
        json_object_object_get_ex(episodeobj, "season", &seasonobj);
        json_object_object_get_ex(episodeobj, "seasonid", &seasonidobj);
        season.season = json_object_get_int(seasonobj);
        season.seasonid = json_object_get_int(seasonidobj);
        seasons.push_back(season);

    }

    int tvshowid =  json_object_get_int(tvshowidobj);


    auto comp = [] ( const tvseason_struct& lhs, const tvseason_struct& rhs ) {return lhs.seasonid < rhs.seasonid;};
    sort(seasons.begin(), seasons.end(),comp);

    auto comp1 = [] ( const tvseason_struct& lhs, const tvseason_struct& rhs ) {return lhs.seasonid == rhs.seasonid;};
    auto last = std::unique(seasons.begin(), seasons.end(),comp1);
    seasons.erase(last, seasons.end());


    int showarraynum = SearchTVShowID(tvshowid);


    koditvshowlib[showarraynum].seasons = seasons;

    json_object_object_get_ex(resobj, "episodes", &moviesobj);
     for(int i=0;i<nmovies;i++){

        struct json_object *episodeobj;
        episodeobj = json_object_array_get_idx(moviesobj, i);
        struct json_object *seasonidobj;
        json_object_object_get_ex(episodeobj, "seasonid", &seasonidobj);
        int seasonid = json_object_get_int(seasonidobj);
        int seasonarraynum = SearchTVSeasonID(seasonid,&koditvshowlib[showarraynum].seasons);

        tvepisode_struct episode;
        struct json_object *labelobj;
        struct json_object *plotobj;
        struct json_object *episodenumobj;
        struct json_object *episodeidobj;

        json_object_object_get_ex(episodeobj, "label", &labelobj);
        json_object_object_get_ex(episodeobj, "plot", &plotobj);
        json_object_object_get_ex(episodeobj, "episode", &episodenumobj);
        json_object_object_get_ex(episodeobj, "episodeid", &episodeidobj);

        char *labelstring = (char *)json_object_get_string(labelobj);
        char *plotstring = (char *)json_object_get_string(plotobj);

        episode.label.assign(labelstring,strlen(labelstring));
        episode.plot.assign(plotstring,strlen(plotstring));
        episode.episode = json_object_get_int(episodenumobj);
        episode.episodeid = json_object_get_int(episodeidobj);

        koditvshowlib[showarraynum].seasons[seasonarraynum].episodes.push_back(episode);


     }






return false;
}

bool CTVShowLib::ParseGetTVShow(char *buffer){

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


    if(start ==total){


        return true;
    }
    else
    {
        struct json_object *moviesobj;
        json_object_object_get_ex(resobj, "tvshows", &moviesobj);
        int nmovies = json_object_array_length(moviesobj);
        for(int i=0;i<nmovies;i++){

            struct json_object *tvshowobj;
            tvshowobj = json_object_array_get_idx(moviesobj, i);
            struct json_object *labelobj;
            struct json_object *plotobj;
            struct json_object *movieidobj;
            struct json_object *yearobj;
            struct json_object *thumbobj;
            struct json_object *fanartobj;
            json_object_object_get_ex(tvshowobj, "label", &labelobj);
            json_object_object_get_ex(tvshowobj, "plot", &plotobj);
            json_object_object_get_ex(tvshowobj, "tvshowid", &movieidobj);
            json_object_object_get_ex(tvshowobj, "year", &yearobj);
            json_object_object_get_ex(tvshowobj, "thumbnail", &thumbobj);
            json_object_object_get_ex(tvshowobj, "fanart", &fanartobj);





            koditvshowlib_struct koditvshow;

            char *labelstring = (char *)json_object_get_string(labelobj);
            char *plotstring = (char *)json_object_get_string(plotobj);
            char *thumbstring = (char *)json_object_get_string(thumbobj);
            char *fanartstring = (char *)json_object_get_string(fanartobj);



            koditvshow.label.assign(labelstring,strlen(labelstring));
            koditvshow.showid = json_object_get_int(movieidobj);
            koditvshow.year = json_object_get_int(yearobj);
            koditvshow.plot.assign(plotstring,strlen(plotstring));
            koditvshow.thumburl.assign(thumbstring,strlen(thumbstring));
            koditvshow.thumburl = urlEncode(koditvshow.thumburl);

            koditvshow.fanarturl.assign(fanartstring,strlen(fanartstring));
            koditvshow.fanarturl = urlEncode(koditvshow.fanarturl);

            char kodiurl[50];
            memset(kodiurl,0,50);
            sprintf(kodiurl,"http://%s:%d/image/",kodiaddress,*kodihttpport);
            koditvshow.thumburl.insert(0,kodiurl);
            koditvshow.fanarturl.insert(0,kodiurl);
            koditvshowlib.push_back(koditvshow);




        }


    return false;
    }



}


void CTVShowLib::PlayEpisode(int episodeid){

    char output[160];
    sprintf(output,"{\"jsonrpc\":\"2.0\", \"method\":\"Player.Open\", \"id\":%d,\"params\":{\"item\":{\"episodeid\": %d}}}",PLAYEPISODE,episodeid);
    send(*kodisock,output,strlen(output),0);

}

void CTVShowLib::PlaySeason(int seasonid){

    char output[160];

    send(*kodisock,output,strlen(output),0);

}


void CAudioLib::RequestAudioList(){



    std::vector<audioartist_struct>().swap(artistslib);

    char test[1024];
    sprintf(test,"{\"jsonrpc\": \"2.0\", \"method\": \"AudioLibrary.GetSongs\", \"params\": { \"limits\": { \"start\" : 0 }, \"properties\": [ \"artist\", \"duration\", \"album\", \"albumid\", \"artistid\", \"track\", \"year\", \"thumbnail\" ], \"sort\": { \"order\": \"ascending\", \"method\": \"artist\", \"ignorearticle\": true } }, \"id\": \"libSongs\"}");
    send(*kodisock,test,strlen(test),0);


}

int CAudioLib::SearchArtistID(int artistid){
    for(unsigned int i=0;i<artistslib.size();i++){
        if(artistslib[i].artistid == artistid)return i;
    }


return -1;
}

int CAudioLib::SearchAlbumID(int albumid,std::vector<audioalbum_struct> *albums){
    for(unsigned int i=0;i<albums->size();i++){
        audioalbum_struct album = albums->at(i);
        if(album.albumid == albumid)return i;
    }

return -1;
}



bool CAudioLib::ParseAudio(char *buffer){


    struct json_object *jobj;
    jobj = json_tokener_parse(buffer);

    struct json_object *resobj;
    json_object_object_get_ex(jobj, "result", &resobj);

    struct json_object *limitsobj;
    json_object_object_get_ex(resobj, "limits", &limitsobj);

    struct json_object *totalobj;
    json_object_object_get_ex(limitsobj, "total", &totalobj);

    int total = json_object_get_int(totalobj);

    if(total == 0)return false;



    struct json_object *songsobj;
    json_object_object_get_ex(resobj, "songs", &songsobj);
    int nsongs = json_object_array_length(songsobj);
    std::vector<songparse_struct> songsparse;






    for(int i=0;i<nsongs;i++){
        struct json_object *songobj;
        songobj = json_object_array_get_idx(songsobj, i);
        struct json_object *artistsobj;
        struct json_object *artistidsobj;

        json_object_object_get_ex(songobj, "artist", &artistsobj);
        json_object_object_get_ex(songobj, "artistid", &artistidsobj);

        struct json_object *artistobj;
        struct json_object *artistidobj;

        struct json_object *albumidobj;
        struct json_object *trackobj;
        struct json_object *songidobj;
        struct json_object *durationobj;
        struct json_object *albumtitleobj;
        struct json_object *songtitleobj;
        struct json_object *yearobj;



        artistobj = json_object_array_get_idx(artistsobj, 0);
        artistidobj = json_object_array_get_idx(artistidsobj, 0);

        songparse_struct song;
        song.artistid = json_object_get_int(artistidobj);
        char * namestring = (char *)json_object_get_string(artistobj);
        song.artistname.assign(namestring,strlen(namestring));

        json_object_object_get_ex(songobj, "albumid", &albumidobj);
        json_object_object_get_ex(songobj, "album", &albumtitleobj);
        json_object_object_get_ex(songobj, "track", &trackobj);
        json_object_object_get_ex(songobj, "songid", &songidobj);
        json_object_object_get_ex(songobj, "label", &songtitleobj);
        json_object_object_get_ex(songobj, "duration", &durationobj);
        json_object_object_get_ex(songobj, "year", &yearobj);

        char * albumtitlestring = (char *)json_object_get_string(albumtitleobj);
        song.albumtitle.assign(albumtitlestring,strlen(albumtitlestring));

        char * songtitlestring = (char *)json_object_get_string(songtitleobj);
        song.songtitle.assign(songtitlestring,strlen(songtitlestring));

        song.albumid = json_object_get_int(albumidobj);
        song.track = json_object_get_int(trackobj);
        song.songid = json_object_get_int(songidobj);
        song.duration = json_object_get_int(durationobj);
        song.year = json_object_get_int(yearobj);

        songsparse.push_back(song);

    }

    std::vector<songparse_struct> artistparse = songsparse;
    std::vector<songparse_struct> albumparse = songsparse;

    auto comp = [] ( const songparse_struct& lhs, const songparse_struct& rhs ) {return lhs.artistid < rhs.artistid;};
    sort(artistparse.begin(), artistparse.end(),comp);

    auto comp1 = [] ( const songparse_struct& lhs, const songparse_struct& rhs ) {return lhs.artistid == rhs.artistid;};
    auto last = std::unique(artistparse.begin(), artistparse.end(),comp1);
    artistparse.erase(last, artistparse.end());


    auto comp2 = [] ( const songparse_struct& lhs, const songparse_struct& rhs ) {return lhs.albumid < rhs.albumid;};
    sort(albumparse.begin(), albumparse.end(),comp2);

    auto comp3 = [] ( const songparse_struct& lhs, const songparse_struct& rhs ) {return lhs.albumid == rhs.albumid;};
    auto last2 = std::unique(albumparse.begin(), albumparse.end(),comp3);
    albumparse.erase(last2, albumparse.end());

    for(unsigned int i=0;i<artistparse.size();i++){
           audioartist_struct artist;
           artist.name = artistparse[i].artistname;
           artist.artistid = artistparse[i].artistid;
           artistslib.push_back(artist);
    }






    for(unsigned int i=0;i<albumparse.size();i++){
        int artistpos = SearchArtistID(albumparse[i].artistid);
        audioalbum_struct album;
        album.albumid = albumparse[i].albumid;
        album.title = albumparse[i].albumtitle;
        album.year = albumparse[i].year;
        artistslib[artistpos].albums.push_back(album);

    }

    for(unsigned int i=0;i<songsparse.size();i++){
        int artistpos = SearchArtistID(songsparse[i].artistid);
        int albumpos = SearchAlbumID(songsparse[i].albumid,&artistslib[artistpos].albums);
        audiosong_struct song;
        song.title = songsparse[i].songtitle;
        song.songid = songsparse[i].songid;
        song.track = songsparse[i].track;
        song.duration = songsparse[i].duration;
        artistslib[artistpos].albums[albumpos].songs.push_back(song);
    }


    return false;

}

void CAudioLib::PlaySong(int songid){


    char output[160];
    sprintf(output,"{\"jsonrpc\":\"2.0\", \"method\":\"Player.Open\", \"id\":%d,\"params\":{\"item\":{\"songid\":%d}}}",PLAYSONG,songid);
    send(*kodisock,output,strlen(output),0);

}


