#include <stdlib.h>
#include <stdio.h>
#include "dstring.h"

// Constructor
Dstr *newDstr_str(char *str) {
    size_t len = strlen(str);
    Dstr *s = (Dstr *)malloc(sizeof(Dstr) + len + 1); // +1 para o caractere nulo
    if (s) {
        s->size = len;
        strcpy($(s), str);
    }
    return s;
}

Dstr *newDstr_char(char c) {
    Dstr *s = (Dstr *)malloc(sizeof(Dstr) + 2); // +1 para o caractere nulo
    if (s) {
        s->size = 1;
        s->buf[0] = c;
        s->buf[1] = '\0';
    }
    return s;
}

Dstr *newDstr_int(int i) {
    int str_size = snprintf(NULL, 0, "%d", i);
    Dstr *s = (Dstr *)malloc(sizeof(Dstr) + str_size + 1);
    if(s){
        s->size = str_size;
        sprintf(s->buf, "%d", i);
    }

    return s;
}

Dstr *newDstr_long(long l) {
    int str_size = snprintf(NULL, 0, "%ld", l);
    Dstr *s = (Dstr *)malloc(sizeof(Dstr) + str_size + 1);
    if(s){
        s->size = str_size;
        sprintf(s->buf, "%ld", l);
    }

    return s;
}

Dstr *newDstr_float(float f) {
    int str_size = snprintf(NULL, 0, "%f", f);
    Dstr *s = (Dstr *)malloc(sizeof(Dstr) + str_size + 1);
    if(s){
        s->size = str_size;
        sprintf(s->buf, "%f", f);
    }

    return s;
}

Dstr *newDstr_double(double d) {
    int str_size = snprintf(NULL, 0, "%f", d);
    Dstr *s = (Dstr *)malloc(sizeof(Dstr) + str_size + 1);
    if(s){
        s->size = str_size;
        sprintf(s->buf, "%f", d);
    }

    return s;
}

// Destructor
void delDstr(Dstr **ps) {
    if(ps) {
        free(*ps);
        *ps = NULL;
    }
}


// Concat
Dstr *Dstr_cat(Dstr *s1, Dstr *s2) {
    size_t size = s1->size + s2->size;
    Dstr *s = (Dstr *)malloc(sizeof(Dstr) + size + 1);

    if (s) {
        s->size = size;
        strcpy(s->buf, $(s1));
        strcat(s->buf, $(s2));
    }

    return s;
}
