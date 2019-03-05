# CMoM - A C++ Method of Moments Solver

### <u>Description</u>

CMoM is a C++ Computational Electromagnetic (CEM)  solver focusing on the Method of Moments (MoM).  CMoM can compute both serially and in parallel (using mpi). A pre-processor, written in python is also included to provide a graphical interface as well as read necessary geometry and create the necessary input files. The pre-processor is also used for post processing to display results and compare CMoM solutions to commercial CEM solvers (currently the only compatible commercial solver is FEKO).

**CMoM is being developed and used in a Linux environment**

#### <u>Current Solvers</u>

##### Method of Moments (MoM)

The MoM is done according to the work presented in "Electromagnetic Scattering by Surfaces of Arbitrary Shape" by Rao, Wilton and Glisson (1982).

##### Characteristic Basis Function Method (CBFM)

The CBFM is done according to the work presented in "Characteristic Basis Function Method: A New Technique For Efficient Solution of Method of Moments Matrix Equations" by Prakash and Mittra (2003).

##### Physical Optics (PO)

TBD

##### Iterative Field Bouncing (IFB)

TBD

### <u>Status</u>

| Solver            | Serial | Parallel       |
| ----------------- | ------ | -------------- |
| MoM               | Done   | Done           |
| Disconnected CBFM | Done   | In Development |
| Connected CBFM    | TBD    | TBD            |
| PO                | TBD    | TBD            |
| IFB               | TBD    | TBD            |

### <u>Roadmap</u>

##### Meta

- Write solution in binary
- Better documentation

##### Pre-Processor

- Write input file in binary
- Read and compare with CST
- Read and compare with the one from the presentation
- Calculate and display properties from the solution (Nearfield/Input Impedance/etc)
- Better Logging
- Run parallel solver

##### MoM

- Singularity Treatment
- Near Singularity Treatment
- Proper Support For Edge Feeds (Delta Gap)

##### Disconnected CBFM

- Parallel CBFM
- Integrate an FPGA for some of the Matrix Matrix and Matrix Vector calculations 

##### Connected CBFM

##### PO

##### IFB

### <u>Dependencies</u>

#### Required

- CMake
- MPI Library (OpenMPI or MPICH)

#### Optional
- Python 3
- numpy

### <u>Serial Install Guide</u>

`mkdir build`

`cd build`

`cmake ..`

`make`

`./mom -h`



### <u>Parallel Install Guide</u>

`mkdir build_mpi`

`cd build_mpi`

`cmake -DMPI=ON ..`

`make`

`./mom_mpi -h`



### <u>Usage From GUI</u>

`python src/pre_processor/main.py`

