#include "math_expr.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

using std::cout;
#define OUTPUT_INFO __FILE__<<": "<<__LINE__<<": "<<__FUNCTION__<<": "
char sign_tools[]={'+','-','*','/','^','(',')'};
enum {SIGN_PLUS,SIGN_SUB,SIGN_MULT,SIGN_DIVIDE,SIGN_EXP,SIGN_LEFT,SIGN_RIGHT,SIGN_TOOLS_MAX};//need sync with sign_tools[]
//TYPE_OPER_0:+-
//TYPE_OPER_1:*/
//TYPE_OPER_2:^
enum {TYPE_NUM=1,TYPE_OPER_0,TYPE_OPER_1,TYPE_OPER_2,TYPE_LEFT,TYPE_RIGHT,TYPE_MAX};
unsigned char char_Analysis(char C){
    if(C>='0'&&C<='9')
        return TYPE_NUM;
    else{
        int i=0;
        for(i=0;i<(SIGN_TOOLS_MAX);++i){
            if(C==sign_tools[i]) break;
        }

        if(i<SIGN_MULT) return TYPE_OPER_0;
        else if(i<SIGN_EXP) return TYPE_OPER_1;
        else if(i<SIGN_LEFT) return TYPE_OPER_2;
        else if(i==SIGN_LEFT) return TYPE_LEFT;
        else if(i==SIGN_RIGHT) return TYPE_RIGHT;

        return TYPE_MAX;
    }
}

void expr_add_elem(expr_type &expr,unsigned char type,void *elem){
    expr.type=type;
    if(type==0){
        expr.oper=*(char*)elem;
    }else if(type==1){
        expr.numb=*(int*)elem;
    }
}

expr_type* expr_find_end(expr_type &expr){
    expr_type *expr_end=&expr;

    for(int i=0;i<EXPR_BUILD_LOOP_DEEP_MAX;++i){
        if(expr_end->next==nullptr) return expr_end;
        expr_end=expr_end->next;
    }
    return nullptr;
}

expr_type *expr_set(char oper,int num0, int num1){
    expr_type *expr_head=nullptr;
    expr_head=new expr_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_type_free(*expr_head);
        return nullptr;
    }

    expr_type *write_p=expr_head;
    expr_add_elem(*write_p,0,&oper);

    write_p->next=new expr_type();
    if(write_p->next==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_type_free(*expr_head);
        return nullptr;
    }
    write_p=write_p->next;
    expr_add_elem(*write_p,1,&num0);

    write_p->next=new expr_type();
    if(write_p->next==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_type_free(*expr_head);
        return nullptr;
    }
    write_p=write_p->next;
    expr_add_elem(*write_p,1,&num1);

    return expr_head;
}

expr_type *expr_set(char oper,int num, expr_type &expr){
    expr_type *expr_head=nullptr;
    expr_head=new expr_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_type_free(*expr_head);
        return nullptr;
    }

    expr_type *write_p=expr_head;
    expr_add_elem(*write_p,0,&oper);

    write_p->next=new expr_type();
    if(write_p->next==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_type_free(*expr_head);
        return nullptr;
    }
    write_p=write_p->next;
    expr_add_elem(*write_p,1,&num);

    write_p->next=&expr;
    return expr_head;
}

expr_type *expr_set(char oper,expr_type &expr, int num){
    expr_type *expr_head=nullptr;
    expr_head=new expr_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_type_free(*expr_head);
        return nullptr;
    }

    expr_type *write_p=expr_head;
    expr_add_elem(*write_p,0,&oper);

    write_p->next=&expr;
    write_p=expr_find_end(*(write_p->next));

    write_p->next=new expr_type();
    if(write_p->next==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_type_free(*expr_head);
        return nullptr;
    }
    expr_add_elem(*(write_p->next),1,&num);

    return expr_head;
}

