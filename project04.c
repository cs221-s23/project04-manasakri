#include "project04.h"

void print_binary(unsigned int number)
{
    for (int i = 31; i >= 0; i--) {
        int k =number >> i;
        if (k & 1)
            printf("1");
        else printf("0");
    }

}

void print_hex(unsigned int number) {

    printf("0x%08x\n", number) ;
}

int main(int argc, char **argv) {

    struct scan_table_st scan_table;  
    struct parse_table_st parse_table;  
    struct parse_node_st *parse_tree;  

    char input[SCAN_INPUT_LEN];
    int len;

    int base = 10 ;
    bool baseFlag = false ;

    for(int i=1;i<argc;i++)
    {
        char *arg = argv[i] ;

        if (strcmp( arg, "-e") ==  0)  {

             strncpy(input, argv[i+1], SCAN_INPUT_LEN);
            i=i+1 ;

        } else if (strcmp(arg, "-b") == 0 ) {
            baseFlag = true;
             base = atoi(argv[i+1]);
            i=i+ 1 ;

        }
    }

    if ( baseFlag && argc < 5 || argc < 3 ) {
        printf("usage: ./lab06 -b ? -e \"expr\"\n");
        printf("  example: ./lab06 --b ? e \"1 + 2\"\n");
        exit(-1);
    }

    len = strnlen(input, SCAN_INPUT_LEN);

    scan_table_init(&scan_table);
    scan_table_scan(&scan_table, input, len);
    

    parse_table_init(&parse_table);
    parse_tree = parse_program(&scan_table);
 
    int eval_sum = eval(parse_tree) ;

    if ( base == 10) {
        printf("%d\n",eval_sum );
    }
    else if ( base == 16) {
        print_hex(eval_sum);

    }else if ( base == 2) {
        printf("0b");
        print_binary(eval_sum );
        printf("\n");
    }

        
    return 0;
}


