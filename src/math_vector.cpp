#include "math_vector.h"
#include <stdlib.h>
//#include <stdio.h>
#include <iostream>
using std::cout;

#define LOG_SHOW

enum data_type{INT,FLOAT};

#define PTR_COPY(ptr0,ptr1,len) for(int i=0;i<len;++i){*(ptr0+i)=*(ptr1+i);}

//private menent function
math_vector::math_vector(int *data,int len,bool T,void *prt):len(len),type(INT),T(T)
{
    if(data==nullptr){
        vector=nullptr;
        return;
    }

    vector=malloc(sizeof(int)*len);
    if(vector==nullptr){
        cout<<"malloc failed!!\n";
        return;
    }

    int *label=(int *)vector;
    PTR_COPY(label,data,len)

    free(prt);
}

//private menent function
math_vector::math_vector(float *data,int len,bool T,void *prt):len(len),type(FLOAT),T(T)
{
    if(data==nullptr){
        vector=nullptr;
        return;
    }

    vector=malloc(sizeof(float)*len);
    if(vector==nullptr){
        cout<<"malloc failed!!\n";
        return;
    }

    float *label=(float *)vector;
    PTR_COPY(label,data,len)

    free(prt);
}

math_vector::math_vector():len(0)
{
    //printf("%s:%d\n",__FUNCTION__,__LINE__);
    type=INT;
    T=0;
    vector=nullptr;
}

math_vector::math_vector(int len):len(len),type(INT),T(0)
{
    vector=malloc(sizeof(int)*len);
    if(vector==nullptr){
        cout<<"malloc failed!!\n";
        return;
    }

    for(int i=0;i<len;++i){
        *((int*)vector+i)=0;
    }
}

math_vector::math_vector(int len, bool T):len(len),type(INT),T(T)
{
    vector=malloc(sizeof(int)*len);
    if(vector==nullptr){
        cout<<"malloc failed!!\n";
        return;
    }

    for(int i=0;i<len;++i){
        *((int*)vector+i)=0;
    }
}

math_vector::math_vector(int *data, int len) : len(len), type(INT), T(0)
{
#ifdef LOG_SHOW
    cout<<__FUNCTION__<<":"<<__LINE__<<"\n";
#endif

    if(data==nullptr){
        vector=nullptr;
        return;
    }

    vector=malloc(sizeof(int)*len);
    if(vector==nullptr){
        cout<<"malloc failed!!\n";
        return;
    }

    int *label=(int *)vector;
    PTR_COPY(label,data,len)
    
#ifdef LOG_SHOW
    cout<<"\n";
#endif
}

math_vector::math_vector(int *data, int len, bool T):len(len),type(INT),T(T)
{
#ifdef LOG_SHOW
    cout<<__FUNCTION__<<":"<<__LINE__<<"\n";
#endif
    if(data==nullptr){
        vector=nullptr;
        return;
    }

    vector=malloc(sizeof(int)*len);
    if(vector==nullptr){
        cout<<"malloc failed!!\n";
        return;
    }

    int *label=(int *)vector;
    PTR_COPY(label,data,len)

#ifdef LOG_SHOW
    cout<<"\n";
#endif
}

math_vector::math_vector(float *data, int len):len(len),type(FLOAT),T(0)
{
    if(data==nullptr){
        vector=nullptr;
        return;
    }

    vector=malloc(sizeof(float)*len);
    if(vector==nullptr){
        cout<<"malloc failed!!\n";
        return;
    }

    float *label=(float *)vector;
    PTR_COPY(label,data,len)
}

math_vector::math_vector(float *data, int len, bool T):len(len),type(FLOAT),T(T)
{
    if(data==nullptr){
        vector=nullptr;
        return;
    }

    vector=malloc(sizeof(float)*len);
    if(vector==nullptr){
        cout<<"malloc failed!!\n";
        return;
    }

    float *label=(float *)vector;
    PTR_COPY(label,data,len)
}

math_vector::math_vector(const math_vector &data):len(data.len),type(data.type),T(data.T)
{
    vector=malloc(sizeof(int)*len);
    if(vector==nullptr){
        cout<<"malloc failed!!\n";
        return;
    }

    if(type==INT){
        int *label_0=(int*)vector;
        int *label_1=(int*)data.vector;
        PTR_COPY(label_0,label_1,len)
    }else if(type==FLOAT){
        float *label_0=(float*)vector;
        float *label_1=(float*)data.vector;
        PTR_COPY(label_0,label_1,len)
    }
}

math_vector::~math_vector()
{
    if(vector!=nullptr)
        free(vector);
}

