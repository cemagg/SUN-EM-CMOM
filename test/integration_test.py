import os
import sys

wdir = os.path.dirname(os.path.abspath(__file__)).split("test", 1)[0]
wdir = wdir + "src/gui"
sys.path.insert(0, wdir)

from run_cmom import CMoM

def errorString(error, threshold=5.00):
    # Colors
    CPASS = '\033[92m'
    CFAIL = '\033[91m'
    CEND  = '\033[0m' 
    
    ERROR_STR = "Error(%): "
    PASS_STR = CPASS + "PASS" + CEND
    FAIL_STR = CFAIL + "FAIL" + CEND
    TWIDTH = os.get_terminal_size().columns

    if error < threshold:
        tmp_str = ERROR_STR + str(error) + " " + PASS_STR
    else:
        tmp_str = ERROR_STR + str(error) + " " + FAIL_STR

    return tmp_str.rjust(TWIDTH-1, '.') 

def CMoMTest(name, serial=True, mpi=False, nprocs=4 , mom=True, cbfm=False):

        
    TWIDTH = os.get_terminal_size().columns
    ERROR_STR = "AAA"

    # Get dir
    test_dir = os.path.dirname(os.path.abspath(__file__))
    cwd = test_dir + "/test_cases/" + name + "/"

    file_path = cwd + name + ".mom"

    exec_path = test_dir + "/.build/"
    mpi_exec_path = test_dir + "/.mpi_build/"

    bin_path = test_dir.split("test", 1)[0]
    
    print(TWIDTH * "-")
    print(name.upper().center(TWIDTH) + '\n')

    # Run solver

    if mom:
        if serial:
            print("MoM  (srl)", end="")

            error, time = CMoM(exec_path, file_path, bin_path, serial=True, mom=True)
            print(errorString(error))
        
        if mpi:
            print("MoM  (mpi)", end="")
            print(ERROR_STR)

    if cbfm:
        if serial:
            print("CBFM (srl)", end="")
            error, time = CMoM(exec_path, file_path, bin_path, serial=True, cbfm=True)
            print(errorString(error))

        if mpi:
            print("CBFM (mpi)", end="")
            print(ERROR_STR)
  
    print(TWIDTH * "-")
    

if __name__ == "__main__":
    CMoMTest("cbfm_debug")
