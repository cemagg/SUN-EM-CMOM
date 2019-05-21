cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

project(mom CXX C Fortran)

option(MPI "Compile with MPI" OFF)
option(ATLAS "Use optimized BLAS" OFF)
option(PARALLEL "Explicit Parallel Atlas" OFF)
option(OpMP "Compile with OMP" OFF)

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
                src/data_structures/cblacs_config.h
                src/data_structures/triangle.h
                src/file_io/mom_file_reader.cpp
                src/file_io/mom_file_writer.cpp
                src/mom/mom_helpers/quadrature.cpp
                src/mom/mom_helpers/vrhs.cpp
                src/mom/mom_helpers/zmn_by_face.cpp
                src/helpers/globals.cpp
                src/mom/parallel_mom/mpi_fill_vrhs.cpp
                src/mom/parallel_mom/mpi_fill_zmn.cpp
                src/mom/parallel_mom/mpi_fill_ilhs.cpp
                src/mom/parallel_mom/mpi_mom.cpp
                src/cbfm/cbfm_helpers/cbfm_helpers.cpp
                src/cbfm/parallel_cbfm/mpi_scalapack_cbfm/mpi_scalapack_cbf_reducer.cpp
                src/cbfm/parallel_cbfm/mpi_scalapack_cbfm/mpi_scalapack_cbfm_solve_ilhs.cpp
                src/cbfm/parallel_cbfm/mpi_scalapack_cbfm/mpi_scalapack_edd_cbfm.cpp
                src/cbfm/parallel_cbfm/mpi_scalapack_cbfm/mpi_scalapack_fill_self_matrices_vectors.cpp
                src/cbfm/parallel_cbfm/mpi_scalapack_cbfm/mpi_scalapack_generate_cbfs.cpp
                src/cbfm/parallel_cbfm/mpi_scalapack_cbfm/mpi_scalapack_z_v_reducer.cpp
                src/cbfm/parallel_cbfm/mpi_cbfm.cpp
                src/main.cpp)
    add_executable(mom_mpi ${SOURCES})

    if(ATLAS OR PARALLEL)
        target_link_libraries(mom_mpi ${MPI_LIBRARIES}
                                    OpenMP::OpenMP_CXX
                                    ${SCALAPACK_LIBRARIES}
                                    ${BLAS_LIBRARIES}
                                    ${LAPACK_LIBRARIES})
    else()
        target_link_libraries(mom_mpi ${MPI_LIBRARIES}
                                    OpenMP::OpenMP_CXX
                                    -L/${CMAKE_CURRENT_SOURCE_DIR}/lib/scalapack
                                    -lscalapack
                                    -lrefblas
                                    -ltmg
                                    -lreflapack
                                    -L/usr/bin
                                    -lgfortran)
    endif()

else()
    MESSAGE("COMPILING WITHOUT MPI")
    if(OpMP)
        add_definitions(-DOMP)
    endif(OpMP)
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