#define FUNC_SHOW_COUT_IN_FOR(ptr,len,end) cout<<"*";for(int i=0;i<len;++i){cout<<*(ptr+i);if(i!=len-1) cout<<end;}cout<<"*\n";
void math_vector::show(void)const
{
    if(len==0 || vector==nullptr){
        cout<<"it`s empty vector.\n";
        return;
    }

    //cout<<"type:"<<type<<"\nT:"<<T<<"\nlen:"<<len<<"\n";
    if(type==INT){
        int *label=(int *)vector;

        if(T){
            FUNC_SHOW_COUT_IN_FOR(label,len,"\n")
        }else{
            FUNC_SHOW_COUT_IN_FOR(label,len,"\t")
        }
    }else if(type==FLOAT){
        float *label=(float *)vector;
        if(T){
            FUNC_SHOW_COUT_IN_FOR(label,len,"\n")
        }else{
            FUNC_SHOW_COUT_IN_FOR(label,len,"\t")
        }
    }
}



const math_vector &math_vector::operator=(const math_vector &data)
{
    if(len!=data.len || type!=data.type){
        void *cont=nullptr;
        if(data.type==INT)
            cont=malloc(sizeof(int)*data.len);
        else if(data.type==FLOAT)
            cont=malloc(sizeof(float)*data.len);

        if(cont==nullptr){
            cout<<"malloc failed!!\n";
            return *this;
        }
        if(vector!=nullptr)
            free(vector);
        vector=cont;
    }

    type=data.type;
    T=data.T;
    len=data.len;

    if(type==INT){
        int *label_0=(int *)vector;
        int *label_1=(int *)data.vector;
        PTR_COPY(label_0,label_1,data.len)
    }else if(type==FLOAT){
        float *label_0=(float *)vector;
        float *label_1=(float *)data.vector;
        PTR_COPY(label_0,label_1,data.len)
    }

    return *this;
}

#define OPERATE_VECTOR(prt0,prt1,cmd) for(int i=0;i<tag_len;++i){if(i<src_0->len) num0=*(prt0+i);else num0=0;if(i<src_1->len) num1=*(prt1+i);else num1=0;cmd;}
math_vector math_vector::operator+(const math_vector &data)const
{
    if(T!=data.T){
        cout<<"it`s different T-status!!\n";
        return math_vector();
    }

    if(len!=data.len) cout<<"note:it`s different len.\n";

    if(type==INT && data.type==INT){//the case:both int-data
        int tag_len= len>=data.len? len:data.len;

        int *tag_cont=(int*)malloc(sizeof(int)*tag_len);
        if(tag_cont==nullptr){
            cout<<"malloc failed!!\n";
            return math_vector();
        }

        bool tag_T=T;
        int tag_type=INT;

        const math_vector *src_0=this;
        const math_vector *src_1=&data;

        int num0=0,num1=0;
        OPERATE_VECTOR((int*)src_0->vector,(int*)src_1->vector,*(tag_cont+i)=num0+num1)

        return math_vector(tag_cont,tag_len,tag_T,tag_cont);        
    }else{//the case:at least ont float-data
        int tag_len= len>=data.len? len:data.len;

        float *tag_cont=(float*)malloc(sizeof(float)*tag_len);
        if(tag_cont==nullptr){
            cout<<"malloc failed!!\n";
            return math_vector();
        }

        bool tag_T=T;
        int tag_type=FLOAT;

        const math_vector *src_0=nullptr;
        const math_vector *src_1=nullptr;

        bool exsit_int=false;
        if(type==INT){exsit_int=true;src_0=this;src_1=&data;}
        else if(data.type==INT){exsit_int=true;src_0=&data;src_1=this;}
        else {src_0=this;src_1=&data;}

        if(exsit_int){
            int num0=0;
            float num1=0;
            OPERATE_VECTOR((int*)src_0->vector,(float*)src_1->vector,*(tag_cont+i)=num0+num1)
        }else{
            float num0=0;
            float num1=0;
            OPERATE_VECTOR((int*)src_0->vector,(float*)src_1->vector,*(tag_cont+i)=num0+num1)
        }

        return math_vector(tag_cont,tag_len,tag_T,tag_cont);
    }

    return math_vector();
}

