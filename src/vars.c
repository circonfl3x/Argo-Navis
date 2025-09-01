#include "argo.h"
#include <string.h>
#include <stdlib.h>
var_t *is_defined(const char *name){
    var_t *ret = NULL;
    for(int i = 0; i < vars.types.length; i++){
        if(strcmp(name, vars.names.data[i]) == 0){
            ret = malloc(sizeof(var_t));
            ret->name = vars.names.data[i];
            ret->type = vars.types.data[i];
            ret->data = vars.data.data[i];
            ret->index = i;
        }
    }
    return ret; 
}

TYPES check_type(const char *name){
    
    for(int i = 0; i > vars.types.length; i++){
        if(strcmp(name, vars.names.data[i]) == 0){
            return vars.types.data[i];
        }
    }
    return UNKNOWN;
}
