#pragma once
#include <wchar.h>
#include <stdbool.h>
typedef struct{
    unsigned long length;
    void **data;
    unsigned int tsize;
} array2d;

typedef struct{
    unsigned long length;
    long *data;
    unsigned int tsize;
} array1d;


//changes pending?
typedef array2d vector;
typedef array1d array;

typedef struct linkedlist1d{
    struct linkedlist1d *prev;
    void *elem;
    unsigned short *tsize;
    struct linkedlist1d *next; 
} linkedlist1d;

typedef struct linkedlist2d{
    struct linkedlist2d *prev;
    void *elem;
    struct linkedlist2d *next;
    unsigned short *tsize;
} linkedlist2d;

typedef struct{
    wchar_t *data;
    unsigned long length;
}   string;
typedef struct {
    string **line;
    unsigned int size;
} filebuf;

typedef struct{
    linkedlist2d *keys;
    linkedlist2d *values;
    unsigned long size;
    // unsigned int k_size;
    // unsigned int v_size;
} map;


//array
array2d *array2d_new(array2d *a, unsigned int tsize);
array2d *array2d_push(array2d *a, void *elem, unsigned long size);
void array2d_free(array2d *a);

array1d *array1d_new(array1d *a, unsigned int tsize);
array1d *array1d_push(array1d *a, long elem);
void array1d_free(array1d *a);
//typedef array1d string;

//string
string *string_new(string *s);
#define string_addchar(s, c) \
    _Generic((c), char: string_addchar_chr, \
            wchar_t: string_addchar_wchr)(s,c)
string *string_addchar_chr(string *s, char c);
string *string_addchar_wchr(string *s, wchar_t wc);
//string *string_addchar(string *s, char c);
#define string_concat(s,x) _Generic((x), \
        string*: string_concat_str, \
        const string*: string_concat_str,\
        char*: string_concat_cstr,\
        const char*:string_concat_cstr,\
        wchar_t*: string_concat_wcstr)(s,x)
string *string_concat_str(string *s, const string *s2);
string *string_concat_cstr(string *s, const char *str2);
string *string_concat_wcstr(string *s, const wchar_t *wcstr2);

unsigned long string_getlength(string *s);
void string_free(string *s);

//how many n of tsize to allocate
//it is safe in the sense you will never forget to pass through the data type size
//but not safe in the sense you can still fuck up the mathematics with n and everything goes to shit
void *lsafe_realloc(void *data, unsigned long n, unsigned short tsize);

wchar_t *cstowcs(const char *s);

//linkedlist
linkedlist1d *ll_new(linkedlist1d *l, unsigned short tsize);
linkedlist1d *ll_add(linkedlist1d *l, void *elem);
void ll_free(linkedlist1d *l);
unsigned long ll_size(linkedlist1d *l);
linkedlist1d *ll_pop(linkedlist1d *l, unsigned int index);
bool ll_islastelement(linkedlist1d *ptr);
bool ll_isnullelement(linkedlist1d *ptr);
bool ll_isempty(linkedlist1d *l);

linkedlist2d *ll2d_new(linkedlist2d *l, unsigned short tsize);
//n many items in the *elem
linkedlist2d *ll2d_add(linkedlist2d *l, void *elem, unsigned long n);
void ll2d_free(linkedlist2d *l);
linkedlist1d *ll2d_size(linkedlist2d *l);
linkedlist1d *ll2d_pop(linkedlist2d *l, unsigned int index);
bool ll2d_islastelement(linkedlist2d *ptr);
bool ll2d_isnullelement(linkedlist2d *ptr);
bool ll2d_isempty(linkedlist2d *l);


//file 
filebuf *file_openr(const char *fpath);
void file_free(filebuf *fb);

map *map_new(map *m, unsigned int k_nmemb, unsigned int v_nmemb);
map *map_add(map *m, void *key, void *value, unsigned long k_len, unsigned long v_len);
map *map_remove(map *m, void *key, unsigned long k_len); //how tf to even do this lmao
