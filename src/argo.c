#include "argo.h"
#include <stdio.h>
#include <stdlib.h>
int main(void){
    
    filebuf *f = file_openr("test.ag");
    // ll2d_new(&vars.names, STRSIZE);
    // ll2d_new(&vars.types, STRSIZE);
    // ll2d_new(&vars.data, STRSIZE);
    array2d_new(&vars.names, STRSIZE);
    array2d_new(&vars.data, STRSIZE);
    array1d_new(&vars.types, sizeof(TYPES));
    init_out_file();

    if(f == NULL){
        Error(L"couldn't open file",NULL);
        return 1;
    }
    for(int i = 0; i < f->size; i++){
        line_number++;
        if(!is_processed(f->line[i])){
            continue;
        }else{
            // printf("%ls\n", f->line[i]->data); 
            tokens *t = tokenize(f->line[i]);
            if(t != NULL){
                string *out = parse(t);
                if(out){
                    write_outws(out->data);
                    
                }else{

                    array2d_free(&t->tokens);
                    free(t);

                    goto end;
                }
                string_free(out);
                free(out);
                // ll2d_free(&t->tokens);
                array2d_free(&t->tokens);
            endcomp: 
                free(t);

            }
            else goto end;
        }
    }
    
end:
    /*for(int i = 0; i > vars.types.length; i++){
        puts("Hello");
        char *name = vars.names.data[i];
        long type = vars.types.data[i];
        char *data = vars.data.data[i];
        printf("name:%s\ntype:%ld\ndata:%s\n\n",
                name,type,data);
    }*/
    close_file();
    array2d_free(&vars.names);
    array2d_free(&vars.data);
    array1d_free(&vars.types);
    // ll2d_free(&vars.names);
    // ll2d_free(&vars.types);
    // ll2d_free(&vars.data);
    file_free(f); 
    free(f);

    return 0;
}
