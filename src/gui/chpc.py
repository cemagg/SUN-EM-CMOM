import numpy as np
import matplotlib.pyplot as plt 
import os
from mom_file_reader import readMoMFile
from read_feko_files import readFEKOStrFile
from read_feko_files import errNormPercentage



if __name__ == "__main__":

    working_dir = os.path.dirname(os.path.abspath(__file__))
    sp = working_dir.split("src", 1)
    cwd = sp[0]


    isol = readFEKOStrFile("/home/tameez/tmp/bow_tie_array_small.str", cwd)
    csol = readMoMFile("/home/tameez/tmp/bow_tie_array_small.sol")
    aisol = np.abs(isol)
    error_p = errNormPercentage(isol, csol)

    feko = open("/home/tameez/tmp/ascii_bow_tie_array_small.str", "r")
    cmom = open("/home/tameez/tmp/ccmom_bow_tie_array_small.str", "w")

    line = feko.readline()
    cmom.write(line)
    line = feko.readline()
    cmom.write(line)
    line = feko.readline()
    cmom.write(line)
    line = feko.readline()
    cmom.write(line)
    line = feko.readline()
    cmom.write(line)
    #file.write("%-30s\t\t%d\n\n" % ("NUM_FIELDS", len(const)))

    for num in csol:
        cmom.write("(%20.10e,%20.10e)\n" % (num.real, num.imag))

    cmom.write("--- separation of different blocks ---")        
    #plt.figure(1)
    #plt.plot(isol.real, label = "fekoreal")
    #plt.plot(csol.real, label = "cmomreal")
    #plt.legend()

    #plt.figure(2)
    #plt.plot(isol.imag, label = "fekoimag")
    #plt.plot(csol.imag, label = "cmomimag")
    #plt.legend()

    #plt.show()

    feko.close()
    cmom.close()