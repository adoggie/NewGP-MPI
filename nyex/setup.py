#!/usr/bin/env python
# -*- coding: UTF-8 -*-

from distutils.core import setup, Extension
import numpy as np

# https://blog.csdn.net/wuxianfeng1987/article/details/77175612
ext_modules = [ Extension('nyex2', sources = ['nyex-welford-py3.cpp'] ,define_macros=[('NPY_NO_DEPRECATED_API','1')],
                          libraries=['tbb'],
                          library_dirs=['/usr/local/lib', '/usr/lib/x86_64-linux-gnu'],
                          extra_compile_args=['-std=c++17'],
                          include_dirs = [np.get_include()],
                          ) ]
# ext_modules = [ Extension('nyex', sources = ['nyexf-par-py3.c'] ,define_macros=[('NPY_NO_DEPRECATED_API','1')]) ]


# module1 = Extension('demo',
#                     define_macros = [('MAJOR_VERSION', '1'),
#                                      ('MINOR_VERSION', '0')],
#                     include_dirs = ['/usr/local/include'],
#                     libraries = ['tcl83'],
#                     library_dirs = ['/usr/local/lib'],
#                     sources = ['demo.c'])

setup(
    name = 'nyex2',
    version = '1.0',
    include_dirs = [np.get_include()], #Add Include path of numpy
    ext_modules = ext_modules
)

"""
rm -rf build *.so ; python setup.py build_ext --inplace
"""