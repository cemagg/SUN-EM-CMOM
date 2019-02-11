cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

project(mom CXX C Fortran)

option(MPI "Compile with MPI" OFF)
option(ATLAS "Use optimized BLAS" OFF)
option(PARALLEL "Explicit Parallel Atlas" OFF)

if(MPI)
    MESSAGE("COMPILING WITH MPI")

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
                src/data_structures/triangle.h
                src/file_io/mom_file_reader.cpp
                src/mom_by_face/quadrature.cpp
                src/mom_by_face/vrhs.cpp
                src/mom_by_face/zmn_by_face.cpp
                src/parallel_mom/mpi_fill_vrhs.cpp
                src/parallel_mom/mpi_fill_zmn.cpp
                src/parallel_mom/mpi_fill_ilhs.cpp
                src/parallel_mom/mpi_mom.cpp
                src/parallel_mom/main_mpi.cpp)
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
    set(SOURCES lib/args/args.hxx
                src/data_structures/edge.h
                src/data_structures/node.h
                src/data_structures/triangle.h
                src/file_io/mom_file_reader.cpp
                src/file_io/write_ilhs_to_file.cpp
                src/mom_by_face/quadrature.cpp
                src/mom_by_face/vrhs.cpp
                src/mom_by_face/zmn_by_face.cpp
                src/mom/fill_vrhs.cpp
                src/mom/fill_zmn.cpp
                src/mom/fill_ilhs.cpp
                src/mom/main.cpp)
    add_executable(mom ${SOURCES})
    target_link_libraries(mom   -L/${CMAKE_CURRENT_SOURCE_DIR}/lib/scalapack
                                -lreflapack
                                -lrefblas
                                -ltmg 
                                -L/usr/bin
                                -lgfortran)
endif(MPI)
unset(MPI CACHE)