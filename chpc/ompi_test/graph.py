import numpy as np
import matplotlib.pyplot as plt

#                 24 48 72 96
thrd_1 = np.array([666, 401, 270, 276])
thrd_2 = np.array([433, 280, 150, 138])
thrd_3 = np.array([291, 143, 139, 139]) 
thrd_4 = np.array([275, 138, 139, 144])

sthrd_1 = thrd_1[0] / thrd_1
sthrd_2 = thrd_1[0] / thrd_2
sthrd_3 = thrd_1[0] / thrd_3
sthrd_4 = thrd_1[0] / thrd_4


nodes = np.array([1, 2, 3, 4])

plt.figure(1)
plt.plot(nodes, thrd_1, label="1 threads")
plt.plot(nodes, thrd_2, label="2 threads")
plt.plot(nodes, thrd_3, label="3 threads")
plt.plot(nodes, thrd_4, label="4 threads")
plt.legend()
plt.title("DGFM OMPI RUNTIME")
plt.xlabel("nodes")
plt.ylabel("Runtime(s)")
plt.savefig('ompi_runtime.png')
plt.show()

plt.figure(2)
plt.plot(nodes, sthrd_1, label="1 threads")
plt.plot(nodes, sthrd_2, label="2 threads")
plt.plot(nodes, sthrd_3, label="3 threads")
plt.plot(nodes, sthrd_4, label="4 threads")
plt.legend()
plt.title("DGFM OMPI SpeedUp")
plt.xlabel("nodes")
plt.ylabel("SpeedUp")
plt.savefig('ompi_speedup.png')
plt.show()