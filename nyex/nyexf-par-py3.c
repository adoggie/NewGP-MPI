#include "Python.h"
#include "numpy/arrayobject.h"


//https://cpp.hotexamples.com/zh/examples/-/-/PyArray_Zeros/cpp-pyarray_zeros-function-examples.html

void * global_buff = NULL;
npy_intp size[2] ;

void _rolling_mean(float* begin,float* end, float* output,unsigned long win_size ){
    float* it = begin;
    float  sum = 0;
    while( it != end){
        sum+= *it;
        uint64_t distance = (it-begin) / sizeof(float );
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

volatile float D = 0;
float * W = 0;

void rolling_mean(PyArrayObject *a,PyArrayObject*b ,uint64_t col,unsigned long win_size ){
    uint64_t begin = 0;
    uint64_t end = a->dimensions[0];
    uint64_t it = begin;
    W = (float *)(b->data);
    float  sum = 0;
    uint64_t  size = 0;
    uint64_t  p = 0;
    while( it != end){
        float *v = (float*)PyArray_GETPTR2(a, it,col);
        uint64_t  offset = ((char*)v) - a->data;
        sum+= *v;
        size = it - p + 1;
        if( it -begin >=  win_size){
            size = win_size;
            v = (float*)PyArray_GETPTR2(a, p,col);
            sum -= *v;
            p++;
        }else{
            W = (float*)PyArray_GETPTR2(b, it,col);
            W = (float*)(b->data + offset);
//            W = (float*)(global_buff + offset);
            *W = NAN;// sum /(float)size;  // expensive write cost
            it++;
            continue;
        }

        W = (float*)PyArray_GETPTR2(b, it,col);
//        W = (float*)(b->data + offset);
//        W = (float*)(global_buff + offset);
        *W =  sum /(float)size;  // expensive write cost
        it++;
//        printf("%d,%f\n",it,*v);
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
    if (!PyArg_ParseTuple(args, "O", &arg1)) {
        printf("ERROR: PyArg_ParseTuple Fail .\n");
        return PyLong_FromLong(0);
    }

    printf("STEP.1\n");
//    arr1 = PyArray_FROM_OTF(arg1, NPY_FLOAT, NPY_IN_ARRAY);
    arr1 = PyArray_FROM_O(arg1);
//    arr2 = PyArray_FROM_OTF(arg2, NPY_float, NPY_IN_ARRAY);
    if (arr1 == NULL) {
        printf("-- ERROR: arr1 is NULL .\n");
        return PyLong_FromLong(0);
    }

    size[0] = arr1->dimensions[0];
    size[1] = arr1->dimensions[1];
    printf("STEP.2\n");
    PyArrayObject *ret;
    ret = PyArray_SimpleNew(2,size,NPY_FLOAT);

    uint64_t  datasize = size[0] * size[1] * sizeof(float );
//    global_buff = malloc( datasize);

    uint64_t win_size = 100;
    printf("D1: %d , D2: %d , WIN:%d \n",arr1->dimensions[0],arr1->dimensions[1],win_size);
    printf("D1: %d , D2: %d , WIN:%d \n",ret->dimensions[0],ret->dimensions[1],win_size);
//    return PyLong_FromLong(11);

//    ret = arr2;
    PyArrayObject *a = arr1;
    PyArrayObject *b = ret;
//    return ret;
    printf("STEP.3\n");
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