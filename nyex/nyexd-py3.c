#include "Python.h"
#include "numpy/arrayobject.h"


//https://cpp.hotexamples.com/zh/examples/-/-/PyArray_Zeros/cpp-pyarray_zeros-function-examples.html

void * global_buff = NULL;
npy_intp size[2] ;

void _rolling_mean(double* begin,double* end, double* output,unsigned long win_size ){
    double* it = begin;
    double  sum = 0;
    while( it != end){
        sum+= *it;
        uint64_t distance = (it-begin) / sizeof(double );
        auto size = distance %win_size;
        size +=1;
        if( distance >= win_size){
            size = win_size;
            sum -= *(it - win_size);
        }
        *output++ = sum / size;
        it++;
    }
}

volatile double D = 0;
double * W = 0;

void rolling_mean(PyArrayObject *a,PyArrayObject*b ,uint64_t col,unsigned long win_size ){
    uint64_t begin = 0;
    uint64_t end = a->dimensions[0];
    uint64_t it = begin;
    W = (double *)(b->data);
    double  sum = 0;
    uint64_t  size = 0;
    uint64_t  p = 0;
    while( it != end){
        double *v = (double*)PyArray_GETPTR2(a, it,col);
        uint64_t  offset = ((char*)v) - a->data;
        sum+= *v;
        size = it - p + 1;
        if( it -begin >=  win_size){
            size = win_size;
            v = (double*)PyArray_GETPTR2(a, p,col);
            sum -= *v;
            p++;
        }else{
                    W = (double*)PyArray_GETPTR2(b, it,col);
        W = (double*)(b->data + offset);
//            W = (double*)(global_buff + offset);
            *W = NAN;// sum /(double)size;  // expensive write cost
            it++;
            continue;
        }

        W = (double*)PyArray_GETPTR2(b, it,col);
        W = (double*)(b->data + offset);
//        W = (double*)(global_buff + offset);
        *W =  sum /(double)size;  // expensive write cost
        it++;

    }
}


static PyObject*
example (PyObject *dummy, PyObject *args)
{
    PyObject *arg1=NULL;
    PyObject *arg2=NULL;
    PyArrayObject *arr1=NULL;
    PyArrayObject *arr2=NULL;
    int nd;
    printf("-- start .. \n");
//    return PyLong_FromLong(111);
    if (!PyArg_ParseTuple(args, "O", &arg1))
        return NULL;


    arr1 = PyArray_FROM_OTF(arg1, NPY_DOUBLE, NPY_IN_ARRAY);
//    arr2 = PyArray_FROM_OTF(arg2, NPY_double, NPY_IN_ARRAY);
    if (arr1 == NULL)
        return NULL;

    size[0] = arr1->dimensions[0];
    size[1] = arr1->dimensions[1];

    PyArrayObject *ret;
    ret = PyArray_SimpleNew(2,size,NPY_DOUBLE);

    uint64_t  datasize = size[0] * size[1] * sizeof(double );
    global_buff = malloc( datasize);

    uint64_t win_size = 5;
    printf("D1: %d , D2: %d , WIN:%d \n",arr1->dimensions[0],arr1->dimensions[1],win_size);
    printf("D1: %d , D2: %d , WIN:%d \n",ret->dimensions[0],ret->dimensions[1],win_size);

//    ret = arr2;
    PyArrayObject *a = arr1;
    PyArrayObject *b = ret;
//    return ret;
    for (int i=0; i<arr1->dimensions[1]; ++i) {
        rolling_mean(a,b,i,win_size);

    }

    return ret;

}

static struct PyMethodDef methods[] = {
        {"example", example, METH_VARARGS, "descript of example"},
        {NULL, NULL, 0, NULL}
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
    "nyex", /* name of module */
    "",          /* module documentation, may be NULL */
    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    methods
};

PyMODINIT_FUNC PyInit_nyex(void)
{
    import_array();
    return PyModule_Create(&nyexDef);
}