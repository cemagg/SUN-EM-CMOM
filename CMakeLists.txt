cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

project(mom CXX C Fortran)

option(MPI "Compile with MPI" OFF)
option(ATLAS "Use optimized BLAS" OFF)
option(PARALLEL "Explicit Parallel Atlas" OFF)
option(OpMP "Compile with OMP" OFF)
option(CHPC "Compile with OMP" OFF)
option(COMP "Compile with OMP" OFF)
option(CACC "Compile with OMP" OFF)
option(PGGG "Compile with OMP" OFF)
option(CATL "Compile with OMP" OFF)

if(MPI)
    MESSAGE("COMPILING WITH MPI")
    add_definitions(-DPARALLEL)

    find_package(MPI REQUIRED)
    find_package(OpenMP REQUIRED)

    if(ATLAS)
        message("COMPILING WITH ATLAS")
        find_package(LAPACK REQUIRED)
        find_package(BLAS REQUIRED)
        SET(SCALAPACK_DIR "/usr/lib/scalapack/build")
        find_package(SCALAPACK REQUIRED)
        SET(SCALAPACK_LIBRARIES "/usr/lib/scalapack/build/lib/libscalapack.so")
    elseif(PARALLEL)
        message("COMPILING PARALLEL ATLAS")
        SET(LAPACK_LIBRARIES "/usr/lib/libptlapack.a")
        SET(BLAS_LIBRARIES "/usr/lib/libtatlas.so")
        SET(SCALAPACK_LIBRARIES "/usr/lib/scalapack/build/lib/libscalapack.so")
    endif()

    message(${CMAKE_SOURCE_DIR})
    include_directories(${MPI_INCLUDE_PATH})
    set(SOURCES lib/args/args.hxx
                src/data_structures/edge.h
                src/data_structures/node.h
                src/data_structures/label.h
                #src/data_structures/cblacs_config.h
                src/data_structures/triangle.h
                src/file_io/mom_file_reader.cpp
                src/file_io/mom_file_writer.cpp
                src/solvers/mom/mom_helpers/quadrature.cpp
                src/solvers/mom/mom_helpers/vrhs.cpp
                src/solvers/mom/mom_helpers/zmn_by_face.cpp
                src/helpers/globals.cpp
                #src/solvers/mom/parallel_mom/mpi_fill_vrhs.cpp
                #src/solvers/mom/parallel_mom/mpi_fill_zmn.cpp
                #src/solvers/mom/parallel_mom/mpi_fill_ilhs.cpp
                #src/solvers/mom/parallel_mom/mpi_mom.cpp
                src/solvers/mom/serial_mom/fill_vrhs.cpp
                src/solvers/mom/serial_mom/fill_zmn.cpp
                src/solvers/mom/serial_mom/fill_ilhs.cpp
                src/solvers/dgfm/dgfm_helpers/dgfm_helpers.cpp
                src/solvers/dgfm/dgfm_helpers/dgfm_weights.cpp
                src/solvers/dgfm/serial_dgfm/dgfm_row.cpp
                src/solvers/dgfm/parallel_dgfm/mpi_dgfm.cpp
                src/main.cpp)
    add_executable(mom_mpi ${SOURCES})

    if(ATLAS OR PARALLEL)
        target_link_libraries(mom_mpi ${MPI_LIBRARIES}
                                    OpenMP::OpenMP_CXX
                                    ${SCALAPACK_LIBRARIES}
                                    ${BLAS_LIBRARIES}
                                    ${LAPACK_LIBRARIES})
    elseif(CHPC)
        find_package(LAPACK REQUIRED)
        find_package(BLAS REQUIRED)

        if(COMP)
            set(CMAKE_CXX_FLAGS "-mp -pgf90libs -Mscalapack -std=c++11 ")
        else()
            set(CMAKE_CXX_FLAGS "-pgf90libs -Mscalapack -std=c++11 ")
        endif()

        target_link_libraries(mom_mpi ${MPI_LIBRARIES}
                                      ${BLAS_LIBRARIES}
                                      ${LAPACK_LIBRARIES})
    elseif(CATL)
            set(CMAKE_CXX_FLAGS "-O3 ")
            target_link_libraries(mom_mpi ${MPI_LIBRARIES}
                                  OpenMP::OpenMP_CXX
                                  #-L/apps/scalapack/gcc/current/lib64
                                  #-lscalapack
                                  -L/apps/libs/openblas/0.2.19/gcc-6.1.0/lib
                                  -lopenblas
                                  -llapack
                                  -L/cm/local/apps/gcc/6.1.0/bin/gfortran
                                  -lgfortran)

    else()
        set(CMAKE_CXX_FLAGS "-O3 ")

        target_link_libraries(mom_mpi ${MPI_LIBRARIES}
                                    OpenMP::OpenMP_CXX
                                    -L/${CMAKE_CURRENT_SOURCE_DIR}/lib/scalapack
                                    -lscalapack
                                    -lreflapack
                                    -lrefblas
                                    -ltmg
                                    -L/usr/bin
                                    -lgfortran)
    endif()

