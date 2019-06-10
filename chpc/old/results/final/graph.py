import numpy as np
import matplotlib.pyplot as plt

thrd_50_1 =  np.array([892, 418, 261, 180])
thrd_50_8 =  np.array([907,  90, 90,  89])
thrd_50_24 = np.array([902,  79, 95,  88])
serial_50 = 1592

thrd_100_1 =  np.array([6052, 3065, 1689, 1323])
thrd_100_8 =  np.array([6098,  748,  610,  673])
thrd_100_24 = np.array([6169,  764,  645,  635])
serial_100 = 12022

# 200_thrd_1 =  np.array([,     ,     , 10518])
# 200_thrd_8 =  np.array([, 5353,     ,  5279])
# 200_thrd_24 = np.array([, 5458,     ,  5721])
# 200_serial = 0

s_thrd_50_1 = serial_50 / thrd_50_1
s_thrd_50_8 = serial_50 / thrd_50_8
s_thrd_50_24 = serial_50 / thrd_50_24


s_thrd_100_1 = serial_100 / thrd_100_1
s_thrd_100_8 = serial_100 / thrd_100_8
s_thrd_100_24 = serial_100 / thrd_100_24


nodes = np.array([2, 4, 8, 10])

plt.figure(1)
plt.plot(nodes, thrd_50_1, label="1 threads")
plt.plot(nodes, thrd_50_8, label="8 threads")
plt.plot(nodes, thrd_50_24, label="24 threads")
plt.legend()
plt.xticks(nodes)
plt.title("DGFM OMPI RUNTIME 50 ELEMENTS")
plt.xlabel("nodes")
plt.ylabel("Runtime(s)")
plt.savefig('50_element_runtime.png')

plt.figure(2)
plt.plot(nodes, thrd_100_1, label="1 threads")
plt.plot(nodes, thrd_100_8, label="8 threads")
plt.plot(nodes, thrd_100_24, label="24 threads")
plt.legend()
plt.xticks(nodes)
plt.title("DGFM OMPI RUNTIME 100 ELEMENTS")
plt.xlabel("nodes")
plt.ylabel("Runtime(s)")
plt.savefig('100_element_runtime.png')

plt.figure(3)
plt.plot(nodes, s_thrd_50_1, label="1 threads")
plt.plot(nodes, s_thrd_50_8, label="8 threads")
plt.plot(nodes, s_thrd_50_24, label="24 threads")
plt.legend()
plt.xticks(nodes)
plt.title("DGFM OMPI SPEEDUP 50 ELEMENTS")
plt.xlabel("nodes")
plt.ylabel("Speedup")
plt.savefig('50_element_speedup.png')

plt.figure(4)
plt.plot(nodes, s_thrd_100_1, label="1 threads")
plt.plot(nodes, s_thrd_100_8, label="8 threads")
plt.plot(nodes, s_thrd_100_24, label="24 threads")
plt.legend()
plt.xticks(nodes)
plt.title("DGFM OMPI SPEEDUP 100 ELEMENTS")
plt.xlabel("nodes")
plt.ylabel("Speedup")
plt.savefig('100_element_speedup.png')

plt.show()
