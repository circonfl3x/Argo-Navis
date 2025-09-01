#include "lyra.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits.h>
void *lsafe_realloc(void *data, unsigned long n, unsigned short tsize){ 
    data = realloc(data, n*tsize); 
    return data;
}

wchar_t *cstowcs(const char *s){
    wchar_t *ret = calloc(strlen(s)+1, sizeof(wchar_t));
    size_t len = mbstowcs(ret, s, strlen(s));
    return len == -1 ? NULL : ret;
}

array2d *array2d_new(array2d *a, unsigned int tsize){
    if(!a) return NULL;
    a->length = 0;
    a->data = NULL;
    a->tsize = tsize;
    return a;
}

array2d *array2d_push(array2d *a, void *elem, unsigned long size){
    if(!a) return NULL;
    a->length++;
    a->data = realloc(a->data, a->length*sizeof(void*));
    a->data[a->length-1] = malloc(a->tsize*size);
    if(!a->data[a->length-1]) return NULL;
    memcpy(a->data[a->length-1], elem, size);
    return a;
}

void array2d_free(array2d *a){
    if(!a) return;
    for(int i = 0; i < a->length; i++){
        free(a->data[i]);
    }
    free(a->data);
}

array1d *array1d_new(array1d *a, unsigned int tsize){
    if(!a) return NULL;
    a->data = NULL;
    a->tsize = 0;
    a->length = 0;
    return a;
}

array1d *array1d_push(array1d *a, long elem){
    if(!a) return NULL;
    a->length++;
    a->data = realloc(a->data,a->length*sizeof(long));
    if(!a->data) return NULL;
    a->data[a->length-1] = elem;
    return a;
}

void array1d_free(array1d *a){
    free(a->data);
}

string *string_new(string *s){
    if(!s) return NULL;
    s->data = calloc(1, sizeof(wchar_t));
    *s->data = L'\0';
    s->length = 0;
    return s;
}
unsigned long string_getlength(string *s){
    if(!s) return 0;
    return s->length == 0 ? 0 : s->length;
}
string *string_addchar_chr(string *s, char c){
    if(!s) return NULL;
    s->length++;
    s->data = realloc(s->data, (s->length+1)*sizeof(wchar_t));
    if(!s->data) return NULL;
    if(!s->data) return NULL;
    s->data[s->length-1] = c;
    s->data[s->length] = L'\0';
    return s;
}
string *string_addchar_wchr(string *s, wchar_t wc){
    if(!s) return NULL; 
    s->length++;
    s->data = realloc(s->data, (s->length)*sizeof(wchar_t));
    if(!s->data) return NULL;
    if(!s->data) return NULL;
    s->data[s->length-1] = wc;
    s->data[s->length] = L'\0';
    return s;
}

string *string_concat_cstr(string *s, const char *str2){
    if(!s) return NULL;
    if(str2 == NULL) return s;
    wchar_t *tmp = calloc(strlen(str2)+1, sizeof(wchar_t));
    size_t len2 = mbstowcs(tmp, str2, strlen(str2));
    if(len2 == -1) return NULL;
    if(len2 == 0) return s;
    if(s->length == 0) s->length++;
    s->data = 
        lsafe_realloc(s->data, s->length+len2, sizeof(wchar_t));
    // s->data = realloc(s->data, (s->length+len2)*sizeof(wchar_t));
    if(!s->data) return NULL;
    wchar_t *ptr = s->data;
    ptr += s->length;
    bzero(ptr, len2);
    wcscat(s->data, tmp);
    free(tmp);

    s->length += len2;
    return s;
}

