import time

N = 1000000
start = time.time()
a = [10000 for i in range(N)]
print(time.time()-start)
input()

start = time.time()
b = [10000] * N
for i in range(N):
    b[i] = i
print(time.time()-start)
input()

start = time.time()
c = [i for i in range(N)]
print(time.time()-start)
input()

start = time.time()
if 10000 in c:
    print(time.time()-start)

start = time.time()
for i in range(N):
    if c[i] == 10000:
        print(time.time() - start)
        break
