#include "Python.h"
#include "numpy/arrayobject.h"


//https://cpp.hotexamples.com/zh/examples/-/-/PyArray_Zeros/cpp-pyarray_zeros-function-examples.html

void * global_buff = NULL;
npy_intp size[2] ;

void _rolling_mean(double* begin,double* end, double* output,unsigned long win_size ){
    double* it = begin;
    float  sum = 0;
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
volatile double * W = 0;

void rolling_mean(PyArrayObject *a,PyArrayObject*b ,uint64_t col,unsigned long win_size ){
//    double* it = begin;
    uint64_t begin = 0;
    uint64_t end = a->dimensions[0];
    uint64_t it = begin;

    W = (double *)(b->data);
//    W = (double*) (b->data + col * 8);
//    W = (double*)global_buff;
    double  sum = 0;
//    uint64_t coloff = col*b->strides[1];

    while( it != end){
        double *v = (double*)PyArray_GETPTR2(a, it,col);
        uint64_t  offset = ((char*)v) - a->data;

//        printf("%f ",*v);
        sum+= *v;
        uint64_t distance = (it-begin) ;
        auto size = distance %win_size;
        size +=1;
        if( distance >= win_size){
            size = win_size;
            sum -= *(v - win_size);
        }

//        PyArrayObject_fields * aof = b;
//        *W++ = *(double*)PyArray_GETPTR2(a, it,col) /size;

//         W =(double*)( b->data + offset); // slow
        *W = sum /(double)size;  // fast
        W++;
//        *W++ = 1.22312;  // fast

//        double *w = (double*)PyArray_GETPTR2(b, it,col);
//        *(double*)global_buff = *w;
//        D = *w;
//        register x = sum/(double)size;
//        *w = x;
//        volatile double *w = (double*)PyArray_GETPTR2(b, it,col); // slow
//        double  X = *(double *)(b->data + offset);
//        double * w = (double *)(a->data + offset);
//        volatile double *w = (double*)(b->data + it + col);  // cheap
//        *w = ( sum / (double)size);
//        *W = sum / (double)size; //  fast
//        W = (double *)(global_buff + offset );
//        *W = sum /(double)size;  // fast
//        *W = 1;
//        *W = sum /(double)size;
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

//    npy_intp dims[1] = {10};
//    PyObject* phase = PyArray_SimpleNew(1, dims, NPY_DOUBLE);
//    return phase;


    arr1 = PyArray_FROM_OTF(arg1, NPY_DOUBLE, NPY_IN_ARRAY);
//    arr2 = PyArray_FROM_OTF(arg2, NPY_DOUBLE, NPY_IN_ARRAY);
    if (arr1 == NULL)
        return NULL;

//    int npts = PyArray_DIM(arg1, PyArray_NDIM(arg1) - 1); //  0 col's size
//    PyArrayObject *ret = (PyArrayObject *)PyArray_Zeros(PyArray_NDIM(arg1), PyArray_DIMS(arg1),
//                                                        PyArray_DescrFromType(NPY_DOUBLE), 0);

    size[0] = arr1->dimensions[0];
    size[1] = arr1->dimensions[1];

    size[1] = arr1->dimensions[0];
    size[0] = arr1->dimensions[1];

    PyArrayObject *ret;
    ret = PyArray_SimpleNew(2,size,NPY_DOUBLE);
//    PyArray_UpdateFlags(ret,NPY_ARRAY_ALIGNED);
//    return ret;
//    double *w = (double*)PyArray_GETPTR2(ret, 0,0);
//    memset(w,size[0]*size[1],0);
    uint64_t  datasize = size[0] * size[1] *8;
    global_buff = malloc( datasize);
    //memcpy(global_buff,a)
//    memset(global_buff,0,size[0] * size[1] *8);
//    npy_intp nrepeats = PyArray_SIZE(ret)/npts;

//    double *wsave, *dptr, *rptr;
//    rptr = (double *)PyArray_DATA(ret);
//    dptr = (double *)PyArray_DATA(ret);
//    printf("\n");
    uint64_t win_size = 10;
    printf("D1: %d , D2: %d , WIN:%d \n",arr1->dimensions[0],arr1->dimensions[1],win_size);
    printf("D1: %d , D2: %d , WIN:%d \n",ret->dimensions[0],ret->dimensions[1],win_size);

//    ret = arr2;
    PyArrayObject *a = arr1;
    PyArrayObject *b = ret;
//    return ret;
    for (int i=0; i<arr1->dimensions[1]; ++i) {


        rolling_mean(a,b,i,win_size);


//        for (int j=0; j<arr1->dimensions[1]; ++j) {
//            double *v = (double*)PyArray_GETPTR2(arr1, i, j);
//            printf("%f ",*v);
//        }
//        printf("\n");
    }
//    printf("\n");
//    memcpy( b->data,global_buff,datasize);
    return ret;

//    return PyLong_FromLong(nrepeats);

    nd = PyArray_NDIM(arr1);   //number of dimensions

    Py_DECREF(arr1);
//    nd = 111;
    PyObject *t;

    t = PyTuple_New(3);
    PyTuple_SetItem(t, 0, PyLong_FromLong(1L));
    PyTuple_SetItem(t, 1, PyLong_FromLong(2L));
    PyTuple_SetItem(t, 2, PyLong_FromLong(nd));
    return t;
//    return PyInt_FromLong(nd);
//    return PyLong_FromLong(200);
//    return Py_RETURN_NONE;
}

static struct PyMethodDef methods[] = {
        {"example", example, METH_VARARGS, "descript of example"},
        {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC
initnyex (void)
{
    (void)Py_InitModule("nyex", methods);
    import_array();
}


//  python 3
//static struct PyModuleDef nyexDef =
//{
//    PyModuleDef_HEAD_INIT,
//    "nyex", /* name of module */
//    "",          /* module documentation, may be NULL */
//    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
//    methods
//};

//PyMODINIT_FUNC PyInit_nyex(void)
//{
//    return PyModule_Create(&nyexDef);
//}