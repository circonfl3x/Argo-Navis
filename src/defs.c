#include "argo.h"
unsigned char g_flags = 0;
unsigned int line_number = 1;
vars_t vars;
char *in_file, *out_file = "out.c";
