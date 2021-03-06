#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

static size_t write_data_file(void *ptr, size_t size, size_t nmemb, void *stream)
{
  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

static size_t write_data_image(void *ptr, size_t size, size_t nmemb, void *stream)
{
  //std::cout << size << " " << nmemb << std::endl;
  //stream = (unsigned char*)malloc(nmemb*sizeof(unsigned char));
  stream = ((unsigned char*)ptr);
  //memcpy(ptr,stream,nmemb*sizeof(unsigned char));
  //std::cout << (int)((unsigned char*)ptr)[0] << std::endl;
  //std::cout << (int)((unsigned char*)ptr)[1] << std::endl;
  //stream = reinterpret_cast<unsigned char*>(ptr);
  //std::cout << (int)((unsigned char*)stream)[0] << std::endl;

}

static size_t get_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  *(std::string*)stream = (char*)ptr;
  ((std::string*)stream)->resize(nmemb);
  replace((*(std::string*)stream),"© ","");
  replace((*(std::string*)stream),"à","a");
  replace((*(std::string*)stream),"è","e");
  replace((*(std::string*)stream),"é","e");
  replace((*(std::string*)stream),"ê","e");
  replace((*(std::string*)stream),"ï","i");
  replace((*(std::string*)stream),"ô","o");
  replace((*(std::string*)stream),"ù","u");
  replace((*(std::string*)stream),"ç","c");
}

void downloadFile(std::string url, std::string filename)
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_file);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    FILE *download = fopen(filename.c_str(),"w");
    curl_easy_setopt(curl,   CURLOPT_WRITEDATA, download);
    res = curl_easy_perform (curl);
    if(res!=0){
        std::cout << "Error code = " << res << std::endl;
    }
    fclose(download);

    curl_easy_cleanup (curl);
}


Image getImage(std::string url, int width, int height)
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_image);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());


    Image img(width,height);

    unsigned char* data;

    curl_easy_setopt(curl,   CURLOPT_WRITEDATA, &data );
    res = curl_easy_perform (curl);

    img.loadFromData(data,512,512,3);

    //if(res!=0){
    //    std::cout << "Error code = " << res << std::endl;
    //}
    //std::cout << "data = " << data << std::endl;

    curl_easy_cleanup (curl);

    return img;
}

std::string get(std::string url)
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    std::string data;
    curl_easy_setopt(curl,   CURLOPT_WRITEDATA, &data);
    res = curl_easy_perform (curl);

    //if(res!=0){
    //    std::cout << "Error code = " << res << std::endl;
    //}
    //std::cout << "data = " << data << std::endl;

    curl_easy_cleanup (curl);

    return data;
}
