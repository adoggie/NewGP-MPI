#include "Python.h"
#include "numpy/arrayobject.h"
//#include "rolling_window.h"
//#include <boost/range/irange.hpp>
#include <numeric>
#include <complex>
#include <algorithm>
#include <vector>
#include <list>
#include <utility>
#include <boost/circular_buffer.hpp>
#include <execution>


//https://cpp.hotexamples.com/zh/examples/-/-/PyArray_Zeros/cpp-pyarray_zeros-function-examples.html

void apply_tsrank(PyArrayObject *in,PyArrayObject *out,uint64_t col,int64_t minp) ;
std::vector<double> roll_skew(std::vector<double> values,
                              std::vector<int64_t> start,
                              std::vector<int64_t> end,
                              int64_t minp);
void roll_skew2(
        PyArrayObject *in,PyArrayObject *out,uint64_t col,
        int64_t minp);

void _rolling_skew(PyArrayObject *in, PyArrayObject *out, uint64_t col, unsigned long win_size) {
    roll_skew2(in,out,col,win_size);
}


enum class RollType {
    MEAN,
    VAR,
    STD,
    SKEW,
    KURT,
    TSRANK
};

//template<typename T>
class RunningStats {
public:
    RunningStats(uint32_t winsize) : winsize_(winsize), windows_(winsize) {
    }

    void clear() {
        n_ = 0;
        windows_.empty();
    }

    void push(double x) {
        if (n_ < winsize_) {
//            # Calculating first variance
            windows_.push_back(x);
            double delta;
            delta = x - mean_;
            mean_ += delta / (n_ + 1);
            run_var_ += delta * (x - mean_);
            n_ += 1;
        } else {
//            # Adjusting variance
            double x_removed;
            x_removed = windows_[0];
            windows_.push_back(x);
            double old_m = mean_;
            mean_ += (x - x_removed) / winsize_;
            run_var_ += (x + x_removed - old_m - mean_) * (x - x_removed);
        }
//        printf("mean: %f , x:%f \n",mean_,x);
    }

    double get_mean(){
        return mean_?mean_:0;
    }

    double get_var(){
        if( n_ > 1){
            return run_var_ / (winsize_ -1);
        }
        return 0;
    }

    double get_std(){
        return std::sqrt( this->get_var());
    }
private:
    uint32_t n_ = 0;
    double mean_ = 0;
    double run_var_ = 0;
    uint32_t winsize_;
    boost::circular_buffer <double> windows_;
};

void rolling_welford(PyArrayObject *in, PyArrayObject *out, uint64_t col, unsigned long win_size, RollType rt) {
    uint64_t begin = 0;
    uint64_t end = in->dimensions[0];
    uint64_t it = begin;
    RunningStats stat(win_size);

    for(;it< end;it++){
        float v = *(float *) PyArray_GETPTR2(in, it, col);
//        printf("v: %f , it:%d , col:%d \n",v,it,col);
        stat.push(v);
        float* p = (float*)PyArray_GETPTR2(out, it,col);
        if(RollType::MEAN == rt){
            *p =(float) stat.get_mean();
        }else if( RollType::VAR == rt){
            *p = (float)stat.get_var();
        } else if(RollType::STD == rt){
            *p = (float)stat.get_std();
        }
    }
}
/*  roll(100)
 start: [ 0  0  0  0  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43
44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67
68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91
92 93 94 95]
 end : [  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18
19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36
37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54
55  56  57  58  59  60  61  62  63  64  65  66  67  68  69  70  71  72
73  74  75  76  77  78  79  80  81  82  83  84  85  86  87  88  89  90
91  92  93  94  95  96  97  98  99 100]
 */

void _rolling_skew2(PyArrayObject *in, PyArrayObject *out, uint64_t col, unsigned long win_size) {
    std::size_t rows = in->dimensions[0];
    std::vector<double> result,input(rows);
    std::vector<int64_t> end(rows);
    std::vector<int64_t> start(rows);
//    printf("_rolling_skew() rows:%u \n",rows);
    for(std::size_t n=0;n<rows; n++){
        end[n] = n+1;
        if(n<win_size){
            start[n] = 0;
        }else{
            start[n] = n - win_size +1;
        }
        float v = *(float *) PyArray_GETPTR2(in, n, col);
        input[n] = v ;
//        printf("%u ",end[n]);
    }

    result = roll_skew(input,start,end,win_size);
    for(std::size_t n=0;n< rows; n++) {
        float* v = (float *) PyArray_GETPTR2(out, n, col);
        *v = (float)result[n];
    }
}

void rolling_kurt(PyArrayObject *in, PyArrayObject *out, uint64_t col, unsigned long win_size) {
}

void rolling_tsrank(PyArrayObject *in, PyArrayObject *out, uint64_t col, unsigned long win_size) {
}


