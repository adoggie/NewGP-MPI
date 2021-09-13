import os,os.path,time,datetime,traceback,json
import numpy as np
import pandas as pd

import numpy as ny
import nyex2
import fire

"""
python 2.x
-------- 
pip install pyarrow fastparquet==0.2.1
"""

df = None

def init_data():
    global  df
    df = pd.read_parquet("/home/eladmin/projects/highprice.parquet").astype('float32')
# df = pd.read_parquet("/Users/scott/Desktop/Projects/EL/GA/highprice.parquet").astype('float32')
# print(df.shape)
# print(df['000001.XSHE'].dtypes)

# arr = np.linspace(1,10000, ROWS*COLS).reshape(ROWS,COLS) .astype('float32')

# arr2 = np.arange(ROWS*COLS).reshape(COLS,ROWS).astype('float64')
# df = pd.Series(arr)
# print arr.alignment

# arr = np.arange(100)
# df = pd.DataFrame(arr)
# print df.dtypes
    print('--')


def test_par(winsize=100,loop=1):
    init_data()
    start = time.time()
    for _ in range(loop):
        # ret = df.rolling(winsize).mean()
        # ret = df.rolling(winsize).skew()
        ret = df.rolling(winsize).apply(lambda x: np.sum(x <= x[-1]) / (winsize), raw=True, engine='numba')
    end = time.time()
    print('test_par elapsed:', end - start)
    # print ret.values,ret.shape
    print(ret.values)

def test_nyex(winsize=100,par=0,loop=1):
    init_data()
    arr = df.values
    # arr = np.arange(100*200)
    # arr = [np.arange(100)] #,np.arange(100)]
    # arr = np.transpose(arr)
    # arr = arr.astype('float32')

    start = time.time()
    for _ in range(loop):
        # ret =nyex.rolling_mean( arr,winsize,par )
        # ret =nyex2.rolling_std( arr,winsize,par )
        # ret =nyex2.rolling_skew( arr,winsize,par )
        ret = nyex2.rolling_tsrank(arr,winsize,par)
    end = time.time()
    print('test_nyex elapsed:', end - start)
    # print(ret,ret.shape)
    # print(arr[0,:])
    # print '---'
    print(ret,ret.shape)


def test_simple():
    # 统计在集合中小于等于最后一个的数值个数   np.sum(x <= x[-1])

    ar = np.arange(100).reshape(10,10)
    df = pd.DataFrame(ar).astype('float32')

    period1 = 3
    factor1 = df
    factor1 = factor1.rolling(period1).apply(lambda x: np.sum(x <= x[-1]) / (period1), raw=True)
    # 统计在集合中小于等于最后一个的数值个数
    print(df)
    ar = ar[:,0]
    print(ar)
    print(ar[-1])
    print(np.sum(ar <= ar[-1]))
    print( ar <= ar[-1] )
    print(factor1)
    print('-'*40)
    ret = nyex2.rolling_tsrank(df.values,period1,0)
    print(ret)

if __name__ == '__main__':
    fire.Fire()