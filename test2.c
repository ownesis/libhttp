#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libhttp.h"

int main(void) {
    char buffer[BUFSIZ] = {0};
    int ret = 0;
    ssize_t data_write = 0;

    char data[] = "Nous forgerons comme de vrai forgeron (seulement du http) espece de connard !!!";
    size_t data_size = strlen(data);
    
    // init http;
    HTTP_t *http = HTTP_init();

    ret = HTTP_set_version(http, HTTPv1_1);
    if (!ret)
        exit(EXIT_FAILURE);
    
    ret = HTTP_set_path(http, "/voltaire_a_terre_maltezer_peper_pomme2terre/");
    if (!ret)
        exit(EXIT_FAILURE);
 
    ret = HTTP_set_method(http, GET);
    if (!ret)
        exit(EXIT_FAILURE);
 


    ret = HTTP_set_header(http, "User-Agent", "MacBook woula");
    if (!ret)
        exit(EXIT_FAILURE);
 
    ret = HTTP_set_header(http, "Cookie", "atoutmangé");
    if (!ret)
        exit(EXIT_FAILURE);

    ret = HTTP_set_header(http, "Boule", "Bill");
    if (!ret)
        exit(EXIT_FAILURE);

#if 1   
    ret = HTTP_set_query(http, "jetebaisealopeabcde", "connasseputeputerhalalala");
    ret = HTTP_set_query(http, "B", "2");
    ret = HTTP_set_query(http, "C", "3");
    ret = HTTP_set_query(http, "D", "4");
    ret = HTTP_set_query(http, "E", "5");
#endif

    // write data
    data_write = HTTP_write_body(http, data, data_size);
    if (!data_write)
        exit(EXIT_FAILURE);

    // make raw request
    data_write = HTTP_make_raw_request(http, buffer, BUFSIZ);
    if (data_write)
        printf("%s", buffer);

    
 //   HTTP_show_header(http);

    // free http
    HTTP_clear(&http);
  
    return 0;
}
