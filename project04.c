#include "project04.h"



int main(int argc, char **argv) {

    struct scan_table_st scan_table; // table of tokens
    struct parse_table_st parse_table; // table of parse nodes
    struct parse_node_st *parse_tree; // tree (pointers only) of parse nodes

    char input[SCAN_INPUT_LEN];
    int len;

    int base = 10 ;
    bool baseFlag = false ;

    for(int i=1;i<argc;i++) {
        if (strcmp(argv[i], "-b") == 0) {
            baseFlag = true;
        }
    }

    if ( baseFlag && argc < 5  ) {
        printf("usage: ./project04 -b ? -e \"expr\"\n");
        printf("  example: ./project04 --b ? e \"1 + 2\"\n");
        exit(-1);
    }
    if ( ! baseFlag && argc < 3  ) {
        printf("usage: ./project04 -b ? -e \"expr\"\n");
        printf("  example: ./project04 --b ? e \"1 + 2\"\n");
        exit(-1);
    }

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



    len = strnlen(input, SCAN_INPUT_LEN);

    scan_table_init(&scan_table);
    scan_table_scan(&scan_table, input, len);
    // You may uncomment this if you need to debug the scanner 
    // but leave it commented for "grade test" since scanner output
    // is not part of the expected output for lab06
    // scan_table_print(&scan_table);

    parse_table_init(&parse_table);
    parse_tree = parse_program(&scan_table);
    //parse_tree_print(parse_tree);

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


