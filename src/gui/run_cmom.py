import subprocess
import time

from read_feko_files import readFEKOStrFile
from read_feko_files import errNormPercentage

from mom_file_reader import readMoMFile

def consoleCommand(command):
    process = subprocess.Popen(command,stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    return str(proc_stdout, 'utf-8')


def CMoM(exec_path, file_path, bin_path, serial=False, mpi=False, nprocs=4, mom=False,
         cbfm=False):

    if serial:
        cmd  = exec_path + "mom " + file_path
    elif mpi:
       x=0

    if cbfm:
        cmd = cmd + " --cbfm"

    t_start = time.time()
    console_output = consoleCommand(cmd)
    t_end = time.time()
    t_elapsed = t_end - t_start

    cmom_sol = readMoMFile(file_path[:-3] + "sol")
    feko_sol = readFEKOStrFile(file_path[:-3] + "str", bin_path)
    error = errNormPercentage(cmom_sol, feko_sol)
    
    return error, time


