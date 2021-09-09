#include "Python.h"
#include "numpy/arrayobject.h"
#include "rolling_window.h"
#include <boost/range/irange.hpp>
#include <numeric>
#include <execution>


//https://cpp.hotexamples.com/zh/examples/-/-/PyArray_Zeros/cpp-pyarray_zeros-function-examples.html

void * global_buff = NULL;
npy_intp size[2] ;

//extern "C" {
//PyObject *
//example(PyObject *dummy, PyObject *args);
//}

 extern "C" PyObject*
example (PyObject *dummy, PyObject *args)
{
    PyObject *arg1=NULL;
    PyObject *arg2=NULL; // win_size
    PyObject *arg3=NULL; // is parall
    uint64_t win_size = 0;
    uint64_t par=0 ;

    PyArrayObject *arr1=NULL;
    PyArrayObject *arr2=NULL;
    int nd;
    printf("-- start .. \n");
//    return PyLong_FromLong(111);
    // https://docs.python.org/zh-cn/3/c-api/arg.html#c.PyArg_Parse
//    if (!PyArg_ParseTuple(args, "OII", &arg1,&win_size,&par)) {
    if (!PyArg_ParseTuple(args, "OII", &arg1,&win_size,&par)) {
        printf("ERROR: PyArg_ParseTuple Fail .\n");
        return PyLong_FromLong(0);
    }

    printf("STEP.1 win_size:%lu, par:%lu \n", win_size,par);
//    return PyLong_FromLong(0);
    printf("STEP.1\n");
//    arr1 = PyArray_FROM_OTF(arg1, NPY_FLOAT, NPY_IN_ARRAY);
    arr1 = (PyArrayObject*)PyArray_FROM_O(arg1);
//    arr2 = PyArray_FROM_OTF(arg2, NPY_float, NPY_IN_ARRAY);
    if (arr1 == NULL) {
        printf("-- ERROR: arr1 is NULL .\n");
        return PyLong_FromLong(0);
    }

    size[0] = arr1->dimensions[0];
    size[1] = arr1->dimensions[1];
//    printf("STEP.2\n");
    PyArrayObject *ret;
    ret = (PyArrayObject*)PyArray_SimpleNew(2,size,NPY_FLOAT);

//    uint64_t  datasize = size[0] * size[1] * sizeof(float );
//    global_buff = malloc( datasize);

//    uint64_t win_size = 100;
//    printf("D1: %d , D2: %d , WIN:%d \n",(int)arr1->dimensions[0],(int)arr1->dimensions[1],(int)win_size);
    printf("D1: %d , D2: %d , WIN:%d \n",(int)ret->dimensions[0],(int)ret->dimensions[1],(int)win_size);
    PyArrayObject *a = arr1;
    PyArrayObject *b = ret;
//    printf("STEP.3\n");

    if( par) {
//        auto range = boost::irange(1, 10);
        std::vector<uint64_t> range(arr1->dimensions[1]);
        std::iota(range.begin(), range.end(), 0);
        std::for_each(std::execution::par_unseq, range.begin(), range.end(), [&](auto &n) {
            el::rolling_mean(a, b, n, win_size);
        });
    }else {
        for (int i = 0; i < arr1->dimensions[1]; ++i) {
            el::rolling_mean(a, b, i, win_size);
        }
    }

    return (PyObject*)ret;

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

PyMODINIT_FUNC PyInit_nyex(void) {
    import_array();
    return PyModule_Create(&nyexDef);
}