expr_type *expr_set(char oper,expr_type &expr0, expr_type &expr1){
    expr_type *expr_head=nullptr;
    expr_head=new expr_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_type_free(*expr_head);
        return nullptr;
    }

    expr_type *write_p=expr_head;
    expr_add_elem(*write_p,0,&oper);

    write_p->next=&expr0;
    write_p=expr_find_end(*(write_p->next));

    write_p->next=&expr1;

    return expr_head;
}


//input one by one
unsigned char expr_build(char *input_str,int len,int &offset,expr_type **expr,int &loop_deep){

    if(input_str==nullptr || len==0){
        cout<<OUTPUT_INFO<<"input_str is nullptr.\n";
        return 0;
    }

    if(loop_deep>=EXPR_BUILD_LOOP_DEEP_MAX){
        cout<<OUTPUT_INFO<<"it`s too long to build.\n";
        return 0;
    }
    ++loop_deep;

    expr_type *expr_head=nullptr;
    expr_type *expr_end=nullptr;
    expr_type *write_oper=nullptr;
    expr_type *expr_cont=nullptr;//it`s a cont of a numb-data or expression.

    int num_bit=0;
    for(int i=0,type=0,pre_type=TYPE_OPER_0;i<len;++i){
        type=char_Analysis(*(input_str+i));

        if(type==TYPE_MAX){
            cout<<OUTPUT_INFO<<"sign out of range\n";
            expr_type_free(*expr_head);
            return 0;
        }else if(type==TYPE_NUM){//record the bit of num_str
            ++num_bit;
        }else{
            if(num_bit!=0){// the insert of num.it will insert the end of expr.
                char * num_str=(char*)malloc(num_bit+1);
                memcpy(num_str,input_str+i-num_bit,num_bit);
                num_str[num_bit]='\0';

                int num=atoi(num_str); // output number with int-type.
                if(expr_cont==nullptr){//case expr_head had beed initialization, so need malloc space in here.
                    expr_cont=new expr_type();
                    if(expr_cont==nullptr){
                        cout<<OUTPUT_INFO<<"malloc failed!!\n";
                        expr_type_free(*expr_head);
                        return 0;
                    }
                }
                expr_add_elem(*expr_cont,1,&num);
                cout<<OUTPUT_INFO<<"numb "<<num<<"\n";
            }

            //需要考虑括号与数字的组合关系
            if(type==TYPE_RIGHT){//the end of this expr_build.
                cout<<OUTPUT_INFO<<"TYPE_RIGHT\n";
                if(expr_cont!=nullptr){//insert the numb-data at the end of expr.
                    expr_end->next=expr_cont;
                    expr_end=expr_find_end(*expr_cont);

                    expr_cont=nullptr;
                }
                *expr=expr_head;
                ++offset;
                return 1;
            }else if(type==TYPE_LEFT){//into a new expr_build-function.
                int offset=0;//考虑到打印时应该注意括号的添加情况
                expr_type *sub_expr=nullptr;
                cout<<OUTPUT_INFO<<"TYPE_LEFT\n";
                if(expr_build(input_str+i+1,len-i-1,offset,&sub_expr,loop_deep)==0){
                    expr_type_free(*expr_head);
                    return 0;
                }
                i+=offset;
                cout<<OUTPUT_INFO<<offset<<"|"<<*(input_str+i)<<"|TYPE_LEFT\n";
                expr_cont=sub_expr;

                //pre_type=TYPE_RIGHT;
                cout<<OUTPUT_INFO<<"|TYPE_LEFT\n";
            }else /* if(type==TYPE_OPER) */{//the insert of oper.it will insert the head of expr.
                cout<<OUTPUT_INFO<<pre_type<<"|"<<type<<":"<<*(input_str+i)<<"\n";
                expr_type *new_expr=new expr_type();
                if(new_expr==nullptr){
                    cout<<OUTPUT_INFO<<"malloc failed!!\n";
                    expr_type_free(*expr_head);
                    return 0;
                }
                expr_add_elem(*new_expr,0,input_str+i);

                //append oper.
                if(expr_head==nullptr){
                    cout<<OUTPUT_INFO<<"expr_head==nullptr\n";
                    expr_head=new_expr;
                    expr_end=new_expr;
                    write_oper=expr_head;
                }else{
                    if(pre_type>=type){//insert the oper-data at the start of expr.it means the expression will be the left one.
                        cout<<OUTPUT_INFO<<"pre_type>=type\n";
                        if(write_oper==expr_head){
                            new_expr->next=expr_head;
                            expr_head=new_expr;
                        }else{
                            new_expr->next=write_oper->next;
                            write_oper->next=new_expr;
                        }
                    }else{//insert the oper-data at the end of expr.it means there is a new expression.
                        cout<<OUTPUT_INFO<<"pre_type<type\n";
                        write_oper=expr_end;
                        expr_end->next=new_expr;
                        expr_end=expr_end->next;
                    }
                }

                //append cache expr.
                if(expr_cont!=nullptr){//insert the numb-data at the end of expr.
                    expr_end->next=expr_cont;
                    expr_end=expr_find_end(*expr_cont);

                    expr_cont=nullptr;
                }

                expr_list_show(*expr_head);

                pre_type=type;//record the type of pre-oper
            }

            num_bit=0;
        }

        ++offset;
    }

    cout<<OUTPUT_INFO<<"|TYPE_LEFT\n";
    if(num_bit!=0){// the insert of num.it will insert the end of expr.
        char * num_str=(char*)malloc(num_bit+1);
        memcpy(num_str,input_str+len-num_bit,num_bit);
        num_str[num_bit]='\0';

        int num=atoi(num_str); // output number with int-type.
        expr_cont=new expr_type();
        if(expr_cont==nullptr){
            cout<<OUTPUT_INFO<<"malloc failed!!\n";
            expr_type_free(*expr_head);
            return 0;
        }
        expr_add_elem(*expr_cont,1,&num);
        expr_end->next=expr_cont;
        expr_end=expr_end->next;
    }

    cout<<OUTPUT_INFO<<"|TYPE_LEFT\n";
    *expr=expr_head;
    return 1;
}