PyObject *
rolling_xxx(PyObject *dummy, PyObject *args,RollType rt) {
    PyObject *arg1 = NULL;
    uint64_t win_size = 0;
    uint64_t par = 0;
    npy_intp size[2];
    PyArrayObject *arr1 = NULL;

    if (!PyArg_ParseTuple(args, "OII", &arg1, &win_size, &par)) {
        printf("ERROR: PyArg_ParseTuple Fail .\n");
        return PyLong_FromLong(0);
    }

    printf("STEP.1 win_size:%u, par:%u \n", (uint32_t)win_size, (uint32_t)par);
//    return PyLong_FromLong(0);
    printf("STEP.1\n");
//    arr1 = PyArray_FROM_OTF(arg1, NPY_FLOAT, NPY_IN_ARRAY);
    arr1 = (PyArrayObject *) PyArray_FROM_O(arg1);
    if (arr1 == NULL) {
        printf("-- ERROR: arr1 is NULL .\n");
        return PyLong_FromLong(0);
    }

    size[0] = arr1->dimensions[0];
    size[1] = arr1->dimensions[1];

    PyArrayObject *ret;
    ret = (PyArrayObject *) PyArray_SimpleNew(2, size, NPY_FLOAT);

    printf("D1: %d , D2: %d , WIN:%d \n", (int) ret->dimensions[0], (int) ret->dimensions[1], (int) win_size);
    PyArrayObject *a = arr1;
    PyArrayObject *b = ret;
    if (par) {
        std::vector <uint64_t> range(arr1->dimensions[1]);
        std::iota(range.begin(), range.end(), 0);
        switch (rt) {
            case RollType::SKEW :
                std::for_each(std::execution::par_unseq, range.begin(), range.end(), [&](auto &n) {
                    _rolling_skew(a, b, n, win_size);
                });
                break;
            case RollType::TSRANK:
                std::for_each(std::execution::par_unseq, range.begin(), range.end(), [&](auto &n) {
                    apply_tsrank(a, b, n, win_size);
                });
            default:
                std::for_each(std::execution::par_unseq, range.begin(), range.end(), [&](auto &n) {
                    rolling_welford(a, b, n, win_size, rt);
                });
        }
        return (PyObject *) ret;
    }

    for (uint32_t i = 0; i < arr1->dimensions[1]; ++i) {
//            printf("call rt:%d \n",rt);
        switch (rt) {
            case RollType::SKEW:
                _rolling_skew(a,b,i,win_size);
                break;
            case RollType::TSRANK:
                apply_tsrank(a,b,i,win_size);
                break;
            default:
                rolling_welford(a, b, i, win_size, rt);
        }
    }

    return (PyObject *) ret;
}

extern "C" PyObject *
rolling_mean(PyObject *dummy, PyObject *args) {
    return (PyObject *) rolling_xxx(dummy,args,RollType::MEAN);
}

extern "C" PyObject *
rolling_std(PyObject *dummy, PyObject *args) {
    return (PyObject *) rolling_xxx(dummy,args,RollType::STD);
}

extern "C" PyObject *
rolling_var(PyObject *dummy, PyObject *args) {
    return (PyObject *) rolling_xxx(dummy,args,RollType::VAR);
}

extern "C" PyObject *
rolling_skew(PyObject *dummy, PyObject *args) {
    printf(" in rolling_skew : %d \n",RollType::SKEW );
    return (PyObject *) rolling_xxx(dummy,args,RollType::SKEW);
}

extern "C" PyObject *
rolling_kurt(PyObject *dummy, PyObject *args) {
    return (PyObject *) rolling_xxx(dummy,args,RollType::KURT);
}

extern "C" PyObject *
rolling_tsrank(PyObject *dummy, PyObject *args) {
    return (PyObject *) rolling_xxx(dummy,args,RollType::TSRANK);
}

static struct PyMethodDef methods[] = {
        {"rolling_mean",  rolling_mean,  METH_VARARGS, "descript of example"},
        {"rolling_std", rolling_std, METH_VARARGS, "descript of example"},
        {"rolling_var", rolling_var, METH_VARARGS, "descript of example"},
        {"rolling_skew", rolling_skew, METH_VARARGS, "descript of example"},
        {"rolling_kurt", rolling_kurt, METH_VARARGS, "descript of example"},
        {"rolling_tsrank", rolling_tsrank, METH_VARARGS, "descript of example"},
        {NULL,            NULL,          0,            NULL}
};


//PyMODINIT_FUNC
//initnyex (void)
//{
//    (void)Py_InitModule("nyex", methods);
//    import_array();
//}



//  python 3
    static struct PyModuleDef nyexDef =
            {
                    PyModuleDef_HEAD_INIT,
                    "nyex2", /* name of module */
                    "",          /* module documentation, may be NULL */
                    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
                    methods
            };

    PyMODINIT_FUNC PyInit_nyex2(void) {
        import_array();
        return PyModule_Create(&nyexDef);
    }