string *string_concat_wcstr(string *s, const wchar_t *str2){
    size_t len2 = wcslen(str2);
    s->data = 
        lsafe_realloc(s->data, s->length+len2, sizeof(wchar_t));
    // s->data = realloc(s->data, (s->length+len2)*sizeof(wchar_t));
    wchar_t *ptr = s->data;
    ptr += s->length;
    bzero(ptr, len2);
    wcscat(s->data, str2);

    s->length += len2;
    return s;
}
string *string_concat_str(string *s, const string *s2){
    if(!s) return NULL;
    if(s2->length == 0) return s;
    if(s->length == 0) s->length++;
    s->data = lsafe_realloc(s->data, s->length+s2->length, sizeof(wchar_t));
    if(!s->data) return NULL;
    wchar_t *ptr = s->data;
    ptr += s->length;
    bzero(ptr,s2->length); 
    wcscat(s->data, s2->data);
    s->length += s2->length;
    return s;
}
string *string_concat_vstr(string *s, const string *s2, ...){
    if(!s) return NULL;
    va_list list;

    for(va_start(list, s2); s2 != NULL; s2 = va_arg(list, const string*)){
        if(s2->length == 0) continue;
        if(s->length == 0) s->length++;
        s->data = lsafe_realloc(s->data, s->length+s2->length, sizeof(wchar_t));
        if(!s->data) return NULL;
        wchar_t *ptr = s->data;
        ptr += s->length;
        bzero(ptr,s2->length); 
        wcscat(s->data, s2->data);
        s->length += s2->length;
    }
    
    return s;
    
}
string *string_concat_vcstr(string *s, const char *s2, ...){
    if(!s) return NULL;
   va_list list;

    for(va_start(list, s2); s2 != NULL; s2 = va_arg(list, const char*)){
        size_t len = strlen(s2);
        if(len == 0) continue;
        if(s->length == 0) s->length++;
        s->data = lsafe_realloc(s->data, s->length+len, sizeof(wchar_t));
        if(!s->data) return NULL;
        wchar_t *ptr = s->data;
        ptr += s->length;
        bzero(ptr,len); 
        wchar_t *dataws = cstowcs(s2); 
        wcscat(s->data, dataws);
        free(dataws);
        s->length += len;
    }
    
    return s; 
}
string *string_concat_vwcstr(string *s, const wchar_t *s2, ...){
    if(!s) return NULL;
    va_list list;

    for(va_start(list, s2); s2 != NULL; s2 = va_arg(list, const wchar_t*)){
        size_t len = wcslen(s2);
        if(len == 0) continue;
        if(s->length == 0) s->length++;
        s->data = lsafe_realloc(s->data, s->length+len, sizeof(wchar_t));
        if(!s->data) return NULL;
        wchar_t *ptr = s->data;
        ptr += s->length;
        bzero(ptr,len); 
        wcscat(s->data, s2);
        s->length += len;
    }
    
    return s; 
}

void string_free(string *s){
    if(!s) return ;
    free(s->data);
}


linkedlist1d *ll_new(linkedlist1d *l, unsigned short tsize){
    if(!l) return NULL;
    l->prev = NULL;
    l->next = NULL;
    l->elem = NULL;
    l->tsize = malloc(sizeof(unsigned short));
    if(!l->tsize) return NULL;
    *l->tsize = tsize;
    return l;
}
linkedlist1d *ll_add(linkedlist1d *l, void *elem){
    if(!l) return NULL;
    linkedlist1d *ptr = l;
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    ptr->elem = calloc(1,sizeof( l->tsize));
    if(!ptr->elem) return NULL;
    memcpy(ptr->elem, elem, *l->tsize);
    ptr->next = malloc(sizeof(linkedlist1d));
    linkedlist1d *tmp = ptr;
    ptr = ptr->next;
    ptr->prev = tmp;
    ptr->next = NULL;
    ptr->elem = NULL;
    ptr->tsize = l->tsize;
    return l;
}

void ll_free(linkedlist1d *l){
    if(!l) return ;
    linkedlist1d *ptr = l;
    free(l->tsize);
    bool first = false;
    while(ptr != NULL){
        free(ptr->elem);
        linkedlist1d *tmp = ptr;
        ptr = ptr->next; 
        if(first) free(tmp); //freeing the first one
                             //is dangerous because linkedlist can be 
                             //either allocated on the heap or the stack
                             //and if on the stack that's a fuck you from the compiler
                             //all remaining are guaranteed to be allocated on the heap so its up
                             //to the user to free the initial linkedlist :)
        // ptr = ptr->next;
        first = true;
    }
    
}

bool ll_islastelement(linkedlist1d *ptr){
    if(!ptr) return true;
    return ptr->next == NULL ? true : false;
}

bool ll_isnullelement(linkedlist1d *ptr){
    if(!ptr) return true;
    return ptr->elem == NULL ? true : false;
}

bool ll_isempty(linkedlist1d *l){
    if(!l) return true;
    return l->elem == NULL ? true : false;
    
}


