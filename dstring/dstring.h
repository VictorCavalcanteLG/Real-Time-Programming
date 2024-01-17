#ifndef _DSTRING_H
#define _DSTRING_H

#include <string.h>

typedef struct dstr {
    size_t size;
    char buf[];
} Dstr;

#define Dstr_size(s) (s->size)
#define $(s) (s->buf)

#define newDstr(X) _Generic((X), \
        char* : newDstr_str, \
        char  : newDstr_char, \
        int   : newDstr_int, \
        long   : newDstr_long, \
        float   : newDstr_float, \
        double   : newDstr_double)(X)
        
// Constructor
Dstr *newDstr_str(char *str);
Dstr *newDstr_char(char c);
Dstr *newDstr_int(int i);
Dstr *newDstr_long(long l);
Dstr *newDstr_float(float f);
Dstr *newDstr_double(double d);

// Destructor
void delDstr(Dstr **ps);

//concat
Dstr *Dstr_cat(Dstr *s1, Dstr *s2);

#endif
