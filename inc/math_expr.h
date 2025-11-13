#ifndef _MATH_EXPR_H
#define _MATH_EXPR_H

//The count limit of elem of expression.
#define EXPR_BUILD_LOOP_DEEP_MAX 20

typedef struct expr_elem_type{
    expr_elem_type *next=nullptr;
    int elem_deep=0;//recode the deep of expression.
    unsigned char type=1;//0-char 1-int
    char oper='\n';//Binary func?expend to func?
    int numb=0;
    expr_elem_type();
    ~expr_elem_type();
}expr_elem;

//(num0 oper num1) such as 1+2
expr_elem *expr_set(char oper,int num0, int num1);
expr_elem *expr_set(char oper,int num, expr_elem &expr);
expr_elem *expr_set(char oper,expr_elem &expr, int num);
expr_elem *expr_set(char oper,expr_elem &expr0, expr_elem &expr1);

expr_elem* expr_find_end(expr_elem &);
int get_expr_len(expr_elem &);

/// @brief from string to expr
/// @param Istr [input]the string will read.
/// @param len [input]the len of string will read.
/// @param expr_head [output]the head-point of expr.
/// @return the result of the func-loop. 0 false. 1 compelete.
unsigned char expr_build(char *Istr,int len,expr_elem *&expr_head);

void expr_elem_free(expr_elem &);

void expr_list_show(expr_elem &);

void expr_printf(expr_elem &);

//expression read-only
class math_expr{
    private:
        expr_elem *expr;//record expr_head
        int len;
    public:
    math_expr();
    explicit math_expr(char *srt,int len);



    /// @brief append expression.
    /// @param new_expr new expression.
    /// @param operate thee sign of the operation.
    /// @param LR the site of new expression. 0 means Left-part of operation. 1 means Right-part of operation.
    void add_expr(math_expr &new_expr,char operate,bool LR);

    ~math_expr();

};
#endif //_MATH_EXPR_H