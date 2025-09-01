#include "argo.h"
#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
bool is_valid_var(const char *data, bool print_error);
bool is_valid_string(const char *data, bool print_err);
bool is_valid_int(const char *data, bool print_err);
bool is_valid_chr(const char *data, bool print_err);

TYPES infer_type(const char *data);



const char *types[3] = {"int", "string", "char"};

const char *reg_str = "^\\\"(\\\"|[^\"])*\"$"; //^\"(\\"|[^"])*\"$
const char *reg_int = "^[0-9]+$";
const char *reg_chr = "^'(.)'$";
const char *reg_var_rules = "^[a-zA-Z]\\w*$";
const char *reg_spec_func = "^externC\\.puts\\((.*)\\)$";





string *parse(tokens *t){
    vector *ptr = &t->tokens;
    string *ret = malloc(sizeof(string));
    TYPES num_type = UNKNOWN;
    string_new(ret);
    if(strncmp(ptr->data[0], "evar_d",6) == 0){
        
        parse_var(ret, t, &num_type, false);    
        if(ret == NULL) goto enderr;
    }else if(strncmp(ptr->data[0], "puts", 4) == 0){
        //externC.print
        char *arg = ptr->data[1];
        bool valid = false;
        if(is_valid_string(arg, false)){
            string_concat_vcstr(ret, "puts(",arg,");",NULL);
        }else if(is_valid_chr(arg, false)){
            string_concat_vcstr(ret, "printf(\"%c\\n\",",arg,");", NULL);
        }else if(is_valid_int(arg, false)){
            string_concat_vcstr(ret, "printf(\"%d\\n\",",arg,");", NULL);
        }else if(is_valid_var(arg, false)){
            var_t *v;
            if((v=is_defined(arg)) != NULL){
                if(v->type == STRING){
                    string_concat_vcstr(ret, "printf(\"%ls\\n\",",arg,".data);",NULL);
                }else if(v->type == INT){
                    string_concat_vcstr(ret, "printf(\"%d\\n\",",arg,");",NULL);
                }else{
                    string_concat_vcstr(ret, "printf(\"%c\\n\",",arg,");",NULL);
                }        
                free(v);
            }
        }else{
            wchar_t *argws = cstowcs(arg);
            Error(L"\"\033[36m",argws, L"\033[0m\"", L" is not a valid argument for print",NULL);
            free(argws);
        }
            
        
    }else if(strncmp(ptr->data[0], "var_r", 5) == 0){
        var_t *v;
        TYPES t = infer_type(ptr->data[2]); 
        if((v=is_defined(ptr->data[1])) != NULL){
            if(t != v->type){
                
                
                
                wchar_t *varws = cstowcs(ptr->data[1]);
                Error(L"Type mismatch in redefinition of variable '\033[36m",varws,L"\033[0m'",NULL);

                free(varws);
                goto enderr;
            }
            wchar_t *varws = cstowcs(ptr->data[1]);
            free(vars.data.data[v->index]); 
            vars.data.data[v->index] =  calloc(strlen(ptr->data[2])+1, vars.data.tsize);
            strcpy(vars.data.data[v->index], ptr->data[2]); 
            

            if(t == STRING){
                string_concat_vcstr
                    (ret,"string_free(&",ptr->data[1],");\n",NULL);
                string_concat_vcstr
                    (ret, "\tstring_new(&",ptr->data[1],");\n",NULL);
                string_concat_vcstr
                    (ret, "\tstring_concat(&",ptr->data[1],", ",ptr->data[2],");\n",NULL);
            }else if(t == INT || t == CHAR){
                string_concat_vcstr(ret, ptr->data[1], "=",ptr->data[2],";",NULL);
            }else{
                Error(L"How did we get here",NULL);
            }
            
            free(varws);
            free(v);

            

        }else{
            wchar_t *varws = cstowcs(ptr->data[1]);
            Error(L"Variable '\033[36m",varws,L"\033[0m' has not been defined",NULL); 
            free(varws);
            goto enderr;
        }
    }
    else{

        
        goto enderr;
    }
    return ret;


enderr:
    string_free(ret);
    free(ret);
    return NULL;
}
void parse_var(string *ret, tokens *t, TYPES *num_type, bool reassign){
    vector *ptr = &t->tokens;
    const char *type = ptr->data[1];
        // printf("%s\n", type);
    for(int i = 0; i < sizeof(types)/sizeof(types[0]); i++){
        if(strcmp(type, types[i]) == 0){
            *num_type = i;
            break;
        }
        }
        if(*num_type == UNKNOWN){
            wchar_t *type_ws = cstowcs(type);
            Error(L"'", type_ws, "' ","is not a valid type", NULL); 
            free(type_ws);
            ret = NULL;
        }
        const char *name = ptr->data[2];
        if(!is_valid_var(name, true)){
            goto enderr;
        }
        const char *value = ptr->data[3];
        if(*num_type == STRING){
            if(!is_valid_string(value, true)) goto enderr;
        }else if(*num_type == INT){
            if(!is_valid_int(value, true)) goto enderr;
        }else if(*num_type == CHAR){
            if(!is_valid_chr(value, true)) goto enderr;
        }else{
            puts("How did we get here");
            goto enderr;
        } 
        // printf("%s %s %s\n", type, name, value);
        string_concat(ret, type);
        if(strcmp(type, "string") == 0){
            /*string_concat(ret, " ");
            string_concat(ret, name); 
            string_concat(ret, ";\n");
            string_concat(ret, "\tstring_new(&");
            string_concat(ret, name);
            string_concat(ret, ");\n");
            string_concat(ret, "\tstring_concat(&");
            string_concat(ret, name);
            string_concat(ret, ",");
            string_concat(ret, value);
            string_concat(ret, ");");*/
            string_concat_vcstr(ret, " ", name,";\n",
                    "\tstring_new(&",name,");\n","\tstring_concat(&",name,",",value,");",NULL);
            //what in the actual fuck is this
            //TODO: make string_concat vardic (DONE)

        }else{ 
            /*string_concat(ret, " ");
            string_concat(ret, name);
            string_concat(ret, "=");
            string_concat(ret, value);
            string_concat(ret, ";");*/
            string_concat_vcstr(ret, " ",name,"=",value,";",NULL);
        }
        if(!reassign){
            array2d_push(&vars.names, (void*)name, strlen(name)+1);
            array2d_push(&vars.data, (void*)value, strlen(value)+1); 
            array1d_push(&vars.types, *num_type);
        }else{
            
        }
enderr: 
        ret = NULL;
}






