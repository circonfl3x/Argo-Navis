#include "argo.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <string.h>
#include <wchar.h>
#include <stdint.h>
tokens *tokenize(string *l){
    tokens *t = malloc(sizeof(tokens));
    t->total = 0;
    // ll2d_new(&t->tokens, STRSIZE); 
    array2d_new(&t->tokens, STRSIZE);
    unsigned int flag = 0;
    const char *reg_vars =  
        //"^\\s*(\\w+)\\s+(\\w+)\\s*=\\s*(\\S+)\\s*$";   
                         "^\\s*(\\w+)\\s+(\\w+)\\s*=\\s*(.*)\\s*$";
    const char *reg_funcs = 
        // "^\\s*fn\\s+.*\\s*\\(\\s*.*\\s*\\)\\s*\\{?\\s*$"; 
        "^\\s*fn\\s+(\\w+)\\s*\\(\\s*([^)]*)\\s*\\)\\s*\\{?\\s*$";
    const char *reg_funccall = "^\\s*(\\w+)\\s*\\((.*\\))\\s*s*$";
    const char *reg_varauto_reass = "^\\s*(\\w+)\\s*=\\s*(.*)\\s*$";
    const char *reg_specfn = "^externC\\.print\\((.*)\\)$";


    char *data = calloc(1,l->length+1);

    size_t size = l->length+1;
    wcstombs(data, l->data, size); 
    int psize = 1 ;
    regex_t reg;
    regmatch_t *pmatch;
    regoff_t off,len;

    int ret = regcomp(&reg, reg_vars, REG_EXTENDED);

    // int ret = 0;
    if (ret == 0){ //vars
        char *ptr = data;
        psize = 4;
        pmatch = calloc(psize, sizeof(regmatch_t));

        for(uint i = 0; ; i++){
            if(regexec(&reg, ptr, psize,pmatch,0)){
                break;
            } 

            // printf("'%ls' is a variable\n", l->data);
            char *v_type = strndup(data+pmatch[1].rm_so, pmatch[1].rm_eo-pmatch[1].rm_so); 
            char *v_name = strndup(data+pmatch[2].rm_so, pmatch[2].rm_eo-pmatch[2].rm_so);
            char *v_data = strndup(data+pmatch[3].rm_so, pmatch[3].rm_eo-pmatch[3].rm_so); 
            if(!v_type || !v_name || !v_data){
                Error(L"error occurred parsing variable", NULL);
            } 
            // ll2d_add(&vars.names, v_name, strlen(v_name)); 
            // ll2d_add(&vars.types, v_type, strlen(v_type)); 
            // ll2d_add(&vars.data, v_data, strlen(v_data)); 
            
            t->total += 4;
            // ll2d_add(&t->tokens, "evar_d", STRSIZE*6); 
            array2d_push(&t->tokens,"evar_d",6);    //e(xplicit)var(iable)_d(eclaration) 
                                                                   //YES IM VERY FKN SMART IK(🤓)
                                                                   //(NOTE: this is an unsafe string because
                                                                   //it isnt null terminated but oh well, we never
                                                                   //print it so fuck it we ball fiwb)
            array2d_push(&t->tokens, v_type, strlen(v_type)+1);
            array2d_push(&t->tokens, v_name, strlen(v_name)+1);
            array2d_push(&t->tokens, v_data, strlen(v_data)+1);
            free(v_type);
            free(v_name);
            free(v_data);
            // ll2d_add(&t->tokens, v_type, strlen(v_type));
            // ll2d_add(&t->tokens, v_name, strlen(v_name));
            // ll2d_add(&t->tokens, v_data, strlen(v_data)); 

            if(pmatch[2].rm_eo == pmatch[2].rm_so){
                Error(L"Cannot match type or name",NULL);
                goto returnnull;
            }
            // printf("type: %s\nname: %s\ndata: %s\n\n", v_type, v_name, v_data); 
            set_flag(&flag, VARS);   
            ptr += pmatch[0].rm_eo;
        }
        free(pmatch);
    }else{
        Error(L"couldn't regex vars",NULL);
        goto returnnull;
    }
    regfree(&reg);
    if(is_on(&flag, VARS)) goto end;

    //special function
    ret = regcomp(&reg, reg_specfn, REG_EXTENDED);
    if(ret == 0){
        char *ptr = data;
        psize = 3;
        pmatch = calloc(psize, sizeof(regmatch_t));
        for(uint i = 0;;i++){
            if(regexec(&reg, ptr, psize, pmatch, 0)) break;
            t->total += 2;
            set_flag(&flag, SPECFUNC);
            char *argument = strndup(data+pmatch[1].rm_so, pmatch[1].rm_eo-pmatch[1].rm_so);
            array2d_push(&t->tokens, "puts", 4);
            array2d_push(&t->tokens, argument, strlen(argument)+1);
            free(argument);
            ptr += pmatch[0].rm_eo;


            
        }
        free(pmatch);
    }
    regfree(&reg);
    if(is_on(&flag, SPECFUNC)) goto end;
    //functions section
    ret = regcomp(&reg, reg_funcs, REG_EXTENDED); 

    if(ret == 0){
        char *ptr = data;
        psize = 3;
        pmatch = calloc(psize, sizeof(regmatch_t));
        for(uint i = 0; ; i++){
            if(regexec(&reg, ptr, psize,pmatch,0)){
                break;
            }
            t->total += 3;
            Warning(L"Functions can be declared but for now they are useless",NULL);
            // ll2d_add(&t->tokens, "efun_d", STRSIZE*6); //e(xplicit)fun(ction)_d(eclaration) ye ye you get the point
                                                                   //also unsafe string again lamao
            char *fn_name = strndup(data+pmatch[1].rm_so, pmatch[1].rm_eo-pmatch[1].rm_so); 
            char *fn_args = strndup(data+pmatch[2].rm_so, pmatch[2].rm_eo-pmatch[2].rm_so);
            array2d_push(&t->tokens,  fn_name, strlen(fn_name)+1); 
            array2d_push(&t->tokens, fn_args, strlen(fn_args)+1);
            
            // printf("'%ls' is a function\n", l->data);
            set_flag(&flag,FUNCS);
            ptr += pmatch[0].rm_eo;
            free(fn_name);
            free(fn_args);
        }
        free (pmatch);
    }else{
        // char error_buf[512];
        // regerror(ret, &reg, error_buf, 512);
        // printf("%s\n", error_buf);
        Error(L"Couldn't regex funcs", NULL);
        goto returnnull;
    }
    regfree(&reg);

    if(is_on(&flag, FUNCS)) goto end; 
    
    //variable reassign

    ret = regcomp(&reg, reg_varauto_reass, REG_EXTENDED);
    if(ret == 0){
        char *ptr = data;
        psize = 3; 
        pmatch = calloc(psize, sizeof(regmatch_t));
        for(uint i = 0; ; i++){
            if(regexec(&reg, ptr, psize, pmatch, 0)) break;
            char *v_name = strndup(data+pmatch[1].rm_so, pmatch[1].rm_eo-pmatch[1].rm_so);
            char *v_data = strndup(data+pmatch[2].rm_so, pmatch[2].rm_eo-pmatch[2].rm_so); 
            if(!v_name || !v_data){
                Error(L"Error occurred parsing variable", NULL);
            }
            t->total += 3;
            array2d_push(&t->tokens, "var_r", 5);
            array2d_push(&t->tokens, v_name, strlen(v_name)+1);
            array2d_push(&t->tokens, v_data, strlen(v_data)+1);
            free(v_name);
            free(v_data);
            if(pmatch[2].rm_eo == pmatch[2].rm_so){
                Error(L"Cannot match name or value",NULL);
                goto returnnull;
            }
            set_flag(&flag, VARS_REASS);
            ptr += pmatch[0].rm_eo;


        }
    }
    free(pmatch);
    if(is_on(&flag, VARS_REASS)) goto end; 
    if(flag == 0){
        Error(L"Cannot parse line '",l->data,L"'", NULL);
        goto returnnull;
    }
end:
    regfree(&reg);
    free(data);
    return t; 
returnnull:
    free(data);
    regfree(&reg);
    // ll2d_free(&t->tokens);
    array2d_free(&t->tokens);
    free(t);
    return NULL;
}
