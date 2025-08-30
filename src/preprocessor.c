#include "argo.h"
#include <stdbool.h>
#include <stdlib.h>
#include <wctype.h>
#include <stdio.h>
bool is_processed(string *l){
    bool isp = false;
    for(size_t i = 0; i < l->length; i++){
        if(iswalnum(l->data[i]) != 0){ //skip lines that are spaces
            // printf("'%ls' is valid\n", l->data);
            isp = true;
            break;
            
        }
    }
    return isp;
}
