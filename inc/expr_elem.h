#ifndef _EXPR_ELEM_H_
#define _EXPR_ELEM_H_

#define CHAR_LIST_LEN_MAX (512-1)
int strlen_limit(char *&,unsigned int);
int strcpy_limit(char *&,char *&,unsigned int);

//The count limit of elem of expression.
#define EXPR_BUILD_LOOP_DEEP_MAX (40)
enum{
    INPUT_ERROR=1,
    MALLOC_ERROR,
    LOGIC_ERROR,
};
typedef struct expr_elem_type{
    expr_elem_type *next=nullptr;
    int elem_deep=0;//recode the deep of expression.
    unsigned char type=1;//0-char 1-int 2-label
    char oper='\n';//Binary func?expend to func?
    int numb=0;
    char *label=nullptr;

    expr_elem_type();
    expr_elem_type(char);
    expr_elem_type(int);
    expr_elem_type(char*);
    ~expr_elem_type();
}expr_elem;

typedef struct expr_list_tpye{
    expr_elem *expr=nullptr;//the head of the expression
    expr_list_tpye *ptr=nullptr;//the pointer toword the forward list-elem
    int len=0;//recode the len of list.

    expr_list_tpye* append(expr_elem*);
    expr_list_tpye* get(expr_elem*&);

    expr_list_tpye();
    ~expr_list_tpye();
}expr_list;

//(num0 oper num1) such as 1+2
expr_elem *expr_set(char oper,int num0, int num1);
expr_elem *expr_set(char oper,int num, expr_elem &expr);
expr_elem *expr_set(char oper,expr_elem &expr, int num);
expr_elem *expr_set(char oper,expr_elem &expr0, expr_elem &expr1);
#define EXPR_LABEL_LEN_MAX 16
expr_elem *expr_set(char oper,char* label0, char* label1);
expr_elem *expr_set(char oper,char* label, int num);
expr_elem *expr_set(char oper, int num, char* label);
expr_elem *expr_set(char oper,char* label, expr_elem &expr);
expr_elem *expr_set(char oper, expr_elem &expr, char* label);

expr_elem *expr_link(char oper, expr_elem &expr0, expr_elem &expr1);


expr_elem* expr_find_end(expr_elem &);
int get_expr_len(expr_elem &);


/*
LOGIC OF [FUNCTION]expr_build
[input]1+2*3-4
strlen=7 oper=null
i=0) left_expr: 1       |                       |
  1) get_oper: +        |                       |
     find_right-------->|[input]2*3-4           |
                        |strlen=5 oper=+        |
                        |i=0) left_expr: 2      |
                        |  1) get_oper: *       |
                        |     comp '+'<'*'      |
                        |     find_right------->|[input]3-4
                        |                       |strlen=3 oper=*
                        |                       |i=0) left_expr: 3
                        |                       |  1) get_oper: -
                        |                       |     comp '*'>'-'
                        |     right_expr: 3 <---|     get_right
                        |     offset: 2 <-------|     offset=i+1
                        |     left_expr: *23
                        |i+offset=3)
                        |     get_oper: -
                        |     comp '+'='-'
     right_expr: *23 <--|     get_right
     offset: 4 <--------|     offset=i+1;
     left_expr:+1*23
i+offset=5)
     get_oper: -        
     find_right ------->|[input]4
                        |strlen=1 oper=-
                        |i=0) left_expr: 4
                        |  1) string end
     right_expr: 4 <----|     get_right
     offset: 2 <--------|     offset=i+1;
     left_expr:
        -+1*234
  6)
     string end
     output the left_expr
*/
/// @brief from string to expr
/// @param Istr [input]the string will read.
/// @param len [input]the len of string will read.
/// @param expr_head [output]the head-point of expr.
/// @return the result of the func-loop. 0 compelete. 1 false.
unsigned char expr_build(char *Istr,int len,expr_elem *&expr_head);

/// @brief from string to expr
/// @param Istr [input]the string will read.
/// @param len [input]the len of string will read.
/// @param expr_head [output]the head-point of expr.
/// @return the result of the func-loop. 0 compelete. 1 false.
unsigned char expr_build_ex(char *Istr,int len,expr_elem *&expr_head);

void expr_elem_free(expr_elem *&);

void expr_list_show(expr_elem &);
void expr_list_show(expr_elem &,const char*,int);

void expr_printf(expr_elem &);
#endif //_EXPR_ELEM_H_