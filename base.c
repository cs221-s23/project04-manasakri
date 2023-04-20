#include "project04.h"
#include <math.h>

void print_binary(int number)
{
    for (int i = 31; i >= 0; i--) {
        int k =number >> i;
        if (k & 1)
            printf("1");
        else printf("0");
    }

}

void print_hex(int number) {

    printf("0x%08X\n", number) ;
}

int string_to_int(char *string, int base) {

    if (base == 16 || base == 2 ) {

        int total = 0;
        int x, i ;
        while (*string)
        {
            total *= base;

            x = 0 ;
            if ( *string == '1') {
                x = 1 ;
            }
            else if( *string >='0' && *string <='9'){
                x = *string - '0';
            }
            else if ( *string >='A' && *string <='Z' ){
                x = *string - 'A' + 10;

            } else if  ( *string >='a' && *string <='z' ) {
                x = *string - 'a' + 10;
            }

            string++ ;

            total += x;
        }
        return total ;
    } else {
        return atoi(string) ;
    }


}
