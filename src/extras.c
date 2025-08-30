#include "argo.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void Error(const wchar_t *msg, ...){
    va_list list;
    fprintf(stderr, "\033[31mError\033[0m (line %d): ", line_number);
    for(va_start(list,msg); msg != NULL; msg = va_arg(list, const wchar_t *)){ 
        fprintf(stderr, "%ls", msg);  
    }
    puts("");
}
void Warning(const wchar_t *msg, ...){
    va_list list;
    printf("\033[35mWarning:\033[0m ");
    for(va_start(list,msg); msg != NULL; msg = va_arg(list, const wchar_t *)){ 
        printf("%ls ", msg);  
    }
    puts("");
}
void set_gerr(Errors e){
   g_flags |= (1<<e); 
}
void clear_gerr(Errors e){
    g_flags &= (1<<e);

}
unsigned char toggle_gerr(){
    return 0;    
}
void set_flag(unsigned int *a, unsigned int flag){
   *a |= (1<<flag); 
}
unsigned int check_flag
    (unsigned int *a, unsigned int flag){
    return *a & (1<<flag);
}

bool is_on(unsigned int *a, unsigned int flag){
    return (*a & (1<<flag)) == 1 ? true : false;
}

unsigned int toggle_flag
    (unsigned int *a, unsigned int flag){
   *a = *a ^ (1 << flag);
   return *a; 
}

char *itoa(int a){
    char *str = calloc(ceil(log10(a))+1,1);
    sprintf(str, "%d", a);

   return str; 
}
