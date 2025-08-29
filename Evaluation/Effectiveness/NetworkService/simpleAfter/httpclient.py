import requests
import time

# 初始化一个变量
total = 0
data = []
# 循环 100 次
for i in range(5000):
    tic = time.perf_counter()
    r = requests.get("http://192.168.0.102:80/hello")
    toc = time.perf_counter()
    #data[i] = toc - tic
    print(toc - tic)

# for da in data:
#     print(da)
#     total = total + da
# print(total)
# print(total/100)

