#ifndef _LAB06_H
#define _LAB06_H

#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>


#define SCAN_TOKEN_LEN 32
#define SCAN_INPUT_LEN 2048

enum scan_token_enum {
    TK_INTLIT, // -123, 415
    TK_BINLIT, // 0b10
    TK_HEXLIT, // 0xFF
    TK_LPAREN, // (
    TK_RPAREN, // )
    TK_PLUS,   // +
    TK_MINUS,  // -
    TK_MULT,   // *
    TK_DIV,    // /
    TK_EOT,    // end of text
    TK_ANY,    // wildcard for parsing
    NUM_TOKENS,
    TK_RIGHTSHIFT,
    TK_LEFTSHIFT,
    TK_TILDE,
    TK_AND,
    TK_OR,
    TK_CARROT,
};

#define SCAN_TOKEN_STRINGS {"TK_INTLIT", "TK_BINLIT", "TK_HEXLIT", \
                            "TK_LPAREN", "TK_RPAREN", \
                            "TK_PLUS", "TK_MINUS", "TK_MULT", "TK_DIV", \
                            "TK_EOT", "TK_ANY",                    \
                            "TK_RIGHTSHIFT", "TK_LEFTSHIFT", "TK_TILDE", "TK_AND", \
                            "TK_OR", " TK_CARROT" } ;


struct scan_token_st {
    enum scan_token_enum id;
    char name[SCAN_TOKEN_LEN];
    struct scan_token_st *next;
};

struct scan_table_st {
    struct scan_token_st *head;
    int len;
    int next;
};

void scan_table_init(struct scan_table_st *st);
void scan_table_scan(struct scan_table_st *st, char *input, int len);
void scan_table_print(struct scan_table_st *st);
struct scan_token_st * scan_table_get(struct scan_table_st *st, int i);
bool scan_table_accept(struct scan_table_st *st, enum scan_token_enum tk_expected);


enum parse_expr_enum {EX_INTVAL, EX_HEXVAL, EX_OPER1, EX_OPER2};
enum parse_oper_enum {OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_LEFTSHIFT, OP_RIGHTSHIFT, OP_TILDE, OP_AND, OP_OR, OP_CARROT};

struct parse_node_st {
    enum parse_expr_enum type;
    union {
        struct {
            int value;
        } intval;
        struct {
            enum parse_oper_enum oper;
            struct parse_node_st *operand;
        } oper1;
        struct {
            enum parse_oper_enum oper;
            struct parse_node_st *left;
            struct parse_node_st *right;
        } oper2;
    };                
};

struct parse_table_st {
    struct parse_node_st *root;
};

void parse_table_init(struct parse_table_st *pt);
struct parse_node_st *parse_node_new();
void parse_tree_print(struct parse_node_st *np);
struct parse_node_st *parse_program(struct scan_table_st *st);

int eval(struct parse_node_st *np);


struct parse_node_st *parse_expression(struct scan_table_st *st);
struct parse_node_st *parse_operand(struct scan_table_st *st);

int string_to_int(char *string, int base) ;
void print_binary(int number);
void print_hex(int number);

#endif 
