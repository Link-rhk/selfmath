#ifndef _MATH_VECTOR_H_
#define _MATH_VECTOR_H_
#include <stdlib.h>
class math_vector
{
private:
    unsigned char type;
    int len;
    bool T;//0- 1|
    void* vector;

    explicit math_vector(int *data,int len,bool T,void *prt);
    explicit math_vector(float *data,int len,bool T,void *prt);
public:
    math_vector();
    
    explicit math_vector(int len);
    explicit math_vector(int len,bool T);

    explicit math_vector(int *data,int len);
    explicit math_vector(int *data,int len,bool T);
    explicit math_vector(float *data,int len);
    explicit math_vector(float *data,int len,bool T);

    math_vector(const math_vector &);

    void show(void)const;

    const math_vector& operator=(const math_vector&);
    math_vector operator+(const math_vector&)const;
    math_vector operator-(const math_vector&)const;
    math_vector operator*(const math_vector&)const;//need define
    math_vector operator-(void)const;

    bool operator==(const math_vector&)const;
    bool operator!=(const math_vector&)const;

    math_vector& operator++() = delete;
    math_vector& operator--() = delete;
    math_vector& operator!() = delete;
    math_vector& operator~() = delete;
    ~math_vector();
};

static const math_vector zero_vector;
#endif //_MATH_VECTOR_H_
