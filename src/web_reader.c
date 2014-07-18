/*
** web_reader.c for webreader in /home/alexmog/testwebsocket/c_version/src/src
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May  8 21:32:21 2014 Moghrabi Alexandre
** Last update Fri May  9 11:27:28 2014 Moghrabi Alexandre
*/

#include <curl/curl.h>
#include <string.h>

// TODO: REWRITE THIS SHIET!

/* the function to invoke as the data recieved */
size_t write_callback_func(void *buffer,
			   size_t size,
			   size_t nmemb,
			   void *userp)
{
  char **response_ptr =  (char**)userp;

  /* assuming the response is a string */
  *response_ptr = strndup(buffer, (size_t)(size *nmemb));
  return (0);
}

/* the function to return the content for a url */
char *do_web_request(char *url)
{
  /* keeps the handle to the curl object */
  CURL *curl_handle = NULL;
  /* to keep the response */
  char *response = NULL;

  /* initializing curl and setting the url */
  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);
  curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1);

  /* follow locations specified by the response header */
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);

  /* setting a callback function to return the data */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback_func);

  /* passing the pointer to the response as the callback parameter */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);

  /* perform the request */
  curl_easy_perform(curl_handle);

  /* cleaning all curl stuff */
  curl_easy_cleanup(curl_handle);

  /* cleaning other curl stuff */
  curl_global_cleanup();

  return (response);
}
