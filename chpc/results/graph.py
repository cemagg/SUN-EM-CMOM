import numpy as np
import matplotlib.pyplot as plt

thrd_4_1 = np.array([4511, 2019, 2256, 1130])
thrd_4_2 = np.array([11816,  2267, 1111,  567])
thrd_4_3 = np.array([4033,  2266, 1034,  565])
thrd_4_4 = np.array([4047,  2276, 552,  284])
serial_4 = 4511

thrd_8_1 = np.array([9143,  7570,  5083, 2545])
thrd_8_2 = np.array([9134,  3440,  2293, 1152])
thrd_8_3 = np.array([9158,  2670,  1912,  893])
thrd_8_4 = np.array([9157,  1865,  1244,  624])
serial_8 = 18219

s_thrd_4_1 = serial_4 / thrd_4_1
s_thrd_4_2 = serial_4 / thrd_4_2
s_thrd_4_3 = serial_4 / thrd_4_3
s_thrd_4_4 = serial_4 / thrd_4_4

s_thrd_8_1 = serial_8 / thrd_8_1
s_thrd_8_2 = serial_8 / thrd_8_2
s_thrd_8_3 = serial_8 / thrd_8_3
s_thrd_8_4 = serial_8 / thrd_8_4


nodes_1 = np.array([1, 2, 3, 4])
nodes_2 = np.array([2, 3, 4, 8])

plt.figure(1)
plt.plot(nodes_1, thrd_4_1, label="1 threads")
plt.plot(nodes_1, thrd_4_2, label="2 threads")
plt.plot(nodes_1, thrd_4_3, label="3 threads")
plt.plot(nodes_1, thrd_4_4, label="4 threads")
plt.legend()
plt.xticks(nodes_1)
plt.title("DGFM OMPI RUNTIME 4 ELEMENTS")
plt.xlabel("nodes")
plt.ylabel("Runtime(s)")
plt.savefig('4_element_runtime.png')

plt.figure(2)
plt.plot(nodes_2, thrd_8_1, label="1 threads")
plt.plot(nodes_2, thrd_8_2, label="2 threads")
plt.plot(nodes_2, thrd_8_3, label="3 threads")
plt.plot(nodes_2, thrd_8_4, label="4 threads")
plt.legend()
plt.xticks(nodes_2)
plt.title("DGFM OMPI RUNTIME 8 ELEMENTS")
plt.xlabel("nodes")
plt.ylabel("Runtime(s)")
plt.savefig('8_element_runtime.png')

plt.figure(3)
plt.plot(nodes_1, s_thrd_4_1, label="1 threads")
plt.plot(nodes_1, s_thrd_4_2, label="2 threads")
plt.plot(nodes_1, s_thrd_4_3, label="3 threads")
plt.plot(nodes_1, s_thrd_4_4, label="4 threads")
plt.legend()
plt.xticks(nodes_1)
plt.title("DGFM OMPI SPEEDUP 4 ELEMENTS")
plt.xlabel("nodes")
plt.ylabel("Speedup")
plt.savefig('4_element_speedup.png')

plt.figure(4)
plt.plot(nodes_2, s_thrd_8_1, label="1 threads")
plt.plot(nodes_2, s_thrd_8_2, label="2 threads")
plt.plot(nodes_2, s_thrd_8_3, label="3 threads")
plt.plot(nodes_2, s_thrd_8_4, label="4 threads")
plt.legend()
plt.xticks(nodes_2)
plt.title("DGFM OMPI SPEEDUP 8 ELEMENTS")
plt.xlabel("nodes")
plt.ylabel("Speedup")
plt.savefig('8_element_speedup.png')

plt.show()
