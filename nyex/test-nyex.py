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

# df = pd.read_parquet("/home/eladmin/projects/highprice.parquet") #.astype('float32')
# print(df.shape)
# print(df['000001.XSHE'].dtypes)
ROWS = 12*1000
COLS = 10*1000
# arr = np.arange(ROWS*COLS).reshape(ROWS,COLS) .astype('float32')
arr = np.linspace(1,10000, ROWS*COLS).reshape(ROWS,COLS) .astype('float32')

# arr2 = np.arange(ROWS*COLS).reshape(COLS,ROWS).astype('float64')
# df = pd.Series(arr)
# print arr.alignment

df = pd.DataFrame(arr)
# print df.dtypes
print('--')


def test_par():
    start = time.time()
    ret = df.rolling(50).mean()
    end = time.time()
    print('elapsed:', end - start)
    # print ret.values,ret.shape
    print(ret.values)

def test_nyex():
    arr = df.values
    start = time.time()
    ret =nyex.example( arr )
    end = time.time()
    print('elapsed:', end - start)
    print(ret,ret.shape)
    # print(arr[0,:])
    # print '---'
    # print ret,ret.shape


# a = ny.arange(100) .reshape(5,20)
# print(a)
# print(nyex.example(a))
# a = ny.arange(100) .reshape(5,20)
# # print(a)
# print( pd.DataFrame(a).rolling(5).mean() )

if __name__ == '__main__':
    fire.Fire()