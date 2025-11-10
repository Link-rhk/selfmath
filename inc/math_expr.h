#ifndef _MATH_EXPR_H
#define _MATH_EXPR_H

typedef struct expr_type{
    expr_type *next=nullptr;
    unsigned char type=1;//0-char 1-int
    char oper='\n';//Binary func?expend to func?
    int numb=0;
    expr_type();
}expr_type;

//(num0 oper num1) such as 1+2
expr_type *expr_set(char oper,int num0, int num1);
expr_type *expr_set(char oper,int num, expr_type &expr);
expr_type *expr_set(char oper,expr_type &expr, int num);
expr_type *expr_set(char oper,expr_type &expr0, expr_type &expr1);

expr_type* expr_find_end(expr_type &expr);

#define EXPR_BUILD_LOOP_DEEP_MAX 20
/// @brief from string to expr by func-loop
/// @param input_str [input]the string will read.
/// @param len [input]the len of string will read.
/// @param offset [output]the offset of string, which has beed read in the func-loop.
/// @param expr_head [output]the head-point of expr.
/// @param loop_deep [input]the deep of func-loop.for stopping the loop,when it`s too deep.
/// @return the result of the func-loop. 0 false. 1 compelete.
unsigned char expr_build(char *input_str,int len,int &offset,expr_type **expr,int &loop_deep);

void expr_type_free(expr_type &);

void expr_list_show(expr_type &);

void expr_printf(expr_type &);

//expression read-only
class math_expr{
    private:
        expr_type *expr;//record expr_head
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