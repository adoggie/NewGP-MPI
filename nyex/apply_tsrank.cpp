#include "Python.h"
#include "numpy/arrayobject.h"
#include <numeric>
#include <complex>
#include <algorithm>
#include <vector>
#include <list>
#include <utility>
#include <boost/circular_buffer.hpp>




/*
 factor1 = factor1.rolling(period1).apply(lambda x: np.sum(x <= x[-1]) / (period1), raw=True, engine='numba')
*/

#define ARRAY_GET(arr,r,c)   *(float *) PyArray_GETPTR2(arr, r, c)
#define ARRAY_SET(arr,r,c,v)   *(float *) PyArray_GETPTR2(arr, r, c) = float(v);

void apply_tsrank(PyArrayObject *in,PyArrayObject *out,uint64_t col,int64_t minp) {

    int64_t i;
    int64_t nobs = 0, N = in->dimensions[0];
    int64_t s, e;
    float result=-1;
    float prev = 0;

    for ( i = 0; i < N; i++) {
        s = 0;
        if( i >= minp ){
            s = i - minp+1;
        }
        e = i + 1;
        nobs = e - s;
        if(nobs < minp){
            ARRAY_SET(out,i,col,NAN);
            continue ;
        }
        //-- optimized --
        if(result !=-1){
            float v = ARRAY_GET(in,e-1,col);
//            printf("i:%d,col:%d, s:%d,e:%d, prev:%f, v(e-1):%f  ,result:%f \n",i,col,s,e,prev,v , result);
            if( prev <= v){
                ARRAY_SET(out,i,col,result );
                prev = v;
                continue; ;
            }
        }
        //-- end optimized --

        float  sum = 0.;
        for(int64_t n = s ; n<e ;n++){
            float v = ARRAY_GET(in,n,col);
            if( v <= ARRAY_GET(in,e-1,col) ){
                sum +=1.;
            }
//            printf("n: %d ,col:%d, v:%f , v[-1]:%f, sum:%f ,minp:%d\n",n,col,v,last,sum,minp);
        }
        prev = ARRAY_GET(in,e-1,col);
        result = sum / minp;
        ARRAY_SET(out,i,col,result );
    }
}
//
//int main(int ,char**){
//
//    return 0;
//}