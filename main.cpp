//#include <stdio.h>
#include "math_vector.h"
#include "math_expr.h"
#include <iostream>
#include <string.h>
using std::cout;

void test_for(int (&a)[3]){
    for(int i:a)
        //printf("%d\n",i);
        cout<<i<<"\t";
    cout<<"\n";
}

void test(int *&a){
    a=nullptr;
}

int main(void){
    int date[3]={1,2,3};
    float date0[5]={5,4,3,2,0};

    //printf("main.cpp\n");
    cout<<"main.cpp\n";

    math_vector a(date,3);
    a.show();

    date[1]=5;
    math_vector b(date0,5,0);
    b.show();

    math_vector c(zero_vector);
    c.show();

    c=b;
    c.show();

    if(a!=b){
        cout<<"c==b\n";
    }

    c=a+b;
    c.show();

    c=a-b;
    c.show();

    char str[]="1/6+y^(2+6)^4*3^4/6-5/6";
    int offset=0,loop=0,ret=0;;
    expr_elem *expr=nullptr;

    ret=expr_build(str,strlen(str),expr);
    if(ret==0) {
        cout<<"expr_build\n";
        expr_list_show(*expr);
        expr_printf(*expr);
        cout<<get_expr_len(*expr)<<"\n";
        expr_elem_free(expr);
    }

    ret=expr_build_ex(str,strlen(str),expr);
    if(ret==0) {
        cout<<"expr_build_ex\n";
        expr_list_show(*expr);
        expr_printf(*expr);
        cout<<get_expr_len(*expr)<<"\n";
        //expr_elem_free(expr);
    }

    expr_elem *ex_expr=expr_expend(expr);
    if(ex_expr!=nullptr) {
        cout<<"expr_expend\n";
        expr_list_show(*ex_expr);
        expr_printf(*ex_expr);
        cout<<get_expr_len(*ex_expr)<<"\n";
        expr_elem_free(ex_expr);
    }

    expr_elem *copy_expr_test=expr_copy(expr);
    if(copy_expr_test!=nullptr){
        cout<<"expr_copy\n";
        expr_list_show(*copy_expr_test);
        expr_printf(*copy_expr_test);
        cout<<get_expr_len(*copy_expr_test)<<"\n";
        expr_elem_free(copy_expr_test);
    }
    cout<<"main.cpp end\n";
    return 0;
}
/*test recode
expr    1+2*3^4
output  + 1 * 2 ^ 3 4
exprr   1^2*3+4
output  + * ^ 1 2 3 4
expr    1+(2-3)*4^5/6
output  + 1 * - 2 3 / ^ 4 5 6
expr    1+2*3^4/5-6
output  + 1 - * 2 / ^ 3 4 5 6
expr    1+(2-3)*4^5/6+7
output  + 1 + * - 2 3 / ^ 4 5 6 7
expr    1+(2-y)*4^x/6+7
output  + 1 + * - 2 y / ^ 4 x 6 7
expr    1+2*3^4-5/6
output  + 1 - * 2 ^ 3 4 / 5 6
expr    (1+(y)*3)^4-5/6
output  - ^ + 1 * y 3 4 / 5 6
expr    (1+(y^2)*3)^4-5/6
output  - ^ + 1 * ^ y 2 3 4 / 5 6
expr    (1+(y^2*3)^4-5)/6
output  / + 1 - ^ * ^ y 2 3 4 5 6
expr    (1+(y^2+6*3)^4-5)/6
output  / + 1 - ^ + ^ y 2 * 6 3 4 5 60
expr    (1+(y^(2+6)*3)^4-5)/6
output  / + 1 - ^ * ^ y + 2 6 3 4 5 6 
expr    1/6+((y^(2+6)*3)^4-5)/6
output  + / 1 6 / - ^ * ^ y + 2 6 3 4 5 6
expr    1/6+(y^(2+6)*3)^4/6-5/6
output  + / 1 6 - / ^ * ^ y + 2 6 3 4 6 / 5 6
expr    1/6+y^(2+6)^4*3^4/6-5/6
output  + / 1 6 - * ^ y ^ + 2 6 4 / ^ 3 4 6 / 5 6
expr    1/6+y^(2+6)^4*3^4/6-5/6
output  + / 1 6 - / * ^ ^ y + 2 6 4 ^ 3 4 6 / 5 6
*/