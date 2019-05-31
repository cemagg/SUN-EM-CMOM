import numpy as np
import matplotlib.pyplot as plt

50_thrd_1 =  np.array([, 418, , 180])
50_thrd_8 =  np.array([,  90, ,  89])
50_thrd_24 = np.array([,  79, ,  88])
50_serial = 0

100_thrd_1 =  np.array([6052, 3065, 1689, 1323])
100_thrd_8 =  np.array([6098,  748,  610,  673])
100_thrd_24 = np.array([    ,  764,  645,  635])
100_serial = 0

200_thrd_1 =  np.array([,     ,     , 10518])
200_thrd_8 =  np.array([, 5353,     ,  5279])
200_thrd_24 = np.array([, 5458,     ,  5721])
200_serial = 0





sthrd_1 = thrd_1[0] / thrd_1

nodes = np.array([2, 4, 8, 10])

# plt.figure(1)
# plt.plot(nodes, thrd_1, label="1 threads")
# plt.plot(nodes, thrd_2, label="2 threads")
# plt.plot(nodes, thrd_3, label="3 threads")
# plt.plot(nodes, thrd_4, label="4 threads")
# plt.legend()
# plt.title("DGFM OMPI RUNTIME")
# plt.xlabel("nodes")
# plt.ylabel("Runtime(s)")
# plt.savefig('ompi_runtime.png')
# plt.show()
