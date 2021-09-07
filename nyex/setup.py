#!/usr/bin/env python
# -*- coding: UTF-8 -*-

from distutils.core import setup, Extension
import numpy as np

# https://blog.csdn.net/wuxianfeng1987/article/details/77175612
ext_modules = [ Extension('nyex', sources = ['nyexf-par-py3.c'] ,define_macros=[('NPY_NO_DEPRECATED_API','1')]) ]

setup(
    name = 'nyex',
    version = '1.0',
    include_dirs = [np.get_include()], #Add Include path of numpy
    ext_modules = ext_modules
)

"""
rm -rf build *.so ; python setup.py build_ext --inplace
"""