bool is_valid_var(const char *data, bool print_err){
    regex_t *reg = calloc(1, sizeof(regex_t));
    int ret = regcomp(reg, reg_var_rules, REG_EXTENDED);
    bool retb = true;
    if(ret != 0) return false;

    for(uint i = 0; ; i++){
        const char *ptr = data;

        if(regexec(reg, data, 0, NULL,0)){
           wchar_t *dataws = cstowcs(data);
           if(print_err) Error(L"'",dataws, L"' is not a valid variable name.", 
                   L" Variable names must begin with a letter and only consist of letters");
           free(dataws);
           retb = false;
        }
        break;
    }
    regfree(reg);
    free(reg);
    return retb;
}
bool is_valid_string(const char *data, bool print_err){
    regex_t *reg = calloc(1, sizeof(regex_t));
    int ret = regcomp(reg, reg_str, REG_EXTENDED); 
    bool retb = true;
    if(ret != 0) return false;
    for(uint i = 0; ; i++){
        const char *ptr = data;

        if(regexec(reg, data, 0, NULL, 0)){
            wchar_t *dataws = cstowcs(data);
            if(print_err) Error(dataws, L" is not a valid string literal. Strings have to be in the format \"...\"",NULL);
            free(dataws);
            retb = false;
            break; 
        }
        // printf("%s is a valid string", data);        
        break;

    }

    regfree(reg);
    free(reg);
    return retb;

}
bool is_valid_int(const char *data, bool print_err){
    regex_t *reg = calloc(1, sizeof(regex_t));
    int ret = regcomp(reg, reg_int, REG_EXTENDED); 
    bool retb = true;
    if(ret != 0) return false;
    for(uint i = 0; ; i++){
        const char *ptr = data;

        if(regexec(reg, data, 0, NULL, 0)){
            wchar_t *dataws = cstowcs(data);
            if(print_err)Error(dataws, L" is not a valid integer",NULL);
            free(dataws);
            retb = false;
            break; 
        }
        // printf("%s is a valid int", data);        
        break;

    }

    regfree(reg);
    free(reg);
    return retb;
}
bool is_valid_chr(const char *data, bool print_err){
    regex_t *reg = calloc(1, sizeof(regex_t));
    int ret = regcomp(reg, reg_chr, REG_EXTENDED); 
    bool retb = true;
    if(ret != 0) return false;
    for(uint i = 0; ; i++){
        const char *ptr = data;

        if(regexec(reg, data, 0, NULL, 0)){
            wchar_t *dataws = cstowcs(data);
            if(print_err) Error(dataws, L" is not a valid char",NULL);
            free(dataws);
            retb = false;
            break; 
        }
        // printf("%s is a valid char", data);        
        break;

    }

    regfree(reg);
    free(reg);
    return retb;

}
TYPES infer_type(const char *data){
    TYPES t;
    if(is_valid_string(data, false)) t = STRING;
    else if(is_valid_int(data, false)) t = INT;
    else if(is_valid_chr(data, false)) t = CHAR;
    else t = UNKNOWN;
    return t;
}
