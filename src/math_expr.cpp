#include "math_expr.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

using std::cout;
#define OUTPUT_INFO __FILE__<<": "<<__LINE__<<": "<<__FUNCTION__<<": "

math_expr::math_expr():expr(nullptr)
{
    ;
}

math_expr::math_expr(char *srt, int len):expr(nullptr)
{
/*     expr=new expr_elem_type();
    if(expr==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        return;
    }

    expr_elem *input_p=expr;
    expr_elem *input_np=expr;//next_prt
    expr_elem *pre_input_p=nullptr;

    char *cprt=nullptr;
    cprt=srt;
    for(int i=0,ret=0,num_bit=0;i<len;++i){
        ret=char_Analysis(*(cprt));

        if(ret==SIGN_TOOLS_MAX){
            cout<<OUTPUT_INFO<<"sign out of range\n";
            expr_elem_free(*expr);
            return;
        }else if(ret=SIGN_TOOLS_MAX+1){//record the bit of num_str
            ++num_bit;
        }else if(ret<SIGN_TOOLS_MAX){
            if(num_bit!=0){
                char * num_str=(char*)malloc(num_bit+1);
                memcpy(num_str,cprt+i-num_bit,num_bit);
                num_str[num_bit]='\0';

                int num=atoi(num_str); // output number with int-type.
            }


            num_bit=0;
        }
    }
 */
}

void math_expr::add_expr(math_expr &new_expr,char operate,bool LR)
{
/*     bool expr_malloc=false;
    if(expr==nullptr){
        expr=new expr_type();
        if(expr==nullptr){
            cout<<OUTPUT_INFO<<"malloc failed!!\n";
            return;
        }
        expr_malloc=true;
    }

    //Analysis str
    char *cprt=nullptr,*num_prt=nullptr;
    int len=0;//strlen(str);

    for(int i=0,j=0,ret=0;i<len;++i){
        ret=char_Analysis(*(cprt));

        if(ret==SIGN_TOOLS_MAX){
            cout<<OUTPUT_INFO<<"sign out of range\n";
            if(expr_malloc) free(expr);
            return;
        }else if(ret=SIGN_TOOLS_MAX+1){
            if(num_prt==nullptr)
                num_prt=cprt+i;
        }else if(ret<SIGN_TOOLS_MAX){
            if(num_prt!=nullptr){
                char * num_str=(char*)malloc(i-(num_prt-cprt)+1);
                memcpy(num_str,num_prt,i-(num_prt-cprt));
                num_str[i-(num_prt-cprt)]='\0';

                int num=atoi(num_str); // output number with int-type.
            }


            num_prt=nullptr;
        }
    } */


    //write expr
    
}

math_expr::~math_expr()
{
    expr_elem *free_p=expr;
    expr_elem *next_p=nullptr;
    do{
        if(free_p->next!=nullptr) next_p=free_p->next;
        
        delete free_p;
        free_p=next_p;
    }while(free_p==nullptr);
}

expr_oper_func_type::expr_oper_func_type():label(nullptr),func(nullptr)
{

}

expr_oper_func_type::expr_oper_func_type(char *label, expr_elem &func):label(label),func(&func)
{
}

expr_oper_func_type::~expr_oper_func_type()
{
    if(label!=nullptr) free(label);
     if(func!=nullptr)   delete func;
}

// void expr_oper_func_build_test(void){}
