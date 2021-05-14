#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libhttp/http.h>

int main(void) {
    version_t version;
    status_code_t code;
    HTTPDict_t *dict = NULL;
    char data[BUFSIZ] = {0};
    
    char raw_res[] = "HTTP/1.1 200 OK\r\nContent-Length: 24\r\nDate: Mon, 03 May 2021 18:36:29 GMT\r\nServer: heavyhttpd/1.2.3\r\n\r\noh=shit&here=we&go=again";

    size_t raw_res_size = strlen(raw_res);

    // init http;
    HTTP_t *http = HTTP_init();

    // parse response
    HTTP_parse_res_raw(http, raw_res, raw_res_size);
    
    code = HTTP_get_status_code(http);
    version = HTTP_get_version(http);
    
    printf("Code: %d (%s)\n", code, HTTP_get_str_code(http, code));
    printf("Version: %s\n", HTTP_version_to_str(http, version));

    printf("\n------- header -------\n");
    HTTP_show_header(http);
    printf("----- end header -----\n");

    HTTP_read_body(http, data, BUFSIZ);
    
    printf("\n-------- body --------\n");
    printf("%s", data);
    printf("\n------ end body ------\n");

    // Parse body data in url format
    HTTP_parse_query(http, data);

    printf("\n-------- query --------\n");
    HTTP_show_query(http);
    printf("\n------ end query ------\n");

    // Get value of 'oh' key.
    dict = HTTP_query_get_val_with_key(http, "oh");
    
    printf("\nPersonal note:\n");
    printf("Java is a %sty programming langage.\n", dict->val);

    // Clear dict
    HTTP_dict_clear(&dict);

    // free http
    HTTP_clear(&http);
 
    return 0;
}