void expr_type_free(expr_type &data){
    expr_type *free_p=&data;
    expr_type *next_p=nullptr;
    do{
        if(free_p->next!=nullptr) next_p=free_p->next;
        
        free(free_p);
        free_p=next_p;
    }while(free_p==nullptr);
}

void expr_list_show(expr_type &expr){
    cout<<__FUNCTION__<<":";
    expr_type *p=&expr;
    while(1){
        if(p->type==0){
            cout<<p->oper<<" ";
        }else if(p->type==1){
            cout<<p->numb<<" ";
        }

        if(p->next==nullptr) break;

        p=p->next;
    }
    cout<<"\n";
}

void expr_printf(expr_type &expr_head){
    
}

math_expr::math_expr():expr(nullptr)
{
    ;
}

math_expr::math_expr(char *srt, int len):expr(nullptr)
{
    expr=new expr_type();
    if(expr==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        return;
    }

    expr_type *input_p=expr;
    expr_type *input_np=expr;//next_prt
    expr_type *pre_input_p=nullptr;

    char *cprt=nullptr;
    cprt=srt;
    for(int i=0,ret=0,num_bit=0;i<len;++i){
        ret=char_Analysis(*(cprt));

        if(ret==SIGN_TOOLS_MAX){
            cout<<OUTPUT_INFO<<"sign out of range\n";
            expr_type_free(*expr);
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
    expr_type *free_p=expr;
    expr_type *next_p=nullptr;
    do{
        if(free_p->next!=nullptr) next_p=free_p->next;
        
        delete free_p;
        free_p=next_p;
    }while(free_p==nullptr);
}

expr_type::expr_type()
{
    next=nullptr;
    type=1;
    numb=0;
    oper='\0';
}
