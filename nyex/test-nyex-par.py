import os,os.path,time,datetime,traceback,json
import numpy as np
import pandas as pd

import numpy as ny
import nyex
import fire

"""
python 2.x
-------- 
pip install pyarrow fastparquet==0.2.1
"""

df = pd.read_parquet("/home/eladmin/projects/highprice.parquet").astype('float32')
print(df.shape)
print(df['000001.XSHE'].dtypes)

# arr = np.linspace(1,10000, ROWS*COLS).reshape(ROWS,COLS) .astype('float32')

# arr2 = np.arange(ROWS*COLS).reshape(COLS,ROWS).astype('float64')
# df = pd.Series(arr)
# print arr.alignment

# df = pd.DataFrame(arr)
# print df.dtypes
print('--')


def test_par(winsize=100,loop=1):
    start = time.time()
    for _ in range(loop):
        # ret = df.rolling(winsize).mean()
        ret = df.rolling(winsize).std()
    end = time.time()
    print('test_par elapsed:', end - start)
    # print ret.values,ret.shape
    print(ret.values)

def test_nyex(winsize=100,par=0,loop=1):
    arr = df.values
    start = time.time()
    for _ in range(loop):
        # ret =nyex.rolling_mean( arr,winsize,par )
        ret =nyex.rolling_stdev( arr,winsize,par,0 )
    end = time.time()
    print('test_nyex elapsed:', end - start)
    # print(ret,ret.shape)
    # print(arr[0,:])
    # print '---'
    print(ret,ret.shape)


# a = ny.arange(100) .reshape(5,20)
# print(a)
# print(nyex.example(a))
# a = ny.arange(100) .reshape(5,20)
# # print(a)
# print( pd.DataFrame(a).rolling(5).mean() )

if __name__ == '__main__':
    fire.Fire()