linkedlist2d *ll2d_new(linkedlist2d *l, unsigned short tsize){
    if(!l) return NULL;
    l->elem = NULL;
    l->prev = NULL;
    l->next = NULL;
    l->tsize = malloc(sizeof(unsigned short));
    *l->tsize = tsize;
    return l;
}

linkedlist2d *ll2d_add(linkedlist2d *l, void *elem, unsigned long n){
    if(!l) return NULL;
    linkedlist2d *ptr = l;
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    ptr->elem = calloc(1,sizeof( l->tsize)*n);
    if(!ptr->elem) return NULL;
    memcpy(ptr->elem, elem, *l->tsize*n);
    ptr->next = malloc(sizeof(linkedlist1d));
    linkedlist2d *tmp = ptr;
    ptr = ptr->next;
    ptr->prev = tmp;
    ptr->next = NULL;
    ptr->elem = NULL;
    ptr->tsize = l->tsize;
    return l;
}
void ll2d_free(linkedlist2d *l){
    if(!l) return ;
    linkedlist2d *ptr = l;
    free(l->tsize);
    bool first = false;
    while(ptr != NULL){
        free(ptr->elem);
        linkedlist2d *tmp = ptr;
        ptr = ptr->next; 
        if(first) free(tmp); //freeing the first one is dangerous here
                                  //too for the same reason
        // ptr = ptr->next;
        first = true;
    }
    
}
bool ll2d_islastelement(linkedlist2d *ptr){
    if(!ptr) return true;
    return ptr->next == NULL ? true : false;
}

bool ll2d_isnullelement(linkedlist2d *ptr){
    if(!ptr) return true;
    return ptr->elem == NULL ? true : false;
}

bool ll2d_isempty(linkedlist2d *l){
    if(!l) return true;
    return l->elem == NULL ? true : false;
    
}

//FILEBUF

filebuf *file_openr(const char *fpath){
    FILE *f = fopen(fpath, "r");
    if(!f) return NULL;
    filebuf *fbuf = calloc(1,sizeof(filebuf));
    fbuf->line = calloc(1, sizeof(string*));
    if(!fbuf) return fbuf;
    fbuf->line[0] = calloc(1,sizeof(string));
    string_new(fbuf->line[0]);
    fbuf->size = 1;
    if(!f) return NULL;
    char ch,ch2;
    while((ch=fgetc(f)) != EOF){
        if(ch == '\n' || ch == '\0'){
            fbuf->size++;
            // fbuf->line = 
                // lsafe_realloc(fbuf->line, fbuf->size, sizeof(string));
            fbuf->line = realloc(fbuf->line, (fbuf->size)*sizeof(string*)); 
            fbuf->line[fbuf->size-1] = calloc(1,sizeof(string));
            // fbuf->line[fbuf->size] = malloc(sizeof(string));
            // fseek(f,-1,SEEK_CUR);
            if(!fbuf->line) return NULL;
            string_new(fbuf->line[fbuf->size-1]);
        }else{
            string_addchar(fbuf->line[fbuf->size-1], ch);
        }
    }

    fclose(f);
    return fbuf;

}
void file_free(filebuf *fb){
    for(unsigned int i = 0; i < fb->size; i++){
        free(fb->line[i]->data);
        free(fb->line[i]);
    }
    free(fb->line);
    
}


//map
//
map *map_new(map *m, unsigned int k_nmemb, unsigned int v_nmemb){
    if(!m) return NULL;
    m->keys = malloc(sizeof(linkedlist2d));
    m->values = malloc(sizeof(linkedlist2d));
    if(!m->keys || !m->values) return NULL;
    if(ll2d_new(m->keys, k_nmemb) == NULL) return NULL;
    if(ll2d_new(m->values, v_nmemb) == NULL) return NULL;
    m->size=0;
    return m;
}

map *map_add(map *m, void *key, void *value, unsigned long k_len, unsigned long v_len){
    if(!m) return NULL;
    m->size++;
    if(ll2d_add(m->keys, key, k_len) == NULL) return NULL;
    if(ll2d_add(m->values, value, v_len) == NULL) return NULL; 
    return m; 
}

map *map_remove(map *m, void *key, unsigned long k_len){
    //TODO
    return NULL;
    
}
