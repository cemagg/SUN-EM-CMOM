import numpy as np
import matplotlib.pyplot as plt

bta_100 = np.array([12.09, 6.58, 4.47, 4.49])
amdahl = np.array([1, 2, 3, 4])


bta_100 = bta_100[0] / bta_100
amdahl = amdahl / amdahl[0]

nodes = np.array([1, 2, 3, 4])

plt.figure(1)
plt.plot(nodes, bta_100, label="19500 unknowns")
plt.plot(nodes, amdahl, label="Ideal")
plt.legend()
plt.title("DGFM MPI CHPC NORMAL SPEEDUP")
plt.xlabel("nodes")
plt.ylabel("SpeedUp")
plt.savefig('mpi_speedup_normal.png')
plt.show()


