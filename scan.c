#include "project04.h"

void scan_error(char *err) {
    printf("scan_error: %s\n", err);
    exit(-1);
}

void scan_table_init(struct scan_table_st *st) {
    st->head = NULL;
    st->len = 0;
    st->next = 0;
}

struct scan_token_st *scan_table_new_token(struct scan_table_st *tt) {
    struct scan_token_st *tp = calloc(1, sizeof(struct scan_token_st));
    if (!tt->head) {
        tt->head = tp;
    } else {
        struct scan_token_st *walk = tt->head;
        while (1) {
            if (!walk->next) {
                walk->next = tp;
                break;
            } else {
                walk = walk->next;
            }
        }
    }

    // increment the length of reserved tokens 
    tt->len += 1;
    return tp;
}

char * scan_read_token(struct scan_token_st *tp, char *p, int len,
                       enum scan_token_enum id) {
    int i;

    tp->id = id;
    for (i = 0; i < len; i++) {
        tp->name[i] = *p;
        p += 1;
    }
    tp->name[i] = '\0';
    return p;
}

bool scan_is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool scan_is_whitespace(char c) {
    return (c == ' ' || c == '\t');
}

char * scan_whitespace(char *p, char *end) {
    while (scan_is_whitespace(*p) && (p < end)) {
        p += 1;
    }
    return p;
}
bool scan_is_bdigit(char *p) {
    //TODO
    if ( *p == '1' || *p == '0') return true ;
    else {
   	 return false ;
     }
}

bool scan_is_hdigit(char *p) {
    if (scan_is_digit(*p) || *p == 'a' || *p == 'b' || *p =='c' || *p  == 'd' || *p == 'e' || *p == 'f' ||
        *p == 'A' || *p == 'B' || *p =='C' || *p  == 'D' || *p == 'E' || *p == 'F')
        return true ;
    else {  
     return false ;
     }
}


char *scan_bintlit(struct scan_token_st *tp, char *p, char *end) {

    tp->id = TK_BINLIT;

    int i=0;
    while ( scan_is_bdigit(p) ) {
        tp->name[i] = *p;
        p += 1;
        i = i + 1 ;
    }
    tp->name[i] = '\0'; 
    return p;

}



char *scan_hintlit(struct scan_token_st *tp, char *p, char *end) {

    tp->id = TK_HEXLIT;

    int i=0;
    while ( scan_is_hdigit(p) ) {
        tp->name[i] = *p;
        p += 1;
        i = i + 1 ;
    }
    tp->name[i] = '\0';  
    return p;

}
char * scan_integer(char *p, char *end, struct scan_token_st *tp) {
    int i = 0;
    while (scan_is_digit(*p) && p < end) {
        tp->name[i] = *p;
        p += 1;
        i += 1;
    }
    tp->name[i] = '\0';
    tp->id = TK_INTLIT;
    return p;
}

char * scan_token(char *p, char *end, struct scan_token_st *tp) {
    if (p == end) {
        p = scan_read_token(tp, p, 0, TK_EOT);
    } else if (scan_is_whitespace(*p)) {
        p = scan_whitespace(p, end);
        p = scan_token(p, end, tp);
    }else if (  *p == '0' && *(p+1) == 'b' ) {
        p = p + 2 ;
        p = scan_whitespace(p, end);
        p = scan_bintlit(tp, p, end);

    } else if ( *p == '0' && *(p+1) == 'x' ) {
        p = p + 2;
        p = scan_whitespace(p, end);
        p = scan_hintlit(tp, p, end);

    }
    else if (scan_is_digit(*p)) {
        p = scan_integer(p, end, tp);
    } else if (*p == '+') {
        p = scan_read_token(tp, p, 1, TK_PLUS);
    } else if (*p == '-') {
        p = scan_read_token(tp, p, 1, TK_MINUS);
    } else if (*p == '*') {
        p = scan_read_token(tp, p, 1, TK_MULT);
    } else if (*p == '/') {
        p = scan_read_token(tp, p, 1, TK_DIV);                        
    } else if (*p == '(') {
        p = scan_read_token(tp, p, 1, TK_LPAREN);
    } else if (*p == ')') {
        p = scan_read_token(tp, p, 1, TK_RPAREN);
    } else if (*p == '<' && *(p+1) == '<') {
        p = scan_read_token(tp, p, 2, TK_LEFTSHIFT);
    } else if (*p == '>' && *(p+1) == '>') {
        p = scan_read_token(tp, p, 2, TK_RIGHTSHIFT);
    } else if (*p == '^') {
        p = scan_read_token(tp, p, 1, TK_CARROT);
    } else if (*p == '&') {
        p = scan_read_token(tp, p, 1, TK_AND);
    }else if (*p == '|') {
        p = scan_read_token(tp, p, 1, TK_OR);
    } else if (*p == '~') {
        p = scan_read_token(tp, p, 1, TK_TILDE);
    } else {
        scan_error("Invalid character");
    }

    return p;
}

void scan_table_scan(struct scan_table_st *st, char *input, int len) {
    struct scan_token_st *tp;
    char *p = input;
    char *end = p + len;

    while(true) {
        tp = scan_table_new_token(st);
        p = scan_token(p, end, tp);
        if (tp->id == TK_EOT) {
            break;
        }
    }
}

void scan_table_print(struct scan_table_st *tt) {

     char *id_names[NUM_TOKENS] = SCAN_TOKEN_STRINGS;

    struct scan_token_st *tp = tt->head;
    while (tp) {
        // print the ID and name of that token
        printf("%s(\"%s\")\n", id_names[tp->id], tp->name);
        tp = tp->next;
    }
}

 struct scan_token_st * scan_table_get(struct scan_table_st *st, int i) {
    struct scan_token_st *pt = st->head;
    int which = st->next + i;

    for (int i = 0; i < which; i++) {
        pt = pt->next;
    }
    return pt;
}

bool scan_table_accept(struct scan_table_st *st, enum scan_token_enum tk_expected) {
    struct scan_token_st *tp;

    if (tk_expected == TK_ANY) {
        st->next += 1;
        return true;
    }

    tp = scan_table_get(st, 0);

    if (tp->id == tk_expected) {
        st->next += 1;
        return true;
    }

    return false;
}


