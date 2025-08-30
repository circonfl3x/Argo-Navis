#pragma once
#include "lyra.h"
#include <stdbool.h>
#define STRSIZE 1
#define SUC 1
#define ERR 0
#define SIZE_STATARR(x) sizeof(x)/x[0]

typedef enum{
    INT,STRING,CHAR, UNKNOWN
} TYPES;

typedef struct{
    vector tokens;
   unsigned int total;
}  tokens;
/*typedef struct {
    //there is probably a better
    //way to do this
    //wanted to use a map
    //but there are 3 data types
    linkedlist2d names;
    linkedlist2d types;
    linkedlist2d data;
}vars_t;*/
typedef struct{
    char *name;
    char *data;
    TYPES type;
} var_t;
typedef struct{
    vector names;
    array types;
    vector data;
} vars_t;
extern vars_t vars; 
extern unsigned char g_flags;
extern unsigned int line_number;
extern char *in_file, *out_file;
//preprocessor
bool is_processed(string *l);
//lexer
tokens *tokenize(string *l);


//parser
string *parse(tokens *t);

//vars
var_t *is_defined(const char *name);
TYPES check_type(const char *name);
//compiler



//misc
typedef enum {
   NONE, EXIT, TOKENIZER,  
} Errors;
typedef enum {
    VARS, FUNCS, SPECFUNC,  COMMENTS,

} token;
void Error(const wchar_t *msg, ...);
void Warning(const wchar_t *msg, ...);
void set_gerr(Errors e);
void clear_gerr(Errors e);
unsigned char toggle_gerr();

//flags
void set_flag(unsigned int *a, unsigned int flag);
unsigned int check_flag(unsigned int *a, unsigned int flag);
unsigned int toggle_flag(unsigned int *a, unsigned int flag);

bool is_on(unsigned int *a, unsigned int flag);
char *itoa(int a);

//file
int init_out_file();

int write_out(const char *s);
int write_outws(const wchar_t *ws);
int close_file();
