// parse.c - parsing and parse tree construction

#include "project04.h"
#include "math.h"

void parse_table_init(struct parse_table_st *parse_table) {
    parse_table->root = NULL;
}

struct parse_node_st * parse_node_new() {
    return calloc(1, sizeof(struct parse_node_st));
}

void parse_error(char *err) {
    printf("parse_error: %s\n", err);
    exit(-1);
}

char *parse_oper_strings[] = {"PLUS", "MINUS", "MULT", "DIV", "LEFTSHIFT", "RIGHTSHIFT", "TILDE", "AND", "OR", "CARROT"};

void parse_tree_print_indent(int level) {
    level *= 2;
    for (int i = 0; i < level; i++) {
        printf(".");
    }
}

void parse_tree_print_expr(struct parse_node_st *node, int level) {
    parse_tree_print_indent(level);
    printf("EXPR ");

    
    if (node->type == EX_INTVAL) {
        printf("INTVAL %d\n", node->intval.value);
    }
    else if (node->type == EX_OPER1) {
        printf("OPER1 %s\n", parse_oper_strings[node->oper1.oper]);
        parse_tree_print_expr(node->oper1.operand, level + 1);

    }
    else if (node->type == EX_OPER2) {
        printf("OPER2 %s\n", parse_oper_strings[node->oper2.oper]);
        parse_tree_print_expr(node->oper2.left, level + 1);
        parse_tree_print_expr(node->oper2.right, level + 1);
    }
}

void parse_tree_print(struct parse_node_st *np) {
    parse_tree_print_expr(np, 0);
}


int eval(struct parse_node_st *node) {


    if (node->type == EX_INTVAL) {
        return node->intval.value;
    }
    else if (node->type == EX_OPER1) {

        if ( node->oper1.oper == OP_MINUS) {
            int sum = eval(node->oper1.operand );
            return -sum ;
        }
        if ( node->oper1.oper == OP_TILDE) {
            int sum = eval(node->oper1.operand );
            return ~sum ;
        }

    }
    else if (node->type == EX_OPER2) {

        if ( node->oper2.oper == OP_PLUS ) {
            int left_sum = eval(node->oper2.left );
            int right_sum = eval(node->oper2.right );
            return left_sum + right_sum ;
        }
        if ( node->oper2.oper == OP_MINUS ) {
            int left_sum = eval(node->oper2.left );
            int right_sum = eval(node->oper2.right );
            return left_sum - right_sum ;
        }
        if ( node->oper2.oper == OP_MULT ) {
            int left_sum = eval(node->oper2.left );
            int right_sum = eval(node->oper2.right);
            return left_sum * right_sum ;
        }
        if ( node->oper2.oper == OP_DIV ) {
            int left_sum = eval(node->oper2.left );
            int right_sum = eval(node->oper2.right );
            return left_sum / right_sum ;
        }

        if ( node->oper2.oper == OP_RIGHTSHIFT ) {
            int left_sum = eval(node->oper2.left );
            int right_sum = eval(node->oper2.right );
            return left_sum >> right_sum ;
        }
        if ( node->oper2.oper == OP_LEFTSHIFT ) {
            int left_sum = eval(node->oper2.left);
            int right_sum = eval(node->oper2.right );
            return left_sum << right_sum ;
        }

        if ( node->oper2.oper == OP_AND ) {
            int left_sum = eval(node->oper2.left );
            int right_sum = eval(node->oper2.right );
            return left_sum & right_sum ;
        }
        if ( node->oper2.oper == OP_OR ) {
            int left_sum = eval(node->oper2.left );
            int right_sum = eval(node->oper2.right );
            return left_sum | right_sum ;
        }
        if ( node->oper2.oper == OP_CARROT ) {
            int left_sum = eval(node->oper2.left );
            int right_sum = eval(node->oper2.right );
            return left_sum ^ right_sum ;
        }


    }
}


struct parse_node_st * parse_program(struct scan_table_st *scan_table) {
    struct parse_node_st *root;

    root = parse_expression(scan_table);

    if (!scan_table_accept(scan_table, TK_EOT)) {
        parse_error("Expecting EOT");        
    }

    return root;                                       
}


struct parse_node_st * parse_expression(struct scan_table_st *scan_table) {

    struct scan_token_st *token;
    struct parse_node_st *node1, *node2;

    node1 = parse_operand(scan_table);

   
    while (true) {    
        token = scan_table_get(scan_table, 0);

        if (token->id == TK_PLUS || token->id == TK_MINUS || token->id == TK_MULT || token->id == TK_DIV ||
            token->id == TK_RIGHTSHIFT || token->id == TK_LEFTSHIFT || token->id == TK_TILDE || token->id == TK_AND ||
                token->id == TK_OR || token->id == TK_CARROT
                ) {

            scan_table_accept(scan_table, TK_ANY);
            node2 = parse_node_new();
            node2->type = EX_OPER2;

            if ( token->id == TK_PLUS ) node2->oper2.oper = OP_PLUS;
            if ( token->id == TK_MINUS ) node2->oper2.oper = OP_MINUS;
            if ( token->id == TK_MULT) node2->oper2.oper = OP_MULT;
            if ( token->id == TK_DIV ) node2->oper2.oper = OP_DIV;

            if ( token->id == TK_LEFTSHIFT ) node2->oper2.oper = OP_LEFTSHIFT;
            if ( token->id == TK_RIGHTSHIFT ) node2->oper2.oper = OP_RIGHTSHIFT;
            if ( token->id == TK_TILDE ) node2->oper1.oper = OP_TILDE;
            if ( token->id == TK_AND ) node2->oper2.oper = OP_AND;
            if ( token->id == TK_OR ) node2->oper2.oper = OP_OR;
            if ( token->id == TK_CARROT ) node2->oper2.oper = OP_CARROT;

            node2->oper2.left = node1;
            node2->oper2.right = parse_operand(scan_table);
            node1 = node2;
        }
        else {
            break;
        }
    }

