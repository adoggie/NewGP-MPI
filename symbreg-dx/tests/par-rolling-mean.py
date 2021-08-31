#coding:utf-8

import os,os.path,time,datetime,traceback,json
import numpy as np
import pandas as pd

df = pd.read_parquet("../../highprice.parquet")
print(df.shape)
print('--')

# for n in range(1500):
#   r = df['000001.XSHE'].rolling(5).mean()
#   r = None

# for n in range(150):
#   # diff =  df['000001.XSHE'].rolling(2).apply(lambda x: (x[1]-x[0])/x[1])
#   diff =  df['000001.XSHE'].rolling(100).std()

# s = pd.Series( list(range(100000000)))
# s = pd.Series( list(range(100000000)))

start = time.time()

# df.rolling(100).std()

# r = df['000001.XSHE'].rolling(5).apply(lambda x: np.mean(x)  )
for n in range(10):
    # df.rolling(100).sum()
    # r = df.rolling(1000).mean()
    # df.rolling(100).std()
    df.pow(0.5)
    # np.power(df.values,0.5)

end = time.time()
print('elasped ',end - start)

# print(r)

  # print(n)