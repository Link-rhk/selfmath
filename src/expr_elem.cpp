#include "expr_elem.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

using std::cout;
#define OUTPUT_INFO __FILE__<<": "<<__LINE__<<": "<<__FUNCTION__<<": "

//limit set 0 will use CHAR_LIST_LEN_MAX
int strlen_limit(char *&str,unsigned int limit){
    if(str==nullptr) return 0;

    for(int i=0;i<CHAR_LIST_LEN_MAX;++i){
        if(*(str+i)=='\0') return i;
    }
    cout<<OUTPUT_INFO<<"[WARNNING]strlen is out of limit(%d)!!\n",CHAR_LIST_LEN_MAX;
    return CHAR_LIST_LEN_MAX;
}

//limit set 0 will use CHAR_LIST_LEN_MAX
int strcpy_limit(char *&Str,char *&Ostr,unsigned int limit){
    int len=strlen_limit(Ostr,limit);

    for(int i=0;i<len;++i){
        *(Str+i)=*(Ostr+i);
    }
    if(len!=0) *(Str+len)='\0';

    return len;
}

char sign_tools[]={'+','-','*','/','^','(',')'};
enum {SIGN_PLUS,SIGN_SUB,SIGN_MULT,SIGN_DIVIDE,SIGN_EXP,SIGN_LEFT,SIGN_RIGHT,SIGN_TOOLS_MAX};//need sync with sign_tools[]
//TYPE_OPER_0:+-
//TYPE_OPER_1:*/
//TYPE_OPER_MAX:^ or func
enum {TYPE_NUM=1,TYPE_LETTER,TYPE_OPER_0,TYPE_OPER_1,TYPE_OPER_MAX,TYPE_LEFT,TYPE_RIGHT,TYPE_MAX};
unsigned char char_Analysis(char &C){
    if(C>='0'&&C<='9')
        return TYPE_NUM;
    else if((C>='a' && C<='z') || (C>'A' && C<='Z'))
        return TYPE_LETTER;
    else{
        int i=0;
        for(i=0;i<(SIGN_TOOLS_MAX);++i){
            if(C==sign_tools[i]) break;
        }

        if(i<SIGN_MULT) return TYPE_OPER_0;
        else if(i<SIGN_EXP) return TYPE_OPER_1;
        else if(i<SIGN_LEFT) return TYPE_OPER_MAX;
        else if(i==SIGN_LEFT) return TYPE_LEFT;
        else if(i==SIGN_RIGHT) return TYPE_RIGHT;

        return TYPE_MAX;
    }
}

/// @brief unpdate the deep of expression, while EXPR_BUILD_LOOP_DEEP_MAX is defined.
/// @param expr the head of expression, need update the deep-recode.
/// @param deep the deep of first expr_elem.
/// @return the deep of the end of expr_elem.
int expr_deep_update(expr_elem &expr, int deep){
#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    expr.elem_deep=deep;
    expr_elem *expr_next=expr.next;
    int i=deep;
    while(expr_next!=nullptr){
        ++i;
        expr_next->elem_deep=i;
        if(i>=EXPR_BUILD_LOOP_DEEP_MAX) break;
        expr_next=expr_next->next;
    }

    if(i>=EXPR_BUILD_LOOP_DEEP_MAX)
        cout<<OUTPUT_INFO<<"WARNNING!! it`s out of limit.\n";

    return i;
#else
    return 0;
#endif
}

int get_expr_len(expr_elem &expr){
#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    expr_elem *expr_next=&expr;
    int deep=expr_next->elem_deep;
    while(expr_next->next!=nullptr){
        expr_next=expr_next->next;
        deep=expr_next->elem_deep;
        if(deep>=EXPR_BUILD_LOOP_DEEP_MAX) break;
    }

    if(deep>=EXPR_BUILD_LOOP_DEEP_MAX)
        cout<<OUTPUT_INFO<<"WARNNING!! it`s out of limit.\n";

    return deep+1;//sign to count
#else
    return 0;
#endif
}