math_vector math_vector::operator-(const math_vector &data) const
{
    if(T!=data.T){
        cout<<"it`s different T-status!!\n";
        return math_vector();
    }

    if(len!=data.len) cout<<"note:it`s different len.\n";

    if(type==INT && data.type==INT){//the case:both int-data
        int tag_len= len>=data.len? len:data.len;

        int *tag_cont=(int*)malloc(sizeof(int)*tag_len);
        if(tag_cont==nullptr){
            cout<<"malloc failed!!\n";
            return math_vector();
        }

        bool tag_T=T;
        int tag_type=INT;

        const math_vector *src_0=this;
        const math_vector *src_1=&data;

        int num0=0,num1=0;
        OPERATE_VECTOR((int*)src_0->vector,(int*)src_1->vector,*(tag_cont+i)=num0-num1)

        return math_vector(tag_cont,tag_len,tag_T,tag_cont);        
    }else{//the case:at least ont float-data
        int tag_len= len>=data.len? len:data.len;

        float *tag_cont=(float*)malloc(sizeof(float)*tag_len);
        if(tag_cont==nullptr){
            cout<<"malloc failed!!\n";
            return math_vector();
        }

        bool tag_T=T;
        int tag_type=FLOAT;

        const math_vector *src_0=nullptr;
        const math_vector *src_1=nullptr;

        bool exsit_int=false;
        if(type==INT){exsit_int=true;src_0=this;src_1=&data;}
        else if(data.type==INT){exsit_int=true;src_0=&data;src_1=this;}
        else {src_0=this;src_1=&data;}

        if(exsit_int){
            int num0=0;
            float num1=0;
            OPERATE_VECTOR((int*)src_0->vector,(float*)src_1->vector,*(tag_cont+i)=num0-num1)
        }else{
            float num0=0;
            float num1=0;
            OPERATE_VECTOR((int*)src_0->vector,(float*)src_1->vector,*(tag_cont+i)=num0-num1)
        }

        return math_vector(tag_cont,tag_len,tag_T,tag_cont);
    }

    return math_vector();
}

math_vector math_vector::operator*(const math_vector &) const
{
    return math_vector();
}

math_vector math_vector::operator-(void) const
{
    if(vector==nullptr) return math_vector();

    if(type==INT){
        int *tag_cont=(int*)malloc(sizeof(int)*len);
        if(tag_cont==nullptr){
            cout<<"malloc failed!!\n";
            return math_vector();
        }

        for(int i=0;i<len;++i){
            *(tag_cont+i)=-*((int*)vector+i);
        }
        return math_vector(tag_cont,len,T,tag_cont);
    }else if(type==FLOAT){
        float *tag_cont=(float*)malloc(sizeof(float)*len);
        if(tag_cont==nullptr){
            cout<<"malloc failed!!\n";
            return math_vector();
        }

        for(int i=0;i<len;++i){
            *(tag_cont+i)=-*((float*)vector+i);
        }
        return math_vector(tag_cont,len,T,tag_cont);
    }

    return math_vector();
}

#define DIFF_IN_FOR(prt0,prt1,cmd) for(int i=0;i<len;++i){if(*(prt0+i)!=*(prt1+i)){cmd}}
bool math_vector::operator==(const math_vector &data) const
{
    bool is_true=true;
    if(len!=data.len) {cout<<"len is diff\n";is_true=false;}
    if(T!=data.T) {cout<<"T is diff\n";is_true=false;}

    if(!is_true) return false;

    if(type!=data.type) cout<<"type is diff,but try find diff in numb\n";

    if(type==INT && data.type==INT){
        const math_vector *src_0=this;
        const math_vector *src_1=&data;

        DIFF_IN_FOR((int*)src_0->vector,(int*)src_1->vector,return false;)
        return true;
    }else if(type==FLOAT && data.type==FLOAT){
        const math_vector *src_0=this;
        const math_vector *src_1=&data;

        DIFF_IN_FOR((float*)src_0->vector,(float*)src_1->vector,return false;)
        return true;
    }else{//INT OR FLOAT
        const math_vector *src_0=nullptr;
        const math_vector *src_1=nullptr;
        if(type==INT) {src_0=this;src_1=&data;}
        else {src_0=&data;src_1=this;}

        int *iprt=(int*)src_0->vector;
        float *fprt=(float*)src_1->vector;

        DIFF_IN_FOR(iprt,fprt,return false;)
        return true;
    }

    return is_true;
}

bool math_vector::operator!=(const math_vector &data) const
{
    bool is_true=true;
    if(len!=data.len) {cout<<"len is diff\n";is_true=false;}
    if(T!=data.T) {cout<<"T is diff\n";is_true=false;}

    if(is_true) return false;

    if(type!=data.type) cout<<"type is diff,but try find diff in numb\n";

    if(type==INT && data.type==INT){
        const math_vector *src_0=this;
        const math_vector *src_1=&data;

        DIFF_IN_FOR((int*)src_0->vector,(int*)src_1->vector,return true;)
        return false;
    }else if(type==FLOAT && data.type==FLOAT){
        const math_vector *src_0=this;
        const math_vector *src_1=&data;

        DIFF_IN_FOR((float*)src_0->vector,(float*)src_1->vector,return true;)
        return false;
    }else{//INT OR FLOAT
        const math_vector *src_0=nullptr;
        const math_vector *src_1=nullptr;
        if(type==INT) {src_0=this;src_1=&data;}
        else {src_0=&data;src_1=this;}

        int *iprt=(int*)src_0->vector;
        float *fprt=(float*)src_1->vector;

        DIFF_IN_FOR(iprt,fprt,return true;)
        return false;
    }

    return !is_true;
}
