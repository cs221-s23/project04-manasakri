#include "project04.h"



int main(int argc, char **argv) {

    struct scan_table_st scan_table; //creating structures that scan and iterate through the contents of the table and organizes them
    struct parse_table_st parse_table;  //structure that takes the values in the table and parse through them to calculate expression
    struct parse_node_st *parse_tree; 

    char input[SCAN_INPUT_LEN];
    int len;

    int base = 10 ; //decimal 
    bool baseFlag = false ;

    for(int i=1;i<argc;i++) {
        if (strcmp(argv[i], "-b") == 0) {
            baseFlag = true; //error check for input
            //without -b, the base is assumed to be 10 automatically
        }
    }

    if ( baseFlag && argc < 5  ) {
        printf("usage: ./project04 -b ? -e \"expr\"\n");
        printf("  example: ./project04 --b ? e \"1 + 2\"\n");
        exit(-1); //error check message for argument count
    }
    if ( ! baseFlag && argc < 3  ) {
        printf("usage: ./project04 -b ? -e \"expr\"\n");
        printf("  example: ./project04 --b ? e \"1 + 2\"\n");
        exit(-1); //error check message for argument count 
    } 

    for(int i=1;i<argc;i++)
    {
        char *arg = argv[i] ;

        if (strcmp( arg, "-e") ==  0)  {

            strncpy(input, argv[i+1], SCAN_INPUT_LEN); //places the input content into the var input
            i=i+1 ;

        } else if (strcmp(arg, "-b") == 0 ) {
            baseFlag = true;
            base = atoi(argv[i+1]);
            i=i+ 1 ;

        }
    }



    len = strnlen(input, SCAN_INPUT_LEN);

    scan_table_init(&scan_table); //initializes struct scan tables values to 0 
    scan_table_scan(&scan_table, input, len); //function that scans input values and organizs them based on operator-operand values 
   //does not calculate expressions, only organizes them into nodes 

    parse_table_init(&parse_table); //parse table struct that initializes vals to 0 
    parse_tree = parse_program(&scan_table); //places values into parse tree 
    
    int eval_sum = eval(parse_tree) ; //eval function calculates mathematical expressions
    //uses recursive call

	//printing and returning  based on base values 
    if ( base == 10) { //decimal 
        printf("%d\n",eval_sum );
    }
    else if ( base == 16) {//hex 
        print_hex(eval_sum);

    }else if ( base == 2) {//binary 
        printf("0b");
        print_binary(eval_sum );
        printf("\n");
    }

        
    return 0;
}


