#include "argo.h"
#include <string.h>
#include <stdlib.h>
void dumpmem(){
    for(int i = 0; i < vars.data.length; i++){
        if(vars.types.data[i] == STRING){
            string *s = malloc(sizeof(string));
            string_new(s);
            string_concat(s,"string_free("); 
            string_concat(s, (char*)vars.names.data[i]);
            string_concat(s,");");
            write_outws(s->data);
            string_free(s);
            free(s);
        }
    } 
}
