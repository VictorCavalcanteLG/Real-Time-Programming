#include <stdio.h>

#include "../adt/dstring/dstring.h"

int main(int argc, char **argv) {
    puts("Test ADT Dstr");

    Dstr *str1 = newDstr("Hello");
    Dstr *str2 = newDstr_char('C');
    Dstr *str3 = newDstr(42);
    Dstr *str4 = newDstr(123456L);
    Dstr *str5 = newDstr(3.14f);
    Dstr *str6 = newDstr(2.718);

    printf("s1 = %s\n", $(str1));
    printf("s2 = %s\n", $(str2));
    printf("s3 = %s\n", $(str3));
    printf("s4 = %s\n", $(str4));
    printf("s5 = %s\n", $(str5));
    printf("s6 = %s\n", $(str6));

    printf("newConcat1 = %s\n", $(Dstr_cat(str1, str2)));
    printf("newConcat2 = %s\n", $(Dstr_cat(str3, str4)));
    printf("newConcat3 = %s\n", $(Dstr_cat(str5, str6)));

    delDstr(&str1);
    delDstr(&str2);
    delDstr(&str3);
    delDstr(&str4);
    delDstr(&str5);
    delDstr(&str6);
    
    return 0;
}