    return node1;
}


struct parse_node_st *parse_operand(struct scan_table_st *scan_table) {
    struct scan_token_st *token;
    struct parse_node_st *node;

  
    if (scan_table_accept(scan_table, TK_MINUS)) {

        node = parse_node_new();
        node->type = EX_OPER1;
        node->oper1.oper = OP_MINUS  ;

        struct parse_node_st *node1;
        token = scan_table_get(scan_table, 0);
        node1 = parse_node_new();
        node1->type = EX_INTVAL;
        node1->intval.value = string_to_int(token->name,10);

        node->oper1.operand = node1 ;
        scan_table->next = scan_table->next + 1 ;
        return node ;

    }else if (scan_table_accept(scan_table, TK_TILDE)) {

            node = parse_node_new();
            node->type = EX_OPER1;
            node->oper1.oper = OP_TILDE  ;

            token = scan_table_get(scan_table, 0);
            if ( token->id == TK_LPAREN) {
                scan_table->next = scan_table->next + 1 ;

                struct scan_table_st scan_table1;
                scan_table_init(&scan_table1);

                char input1[SCAN_INPUT_LEN];
                memset(input1, '\0', sizeof(SCAN_INPUT_LEN));
                int i=0;
                int left = 1 ;
                int right = 0 ;
                while ( left != right ) {
                    char cc[32] ;
                    memset(cc, '\0', sizeof(32));
                    struct scan_token_st *p = scan_table_get(scan_table,i) ;

                    if ( p->id== TK_LPAREN  ) {
                        left =left + 1 ;
                    }
                    else if ( p->id == TK_RPAREN  )  {
                        right =right + 1 ;
                    }

                    if ( left != right ) {
                        strncpy(cc, p->name, 32);
                        strcat(input1, " ");
                        if ( p->id == TK_HEXLIT) strcat(input1, "0x");
                        if ( p->id == TK_BINLIT) strcat(input1, "0b");
                        strcat(input1, cc);
                    }
                    i = i + 1 ;
                }
                scan_table->next = scan_table->next + i ;
                int len = strnlen(input1, SCAN_INPUT_LEN);
                scan_table_scan(&scan_table1, input1, len);

                struct parse_node_st *node1;
                node1 = parse_expression(&scan_table1);
                node->oper1.operand = node1 ;

            } else {
                struct parse_node_st *node1;
                node1 = parse_node_new();
                node1->type = EX_INTVAL;
                node1->intval.value = string_to_int(token->name,10);
                node->oper1.operand = node1 ;

            }
            return node ;
        }
    else if (scan_table_accept(scan_table, TK_INTLIT)) {
        token = scan_table_get(scan_table, -1);
        node = parse_node_new();
        node->type = EX_INTVAL;
        node->intval.value = string_to_int(token->name,10);
    }
    else if (scan_table_accept(scan_table, TK_BINLIT) ){
        token = scan_table_get(scan_table, -1);
        node = parse_node_new();
        node->type = EX_INTVAL ;
        node->intval.value = string_to_int(token->name,2);

    }
    else if (scan_table_accept(scan_table, TK_HEXLIT) ){
        token = scan_table_get(scan_table, -1);
        node = parse_node_new();
        node->type = EX_INTVAL;
        node->intval.value = string_to_int(token->name,16);

    } else if (scan_table_accept(scan_table, TK_LPAREN) ){
        struct scan_table_st scan_table1;
        scan_table_init(&scan_table1);

        char input1[SCAN_INPUT_LEN];
        memset(input1, '\0', sizeof(SCAN_INPUT_LEN));
        int i=0;
        int left = 1 ;
        int right = 0 ;
        while ( left != right ) {
            char cc[32] ;
            memset(cc, '\0', sizeof(32));
            struct scan_token_st *p = scan_table_get(scan_table,i) ;

            if ( p->id== TK_LPAREN  ) {
                left =left + 1 ;
            }
            else if ( p->id == TK_RPAREN  )  {
                right =right + 1 ;
            }

            if ( left != right ) {
                strncpy(cc, p->name, 32);
                strcat(input1, " ");
                if ( p->id == TK_HEXLIT) strcat(input1, "0x");
                if ( p->id == TK_BINLIT) strcat(input1, "0b");
                strcat(input1, cc);
            }
            i = i + 1 ;
        }
        scan_table->next = scan_table->next + i ;
        int len = strnlen(input1, SCAN_INPUT_LEN);
        scan_table_scan(&scan_table1, input1, len);
        node = parse_expression(&scan_table1);

    }
    else if (scan_table_accept(scan_table, TK_RPAREN) ){

    }
    else {
        parse_error("Bad operand");
    }

    return node;
}

