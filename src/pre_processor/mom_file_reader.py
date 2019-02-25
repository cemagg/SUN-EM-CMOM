import numpy as np

def readMoMFile(file_path):
    file = open(file_path, 'r')
    line = file.readline()
    line = file.readline()
    content = line.split()
    sol_length = int(content[1])
    line = file.readline()

    isol = np.empty(sol_length, dtype=complex)
    index = 0

    while line:
        content = line.split()
        content = content[0][1:-1].split(',')
        isol[index] = np.complex(float(content[0]), float(content[1]))
        index = index + 1
        line = file.readline()
    
    return isol



#readMoMFile("../../examples/two_plate_array/two_plate_array_coarse.sol")