else()
    MESSAGE("COMPILING WITHOUT MPI")

    set(SOURCES lib/args/args.hxx
                src/data_structures/edge.h
                src/data_structures/node.h
                src/data_structures/triangle.h
                src/data_structures/label.h
                src/data_structures/excitation.h
                src/file_io/mom_file_reader.cpp
                src/file_io/mom_file_writer.cpp
                src/solvers/mom/mom_helpers/quadrature.cpp
                src/solvers/mom/mom_helpers/vrhs.cpp
                src/solvers/mom/mom_helpers/zmn_by_face.cpp
                src/helpers/globals.cpp
                src/solvers/mom/serial_mom/fill_vrhs.cpp
                src/solvers/mom/serial_mom/fill_zmn.cpp
                src/solvers/mom/serial_mom/fill_ilhs.cpp
                src/solvers/mom/serial_mom/mom.cpp
                src/solvers/cbfm/cbfm_helpers/cbfm_helpers.cpp
                src/solvers/cbfm/serial_cbfm/cbfm.cpp
                src/solvers/cbfm/serial_cbfm/serial_edd_cbfm.cpp
                src/solvers/cbfm/serial_cbfm/fill_self_matrices_vectors.cpp
                src/solvers/cbfm/serial_cbfm/serial_cbf_reducer.cpp
                src/solvers/cbfm/serial_cbfm/serial_cbfm_solve_ilhs.cpp
                src/solvers/cbfm/serial_cbfm/serial_generate_cbfs.cpp
                src/solvers/cbfm/serial_cbfm/serial_z_v_reducer.cpp
                src/solvers/dgfm/dgfm_helpers/dgfm_helpers.cpp
                src/solvers/dgfm/dgfm_helpers/dgfm_weights.cpp
                src/solvers/dgfm/serial_dgfm/dgfm_row.cpp
                src/solvers/dgfm/serial_dgfm/dgfm.cpp
                src/main.cpp)
    add_executable(mom ${SOURCES})

    if(OpMP)
        find_package(OpenMP REQUIRED)
        target_link_libraries(mom   OpenMP::OpenMP_CXX
                                    -L/${CMAKE_CURRENT_SOURCE_DIR}/lib/scalapack
                                    -lreflapack
                                    -lrefblas
                                    -ltmg 
                                    -L/usr/bin
                                    -lgfortran)
    elseif(PGGG)
        find_package(LAPACK REQUIRED)
        find_package(BLAS REQUIRED)
        set(CMAKE_CXX_FLAGS "-acc -ta=tesla:managed -fast -Minfo=accel -pgf90libs -std=c++11 ")
        target_link_libraries(mom ${BLAS_LIBRARIES}
                                  ${LAPACK_LIBRARIES})

    elseif(CHPC)
        find_package(LAPACK REQUIRED)
        find_package(BLAS REQUIRED)

        if(COMP)
        find_package(OpenMP REQUIRED)

        set(CMAKE_CXX_FLAGS "-mp -pgf90libs -std=c++11 ")

        target_link_libraries(mom ${BLAS_LIBRARIES}
                                  ${LAPACK_LIBRARIES})

        elseif(CACC)
		set(CMAKE_CXX_FLAGS "-acc -ta=tesla:managed -Minfo=accel -pgf90libs -std=c++11 ")

        	target_link_libraries(mom ${BLAS_LIBRARIES}
                	                  ${LAPACK_LIBRARIES})

        else()
            set(CMAKE_CXX_FLAGS "-pgf90libs -std=c++11 ")

            target_link_libraries(mom ${BLAS_LIBRARIES}
                                      ${LAPACK_LIBRARIES})
        endif()


    else()

        target_link_libraries(mom   -L/${CMAKE_CURRENT_SOURCE_DIR}/lib/scalapack
                                    -lreflapack
                                    -lrefblas
                                    -ltmg 
                                    -L/usr/bin
                                    -lgfortran)
    endif(OpMP)

endif(MPI)
unset(MPI CACHE)
