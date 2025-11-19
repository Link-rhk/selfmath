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

    char str[]="1+(2-y)*4^x/6+7";
    int offset=0,loop=0,ret=0;;
    expr_elem *expr=nullptr;

    ret=expr_build(str,strlen(str),expr);
    if(ret==1) {
        expr_list_show(*expr);
        expr_printf(*expr);
        cout<<get_expr_len(*expr)<<"\n";
        expr_elem_free(expr);
    }

/*     {
        expr_elem *a=expr_set('+',10,"xx");
        expr_list_show(*a);
        cout<<get_expr_len(*a)<<"\n";
        expr_elem *b=expr_set('*',*a,5);
        // expr_elem_free(a);
        expr_list_show(*b);
        cout<<get_expr_len(*b)<<"\n";
        expr_printf(*b);
        expr_printf(*a);
        expr_elem_free(b);
        expr_elem_free(a);
        // cout<<b<<"\n";
    } */

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
expr    x
*/