#define DEFUALT_LABEL "label"
/// @return the deep for the expression. if return -1 means error input.
int expr_init_elem(expr_elem &expr, int deep, unsigned char type, void *elem){
    expr.type=type;
    if(type==0){
        int char_type=char_Analysis(*(char*)elem);
        if(char_type>=TYPE_OPER_0 && char_type<=TYPE_OPER_MAX)
            expr.oper=*(char*)elem;
        else{
            cout<<OUTPUT_INFO<<"illegal char \"%c\"\n",*(char*)elem;
            return -1;
        }
    }else if(type==1){
        expr.numb=*(int*)elem;
    }else if(type==2){
        char *str=(char*)elem;
        int len=strlen_limit(str,0);
        if(len==0){
            expr.label=(char*)malloc(strlen(DEFUALT_LABEL)+1);
            sprintf(expr.label,"%s",DEFUALT_LABEL);
        }else{
            expr.label=(char*)malloc(len+1);

            for(int i=0;i<len;++i){
                if(char_Analysis(*(str+i))>=TYPE_MAX){
                    cout<<OUTPUT_INFO<<"illegal char "<<*(str+i)<<"\n";
                    free(expr.label);
                    return -1;
                }

                *(expr.label+i)=*(str+i);
            }

            *(expr.label+len)='\0';
        }

    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    return expr_deep_update(expr,deep);
#else
    return 0;
#endif
}

/// @return the deep for the expression. if return -1 means error input.
int expr_init_elem(expr_elem &expr, int deep, expr_elem &Sexpr){
    expr.type=Sexpr.type;
    expr.elem_deep=deep;
    expr.numb=Sexpr.numb;
    expr.oper=Sexpr.oper;
    if(Sexpr.label!=nullptr){
        int len=strlen_limit(Sexpr.label,EXPR_LABEL_LEN_MAX);
        expr.label=(char*)malloc(len+1);
        snprintf(expr.label,len+1,"%s",Sexpr.label);
    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    return expr_deep_update(expr,deep);
#else
    return 0;
#endif
}

expr_elem* expr_find_end(expr_elem &expr){
    expr_elem *expr_end=&expr;

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    for(int i=0;i<EXPR_BUILD_LOOP_DEEP_MAX;++i)
#else
    while(1)
#endif
    {
        if(expr_end->next==nullptr) return expr_end;
        expr_end=expr_end->next;
    }
    cout<<OUTPUT_INFO<<"it out of limit!!\n";
    return nullptr;
}

int expr_set_append_num(int &num, expr_elem *&expr);
int expr_set_append_label(char *&label, expr_elem *&expr);
int expr_set_append_expr(expr_elem &Sexpr, expr_elem *&expr);

expr_elem *expr_set(char oper,int num0, int num1){
    expr_elem *expr_head=nullptr;
    expr_head=new expr_elem_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    int deep_ret=0;
    expr_elem *write_p=expr_head;
    deep_ret=expr_init_elem(*write_p,0,0,&oper);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"oper add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    deep_ret=expr_set_append_num(num0,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    // write_p->next=new expr_elem_type();
    // if(write_p->next==nullptr){
    //     cout<<OUTPUT_INFO<<"malloc failed!!\n";
    //     expr_elem_free(expr_head);
    //     return nullptr;
    // }
    
    // deep_ret=expr_init_elem(*(write_p->next),write_p->elem_deep+1,1,&num0);

    write_p=write_p->next;

    deep_ret=expr_set_append_num(num1,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    if(deep_ret>=EXPR_BUILD_LOOP_DEEP_MAX){
        cout<<OUTPUT_INFO<<"[WARNNING]the expression is out of limit[%s]\n",deep_ret;
    }
#endif

    return expr_head;
}

expr_elem *expr_set(char oper,int num, expr_elem &expr){
    expr_elem *expr_head=nullptr;
    expr_head=new expr_elem_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    int deep_ret=0;
    expr_elem *write_p=expr_head;
    deep_ret=expr_init_elem(*write_p,0,0,&oper);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"oper add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    deep_ret=expr_set_append_num(num,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    write_p=write_p->next;

    deep_ret=expr_set_append_expr(expr,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    if(deep_ret>=EXPR_BUILD_LOOP_DEEP_MAX){
        cout<<OUTPUT_INFO<<"[WARNNING]the expression is out of limit[%s]\n",deep_ret;
    }
#endif

    return expr_head;
}

expr_elem *expr_set(char oper,expr_elem &expr, int num){
    expr_elem *expr_head=nullptr;
    expr_head=new expr_elem_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    int deep_ret=0;
    expr_elem *write_p=expr_head;
    deep_ret=expr_init_elem(*write_p,0,0,&oper);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"oper add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    deep_ret=expr_set_append_expr(expr,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    write_p=expr_find_end(*(write_p->next));

    deep_ret=expr_set_append_num(num,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    if(deep_ret>=EXPR_BUILD_LOOP_DEEP_MAX){
        cout<<OUTPUT_INFO<<"[WARNNING]the expression is out of limit[%s]\n",deep_ret;
    }
#endif

    return expr_head;
}

expr_elem *expr_set(char oper,expr_elem &expr0, expr_elem &expr1){
    expr_elem *expr_head=nullptr;
    expr_head=new expr_elem_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    int deep_ret=0;
    expr_elem *write_p=expr_head;
    deep_ret=expr_init_elem(*write_p,0,0,&oper);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"oper add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    deep_ret=expr_set_append_expr(expr0,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }
    

    write_p=expr_find_end(*(write_p->next));
    deep_ret=expr_set_append_expr(expr1,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    if(deep_ret>=EXPR_BUILD_LOOP_DEEP_MAX){
        cout<<OUTPUT_INFO<<"[WARNNING]the expression is out of limit[%s]\n",deep_ret;
    }
#endif

    return expr_head;
}

expr_elem *expr_set(char oper,char* label0, char* label1){
    if(label0==nullptr || label1==nullptr){
        cout<<OUTPUT_INFO<<"nullptr input!!\n";
        return nullptr;
    }
    expr_elem *expr_head=nullptr;
    expr_head=new expr_elem_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    int deep_ret=0;
    expr_elem *write_p=expr_head;
    deep_ret=expr_init_elem(*write_p,0,0,&oper);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"oper add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    deep_ret=expr_set_append_label(label0,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"illegal char\n";
        expr_elem_free(expr_head);
        return nullptr;
    }
    write_p=write_p->next;

    deep_ret=expr_set_append_label(label1,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"illegal char\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    if(deep_ret>=EXPR_BUILD_LOOP_DEEP_MAX){
        cout<<OUTPUT_INFO<<"[WARNNING]the expression is out of limit[%s]\n",deep_ret;
    }
#endif

    return expr_head;
}

expr_elem *expr_set(char oper, char *label, int num)
{
    if(label==nullptr){
        cout<<OUTPUT_INFO<<"nullptr input!!\n";
        return nullptr;
    }
    expr_elem *expr_head=nullptr;
    expr_head=new expr_elem_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    int deep_ret=0;
    expr_elem *write_p=expr_head;
    deep_ret=expr_init_elem(*write_p,0,0,&oper);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"oper add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    deep_ret=expr_set_append_label(label,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"illegal char\n";
        expr_elem_free(expr_head);
        return nullptr;
    }
    write_p=write_p->next;

    deep_ret=expr_set_append_num(num,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    if(deep_ret>=EXPR_BUILD_LOOP_DEEP_MAX){
        cout<<OUTPUT_INFO<<"[WARNNING]the expression is out of limit[%s]\n",deep_ret;
    }
#endif

    return expr_head;
}

expr_elem *expr_set(char oper, int num, char *label)
{
    if(label==nullptr){
        cout<<OUTPUT_INFO<<"nullptr input!!\n";
        return nullptr;
    }
    expr_elem *expr_head=nullptr;
    expr_head=new expr_elem_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    int deep_ret=0;
    expr_elem *write_p=expr_head;
    deep_ret=expr_init_elem(*write_p,0,0,&oper);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"oper add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    deep_ret=expr_set_append_num(num,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }
    write_p=write_p->next;

    deep_ret=expr_set_append_label(label,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"illegal char\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    if(deep_ret>=EXPR_BUILD_LOOP_DEEP_MAX){
        cout<<OUTPUT_INFO<<"[WARNNING]the expression is out of limit[%s]\n",deep_ret;
    }
#endif

    return expr_head;
}

expr_elem *expr_set(char oper, char *label, expr_elem &expr)
{
    if(label==nullptr){
        cout<<OUTPUT_INFO<<"nullptr input!!\n";
        return nullptr;
    }
    expr_elem *expr_head=nullptr;
    expr_head=new expr_elem_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    int deep_ret=0;
    expr_elem *write_p=expr_head;
    deep_ret=expr_init_elem(*write_p,0,0,&oper);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"oper add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    deep_ret=expr_set_append_label(label,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"illegal char\n";
        expr_elem_free(expr_head);
        return nullptr;
    }
    write_p=write_p->next;

    deep_ret=expr_set_append_expr(expr,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    if(deep_ret>=EXPR_BUILD_LOOP_DEEP_MAX){
        cout<<OUTPUT_INFO<<"[WARNNING]the expression is out of limit[%s]\n",deep_ret;
    }
#endif

    return expr_head;
}

expr_elem *expr_set(char oper, expr_elem &expr, char *label)
{
    if(label==nullptr){
        cout<<OUTPUT_INFO<<"nullptr input!!\n";
        return nullptr;
    }
    expr_elem *expr_head=nullptr;
    expr_head=new expr_elem_type();
    if(expr_head==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    int deep_ret=0;
    expr_elem *write_p=expr_head;
    deep_ret=expr_init_elem(*write_p,0,0,&oper);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"oper add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    deep_ret=expr_set_append_expr(expr,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"num add failed!!\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

    write_p=expr_find_end(*(write_p->next));

    deep_ret=expr_set_append_label(label,write_p);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"illegal char\n";
        expr_elem_free(expr_head);
        return nullptr;
    }

#ifdef EXPR_BUILD_LOOP_DEEP_MAX
    if(deep_ret>=EXPR_BUILD_LOOP_DEEP_MAX){
        cout<<OUTPUT_INFO<<"[WARNNING]the expression is out of limit[%s]\n",deep_ret;
    }
#endif

    return expr_head;
}

expr_elem* get_num_from_str(char *str,int num_bit);
expr_elem* get_label_from_str(char *str,int label_bit);
expr_elem* find_a_right_expr(char *Istr,int &len,unsigned char oper_lvl);

unsigned char expr_build(char *Istr,int len,expr_elem *&expr_head){
    if(Istr==nullptr || len==0){
        cout<<OUTPUT_INFO<<"error input.\n";
        return 0;
    }

    // cout<<OUTPUT_INFO<<"len "<<len<<"\n";
    // expr_elem *ret_expr=nullptr;
    expr_elem *left_expr=nullptr;

    int i=0,type=0,num_bit=0,label_bit=0;
    for(i=0;i<len;++i){
        type=char_Analysis(*(Istr+i));
        if(type==TYPE_MAX){
            cout<<OUTPUT_INFO<<"sign out of range\n";
            if(left_expr==nullptr)
                expr_elem_free(left_expr);
            return 0;
        }else if(type==TYPE_NUM){//record the bit of num_str
            if(label_bit!=0){// find a number it wiil be the part of left_expr os just left_expr.
                expr_elem *tmp_expr=get_label_from_str(Istr+i,num_bit);
                if(tmp_expr==nullptr){
                    cout<<OUTPUT_INFO<<"error!!\n";
                    return 0;
                }
                num_bit=0;

                if(left_expr==nullptr){
                    left_expr=tmp_expr;
                }else{// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*tmp_expr);
                }
            }

            ++num_bit;
        }else if(type==TYPE_LETTER){//record the bit of label_str
            if(num_bit!=0){// find a number it wiil be the part of left_expr os just left_expr.
                expr_elem *tmp_expr=get_num_from_str(Istr+i,num_bit);
                if(tmp_expr==nullptr){
                    cout<<OUTPUT_INFO<<"malloc failed!!\n";
                    return 0;
                }
                num_bit=0;

                if(left_expr==nullptr){
                    left_expr=tmp_expr;
                }else{// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*tmp_expr);
                }
            }

            ++label_bit;
        }else{
            if(num_bit!=0){// find a number it wiil be the part of left_expr os just left_expr.
                expr_elem *tmp_expr=get_num_from_str(Istr+i,num_bit);
                if(tmp_expr==nullptr){
                    cout<<OUTPUT_INFO<<"error\n";
                    return 0;
                }
                num_bit=0;

                if(left_expr==nullptr){
                    left_expr=tmp_expr;
                }else{// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*tmp_expr);
                }
            }//num_bit!=0 && label_bit!=0 can`t both happen
            else if(label_bit!=0){// find a number it wiil be the part of left_expr os just left_expr.
                expr_elem *tmp_expr=get_label_from_str(Istr+i,label_bit);
                if(tmp_expr==nullptr){
                    cout<<OUTPUT_INFO<<"error!!\n";
                    return 0;
                }
                label_bit=0;

                if(left_expr==nullptr){
                    left_expr=tmp_expr;
                }else{// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*tmp_expr);
                }
            }

            if(type==TYPE_RIGHT){//the end of a expression. but not in this function.
                cout<<OUTPUT_INFO<<"error!!\n";
                if(left_expr==nullptr)
                    expr_elem_free(left_expr);
                return 0;
            }else if(type==TYPE_LEFT){//find a new expression.
                int sub_len=len-i-1;
                //this expression is always a left-expression in this function(-loop).
                expr_elem* sub_expr=find_a_right_expr(Istr+i+1,sub_len,type);
                if(sub_expr==nullptr){
                    cout<<OUTPUT_INFO<<"error!!\n";
                    if(left_expr==nullptr)
                        expr_elem_free(left_expr);
                    return 0;
                }

                if(left_expr!=nullptr){// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*sub_expr);
                }else{
                    left_expr=sub_expr;
                }
                i=len-sub_len;//add the offset had been read.
                if(sub_len==0) break;//break the for-loop for skipping ++i;
            }else/* if(type==TYPE_OPER) */{//find an operation.it means there is a new expression.
                //check if left_expr is exsit.
                if(left_expr==nullptr){
                    cout<<OUTPUT_INFO<<"lack the expression in the left of operation!!\n";
                    return 0;
                }

                int sub_len=len-(i+1);//"+1" for transfor sign ot count of the str.
                // cout<<OUTPUT_INFO<<"sub_len "<<sub_len<<"\n";
                // cout<<OUTPUT_INFO<<*(Istr+i)<<"before\n";
                // expr_list_show(*left_expr);
                expr_elem* right_expr=find_a_right_expr(Istr+i+1,sub_len,type);
                // cout<<OUTPUT_INFO<<"sub_len "<<sub_len<<"\n";
                // cout<<OUTPUT_INFO<<*(Istr+len-sub_len-1)<<"after\n";
                // expr_list_show(*right_expr);
                if(right_expr==nullptr){
                    cout<<OUTPUT_INFO<<"error!!\n";
                    expr_elem_free(left_expr);
                    return 0;
                }
                //get the expression.
                left_expr=expr_set(*(Istr+i),*left_expr,*right_expr);
                // expr_list_show(*left_expr);
                i=len-sub_len;//add the offset had been read.
                if(sub_len==0) break;//break the for-loop for skipping ++i;
            }
        }
    }

    if(num_bit!=0){//find a number wait input
        expr_elem *tmp_expr=get_num_from_str(Istr+i,num_bit);
        if(tmp_expr==nullptr){
            cout<<OUTPUT_INFO<<"error\n";
            return 0;
        }
        num_bit=0;

        if(left_expr==nullptr){
            left_expr=tmp_expr;
        }else{// find a left_expr in front.
            //default a '*'operation between left_expr and parentheses.
            left_expr=expr_set('*',*left_expr,*tmp_expr);
        }
    }//num_bit!=0 && label_bit!=0 can`t both happen
    else if(label_bit!=0){// find a number it wiil be the part of left_expr os just left_expr.
        expr_elem *tmp_expr=get_label_from_str(Istr+i,label_bit);
        if(tmp_expr==nullptr){
            cout<<OUTPUT_INFO<<"error!!\n";
            return 0;
        }
        label_bit=0;

        if(left_expr==nullptr){
            left_expr=tmp_expr;
        }else{// find a left_expr in front.
            //default a '*'operation between left_expr and parentheses.
            left_expr=expr_set('*',*left_expr,*tmp_expr);
        }
    }
    expr_head=left_expr;
    return 1;
}

//build without func-loop trying
unsigned char expr_build_new(char *Istr,int len,expr_elem *&expr_head){
    if(Istr==nullptr || len==0){
        cout<<OUTPUT_INFO<<"error input.\n";
        return 0;
    }

    expr_elem *left_expr=nullptr;

    int i=0,type=0,num_bit=0;
    for(i=0;i<len;++i){
        type=char_Analysis(*(Istr+i));
        if(type==TYPE_MAX){
            cout<<OUTPUT_INFO<<"sign out of range\n";
            if(left_expr==nullptr)
                expr_elem_free(left_expr);
            return 0;
        }else if(type==TYPE_NUM){//record the bit of num_str
            ++num_bit;
        }else{
            if(num_bit!=0){//find a number wait input
                char * num_str=(char*)malloc(num_bit+1);
                memcpy(num_str,Istr+i-num_bit,num_bit);
                num_str[num_bit]='\0';

                int num=atoi(num_str); // output number with int-type.
                free(num_str);

                expr_elem *tmp_expr=new expr_elem_type();
                if(tmp_expr==nullptr){
                    cout<<OUTPUT_INFO<<"malloc failed!!\n";
                    expr_elem_free(left_expr);
                    return 0;
                }
                expr_init_elem(*tmp_expr,0,1,&num);

                if(left_expr==nullptr){
                    left_expr=tmp_expr;
                }else{// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*tmp_expr);
                }
            }
        }
    }

    return 0;
}

void expr_elem_free(expr_elem *&data){
    if(data!=nullptr)
        delete data;

    data=nullptr;
    // expr_elem *free_p=&data;
    // expr_elem *next_p=nullptr;
    // do{
    //     if(free_p->next!=nullptr) next_p=free_p->next;
        
    //     // free(free_p);
    //     delete free_p;
    //     free_p=next_p;
    // }while(free_p==nullptr);
}

void expr_list_show(expr_elem &expr){
    cout<<__FUNCTION__<<":";
    expr_elem *p=&expr;
    while(1){
        if(p->type==0){
            cout<<p->oper<<" ";
        }else if(p->type==1){
            cout<<p->numb<<" ";
        }else if(p->type==2){
            cout<<p->label<<" ";
        }

        // cout<<OUTPUT_INFO<<p<<"\n";
        if(p->next==nullptr) break;

        p=p->next;
    }
    cout<<"\n";
}

expr_elem* find_a_numb(expr_elem &expr_head, char **Estr_p, int *expr_level);

void expr_printf(expr_elem &expr_head){
    if((expr_head.type==1 || expr_head.type==2)
    && expr_head.next!=nullptr){//the first data is a number while it has a next data!!
        cout<<OUTPUT_INFO<<"form error!!\n";
        return;
    }else if(expr_head.type==1){//find a numb-data at the end.
        cout<<expr_head.numb<<"\n";
        return;
    }else if(expr_head.type==2){//find a label-data at the end.
        cout<<expr_head.label<<"\n";
        return;
    }else if(expr_head.next==nullptr){//find a oper-data at the end.
        cout<<OUTPUT_INFO<<"form error!!\n";
        return;
    }

    expr_elem* ret_expr=nullptr;//point to the end of expr which had printf.

    //the first data is an oper-data. and exsit next.
    char *subL_Estr=nullptr;
    int Lexpr_lvl=0;
    ret_expr=find_a_numb(*expr_head.next,&subL_Estr,&Lexpr_lvl);

    if(subL_Estr==nullptr){//left-expr not found.
        cout<<OUTPUT_INFO<<"ERROR\n";
        return;
    }

    if(ret_expr->next==nullptr){//lack a right expr of oper.
        cout<<OUTPUT_INFO<<"form error!!\n";
        free(subL_Estr);
        return;
    }

    char *subR_Estr=nullptr;
    int Rexpr_lvl=0;
    ret_expr=find_a_numb(*(ret_expr->next),&subR_Estr,&Rexpr_lvl);
    if(subR_Estr==nullptr){//right-expr not found.
        cout<<OUTPUT_INFO<<"ERROR\n";
        return;
    }

    char *Estr=nullptr;
    int expr_lvl=char_Analysis(expr_head.oper);
    // cout<<OUTPUT_INFO<<expr_lvl<<"|"<<Lexpr_lvl<<"|"<<Rexpr_lvl<<"|"<<"\n";
    if(Lexpr_lvl==TYPE_NUM 
    || Lexpr_lvl==TYPE_LETTER
    || Lexpr_lvl>=expr_lvl){
        Estr=(char *)malloc(strlen(subL_Estr)+1+strlen(subR_Estr)+2+1);
        sprintf(Estr,"%s%c",subL_Estr,expr_head.oper);
    }else{
        Estr=(char *)malloc(strlen(subL_Estr)+1+strlen(subR_Estr)+4+1);
        sprintf(Estr,"(%s)%c",subL_Estr,expr_head.oper);
    }

    //input right-expr to the Estr_p.
    if(Rexpr_lvl==TYPE_NUM 
    || Rexpr_lvl==TYPE_LETTER
    || Rexpr_lvl>=expr_lvl){
        sprintf(Estr,"%s%s",Estr,subR_Estr);
    }else{
        sprintf(Estr,"%s(%s)",Estr,subR_Estr);
    }

    // cout<<OUTPUT_INFO<<Estr<<"\n";
    free(subL_Estr);
    free(subR_Estr);

    cout<<Estr<<"\n";
    free(Estr);
}

//will return number-only if evey operation is had been defined.
char *expr_result(expr_elem &expr_head){
    
    return nullptr;
}

expr_elem_type::expr_elem_type()
{
    next=nullptr;
    elem_deep=0;
    type=1;
    numb=0;
    oper='\0';
    label=nullptr;
}

expr_elem_type::~expr_elem_type()
{
    if(label!=nullptr)
        free(label);
        
    if(next!=nullptr)
        delete next;
    next==nullptr;
    label=nullptr;
}


/// @return the deep for the expression. if return -1 means error input.
int expr_set_append_num(int &num, expr_elem *&expr){
    expr->next=new expr_elem_type();
    if(expr->next==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        // expr_elem_free(expr_head);
        return -1;
    }
    
    return expr_init_elem(*(expr->next),expr->elem_deep+1,1,&num);
}

/// @return the deep for the expression. if return -1 means error input.
int expr_set_append_label(char *&label, expr_elem *&expr){
    //check if have illegal char
    int len=strlen_limit(label,EXPR_LABEL_LEN_MAX);
    for(int i=0;i<len;++i){
        if(*(label+i)=='\n') break;

        if(char_Analysis(*(label+i))!=TYPE_LETTER){
            cout<<OUTPUT_INFO<<"illegal char\n";
            // expr_elem_free(expr_head);
            return -1;
        }
    }

    expr->next=new expr_elem_type();
    if(expr->next==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        // expr_elem_free(expr_head);
        return -1;
    }

    return expr_init_elem(*(expr->next),expr->elem_deep+1,2,label);
}

/// @return the deep for the expression. if return -1 means error input.
int expr_set_append_expr(expr_elem &Sexpr, expr_elem *&expr){
    expr->next=new expr_elem_type();
    if(expr->next==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        return -1;
    }

    expr_elem *write_p=expr->next;//nullptr here
    expr_elem *S_ptr=&Sexpr;
    int deep_ret=expr->elem_deep;
    deep_ret=expr_init_elem(*write_p,deep_ret+1,*S_ptr);
    if(deep_ret==-1){
        cout<<OUTPUT_INFO<<"error\n";
        return -1;
    }

    S_ptr=S_ptr->next;
    int i=0;
    while(1){
        if(S_ptr==nullptr) break;
        
        write_p->next=new expr_elem_type();
        if(write_p->next==nullptr){
            cout<<OUTPUT_INFO<<"malloc failed!!\n";
            expr_elem_free(expr->next);
            return -1;
        }

        deep_ret=expr_init_elem(*(write_p->next),deep_ret+1,*S_ptr);

        write_p=write_p->next;
        S_ptr=S_ptr->next;

        if(++i==20) break;
    }
    return deep_ret;
}

/// @brief find a right-expression of a oper in the Input string.
/// @param Istr input string.
/// @param len [input/output]the len of the string still need to read.
/// @param oper_lvl the level of the oper.
/// @return the expression found.
expr_elem* find_a_right_expr(char *Istr,int &len,unsigned char oper_lvl){
    if(Istr==nullptr || len==0 || oper_lvl>TYPE_LEFT){
        cout<<OUTPUT_INFO<<"error input.\n";
        return nullptr;
    }

    expr_elem *left_expr=nullptr;

    int i=0,type=0,num_bit=0,label_bit=0;
    for(i=0;i<len;++i){
        type=char_Analysis(*(Istr+i));
        if(type==TYPE_MAX){
            cout<<OUTPUT_INFO<<"sign out of range\n";
            if(left_expr!=nullptr)
                expr_elem_free(left_expr);
            return nullptr;
        }else if(type==TYPE_NUM){//record the bit of num_str
            ++num_bit;
            if(label_bit!=0){// find a number it wiil be the part of left_expr os just left_expr.
                expr_elem *tmp_expr=get_label_from_str(Istr+i,num_bit);
                if(tmp_expr==nullptr){
                    cout<<OUTPUT_INFO<<"error!!\n";
                    return 0;
                }
                num_bit=0;

                if(left_expr==nullptr){
                    left_expr=tmp_expr;
                }else{// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*tmp_expr);
                }
            }
        }else if(type==TYPE_LETTER){//record the bit of label_str
            if(num_bit!=0){// find a number it wiil be the part of left_expr os just left_expr.
                expr_elem *tmp_expr=get_num_from_str(Istr+i,num_bit);
                if(tmp_expr==nullptr){
                    cout<<OUTPUT_INFO<<"malloc failed!!\n";
                    return 0;
                }
                num_bit=0;

                if(left_expr==nullptr){
                    left_expr=tmp_expr;
                }else{// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*tmp_expr);
                }
            }

            ++label_bit;
        }else{
            if(num_bit!=0){// find a number it wiil be the part of left_expr os just left_expr.
                char *num_str=(char*)malloc(num_bit+1);
                memcpy(num_str,Istr+i-num_bit,num_bit);
                num_str[num_bit]='\0';

                int num=atoi(num_str); // output number with int-type.
                free(num_str);

                expr_elem *tmp_expr=new expr_elem_type();
                if(tmp_expr==nullptr){
                    cout<<OUTPUT_INFO<<"malloc failed!!\n";
                    return nullptr;
                }
                expr_init_elem(*tmp_expr,0,1,&num);
                num_bit=0;

                if(left_expr==nullptr){
                    left_expr=tmp_expr;
                }else{// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*tmp_expr);
                }
            }//num_bit!=0 && label_bit!=0 can`t both happen
            else if(label_bit!=0){// find a number it wiil be the part of left_expr os just left_expr.
                expr_elem *tmp_expr=get_label_from_str(Istr+i,label_bit);
                if(tmp_expr==nullptr){
                    cout<<OUTPUT_INFO<<"error!!\n";
                    return 0;
                }
                label_bit=0;

                if(left_expr==nullptr){
                    left_expr=tmp_expr;
                }else{// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*tmp_expr);
                }
            }

            if(type==TYPE_RIGHT){//the end of a expression.
                break;
            }else if(type==TYPE_LEFT){//find a new expression.
                int sub_len=len-i-1;
                //this expression is always a left-expression in this function(-loop).
                expr_elem* sub_expr=find_a_right_expr(Istr+i+1,sub_len,type);
                if(sub_expr==nullptr){
                    cout<<OUTPUT_INFO<<"lack right expression!!\n";
                    if(left_expr!=nullptr)
                        expr_elem_free(left_expr);
                    return nullptr;
                }

                if(left_expr!=nullptr){// find a left_expr in front.
                    //default a '*'operation between left_expr and parentheses.
                    left_expr=expr_set('*',*left_expr,*sub_expr);
                }else{
                    left_expr=sub_expr;
                }
                i=len-sub_len;//add the offset had been read.
                if(sub_len==0) break;//break the for-loop for skipping ++i;
            }else/* if(type==TYPE_OPER) */{//find an operation
                //check if left_expr is exsit.
                if(left_expr==nullptr){
                    cout<<OUTPUT_INFO<<"lack the expression in the left of operation!!\n";
                    return nullptr;
                }

                if(oper_lvl>type){//the end of a expression.
                    --i;
                    // cout<<OUTPUT_INFO<<*(Istr+i)<<"back to\n";
                    break;
                }

                int sub_len=len-(i+1);//"+1" for transfor sign ot count of the str.
                // cout<<OUTPUT_INFO<<"sub_len "<<sub_len<<"\n";
                // cout<<OUTPUT_INFO<<*(Istr+i)<<"before\n";
                expr_elem* right_expr=find_a_right_expr(Istr+i+1,sub_len,type);
                // cout<<OUTPUT_INFO<<"sub_len "<<sub_len<<"\n";
                // cout<<OUTPUT_INFO<<*(Istr+len-sub_len-1)<<"after\n";
                // if(left_expr!=nullptr){
                //     cout<<"left_expr:";
                //     expr_list_show(*left_expr);
                // }
                // if(right_expr!=nullptr){
                //     cout<<"right_expr:";
                //     expr_list_show(*right_expr);
                // }

                if(right_expr==nullptr){
                    cout<<OUTPUT_INFO<<"error!!\n";
                    expr_elem_free(left_expr);
                    return nullptr;
                }
                //get the expression.
                left_expr=expr_set(*(Istr+i),*left_expr,*right_expr);
                // if(left_expr!=nullptr){
                //     cout<<"left_expr:";
                //     expr_list_show(*left_expr);
                // }
                // cout<<OUTPUT_INFO<<(Istr+i)<<"|"<<i<<"|"<<len<<"|"<<sub_len<<"|offset\n";
                i=len-sub_len;//add the offset had been read.
                if(sub_len==0) break;//break the for-loop for skipping ++i;
                // cout<<OUTPUT_INFO<<*(Istr+i)<<"offset\n";
            }
        }
    }

    if(num_bit!=0){//find a number wait input
        expr_elem *tmp_expr=get_num_from_str(Istr+i,num_bit);
        if(tmp_expr==nullptr){
            cout<<OUTPUT_INFO<<"error\n";
            return 0;
        }
        num_bit=0;

        if(left_expr==nullptr){
            left_expr=tmp_expr;
        }else{// find a left_expr in front.
            //default a '*'operation between left_expr and parentheses.
            left_expr=expr_set('*',*left_expr,*tmp_expr);
        }
    }//num_bit!=0 && label_bit!=0 can`t both happen
    else if(label_bit!=0){// find a number it wiil be the part of left_expr os just left_expr.
        expr_elem *tmp_expr=get_label_from_str(Istr+i,label_bit);
        if(tmp_expr==nullptr){
            cout<<OUTPUT_INFO<<"error!!\n";
            return 0;
        }
        label_bit=0;

        if(left_expr==nullptr){
            left_expr=tmp_expr;
        }else{// find a left_expr in front.
            //default a '*'operation between left_expr and parentheses.
            left_expr=expr_set('*',*left_expr,*tmp_expr);
        }
    }
    len=len-i;//"--i;len=len-(i+1);" recode the len unread.

    // if(left_expr!=nullptr){
    //     cout<<"left_expr:";
    //     expr_list_show(*left_expr);
    // }
    return left_expr;
}

expr_elem* get_num_from_str(char *str,int num_bit){
    char *num_str=(char*)malloc(num_bit+1);
    memcpy(num_str,str-num_bit,num_bit);
    num_str[num_bit]='\0';

    int num=atoi(num_str); // output number with int-type.
    free(num_str);

    expr_elem *tmp_expr=new expr_elem_type();
    if(tmp_expr==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        return nullptr;
    }
    expr_init_elem(*tmp_expr,0,1,&num);
    return tmp_expr;
}

expr_elem* get_label_from_str(char *str,int label_bit){
    char *label=(char*)malloc(label_bit+1);
    memcpy(label,str-label_bit,label_bit);
    label[label_bit]='\0';

    expr_elem *tmp_expr=new expr_elem_type();
    if(tmp_expr==nullptr){
        cout<<OUTPUT_INFO<<"malloc failed!!\n";
        return nullptr;
    }
    expr_init_elem(*tmp_expr,0,2,label);

    return tmp_expr;
}

/*
LOGIC
get a oper
[func]find it left expression.
    it`s a number.(the reasion why it called find_a_numb)
        [return]get left expression.
    it`s a oper.
        [func]find it left expression.
        [func]find it right expression.
[func]find it right expression.
    it`s a number.
        [return]get right expression.
    it`s a oper.
        [func]find it left expression.
        [func]find it right expression.
*/
/// @brief find a number in the loop.
/// @param expr_head [input]the head of expr.
/// @param Estr_p [output]recode the str should printf.it`s malloc in function.so need free after printf.
/// @param expr_level [output]recode the oper-level in this expression.
/// @return the last expr_type has been printf.
expr_elem* find_a_numb(expr_elem &expr_head, char **Estr_p, int *expr_level){
    if(expr_head.type==1){//find a numb-data
        //INT_MAX 2147483647 strlen=10
        char Nstr[16]={0};
        itoa(expr_head.numb,Nstr,10);

        *Estr_p=(char *)malloc(strlen(Nstr)+1+1);
        if(*Estr_p==nullptr){
            cout<<OUTPUT_INFO<<"malloc failed!!\n";
            return &expr_head;
        }
        sprintf(*Estr_p,"%s",Nstr);
        *expr_level=TYPE_NUM;
        return &expr_head;
    }else if(expr_head.type==2){//find a numb in label-data
        *Estr_p=(char *)malloc(strlen(expr_head.label)+1+1);
        if(*Estr_p==nullptr){
            cout<<OUTPUT_INFO<<"malloc failed!!\n";
            return &expr_head;
        }
        sprintf(*Estr_p,"%s",expr_head.label);
        *expr_level=TYPE_LETTER;
        return &expr_head;
    }else if(expr_head.next==nullptr){//find a oper-data at the end.
        cout<<OUTPUT_INFO<<"form error!!\n";
        return &expr_head;
    }

    //the first data is an oper-data. and exsit next.
    expr_elem *ret_expr=&expr_head;//init the return of this function by input-expr.

    char *subL_Estr=nullptr;
    int Lexpr_lvl=0;
    ret_expr=find_a_numb(*expr_head.next,&subL_Estr,&Lexpr_lvl);

    if(subL_Estr==nullptr){//left-expr not found.
        cout<<OUTPUT_INFO<<"ERROR\n";
        return expr_find_end(*ret_expr);
    }

    if(ret_expr->next==nullptr){//lack a right expr of oper.
        cout<<OUTPUT_INFO<<"form error!!\n";
        return ret_expr;
    }

    char *subR_Estr=nullptr;
    int Rexpr_lvl=0;
    ret_expr=find_a_numb(*(ret_expr->next),&subR_Estr,&Rexpr_lvl);
    if(subR_Estr==nullptr){//right-expr not found.
        cout<<OUTPUT_INFO<<"ERROR\n";
        return expr_find_end(*ret_expr);
    }

    *expr_level=char_Analysis(expr_head.oper);
    // cout<<OUTPUT_INFO<<*expr_level<<"|"<<Lexpr_lvl<<"|"<<Rexpr_lvl<<"|"<<"\n";
    //input left-expr and oper to the Estr_p.
    if(Lexpr_lvl==TYPE_NUM 
    || Lexpr_lvl==TYPE_LETTER
    || Lexpr_lvl>=*expr_level){
        *Estr_p=(char *)malloc(strlen(subL_Estr)+1+strlen(subR_Estr)+2+1);
        sprintf(*Estr_p,"%s%c",subL_Estr,expr_head.oper);
    }else{
        *Estr_p=(char *)malloc(strlen(subL_Estr)+1+strlen(subR_Estr)+4+1);
        sprintf(*Estr_p,"(%s)%c",subL_Estr,expr_head.oper);
    }

    //input right-expr to the Estr_p.
    if(Rexpr_lvl==TYPE_NUM
    || Rexpr_lvl==TYPE_LETTER
    || Rexpr_lvl>=*expr_level){
        sprintf(*Estr_p,"%s%s",*Estr_p,subR_Estr);
    }else{
        sprintf(*Estr_p,"%s(%s)",*Estr_p,subR_Estr);
    }

    // cout<<OUTPUT_INFO<<*Estr_p<<"\n";
    free(subL_Estr);
    free(subR_Estr);

    return ret_expr;//tmp return
}