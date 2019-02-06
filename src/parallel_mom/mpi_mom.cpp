#include "mpi_mom.h"


void mpiPerformMoM(std::map<std::string, std::string> &const_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> nodes)
{
    MPI_Init(NULL, NULL);
	// Some constants
	int zero = 0;
	int one = 1;
	int matrix_size = edges.size(); //TODO: Change

	// Get BLACS context
	int context;
	Cblacs_get(0, 0, &context);

	// Create BLACS grid
	//TODO: make variable grid size
	int total_proc_rows = 2;
	int total_proc_cols = 2;
	Cblacs_gridinit(&context, "Row-major", total_proc_rows, total_proc_cols);

	// Get proc coords
	int proc_row;
	int proc_col;
	Cblacs_gridinfo(context, &total_proc_rows, &total_proc_cols, &proc_row, &proc_col);

	//TODO: dynamic block size
	int block_size = 64;

	// Get the amount of rows for vrhs on each proc
	int num_vrhs_rows = numroc_(&matrix_size, &block_size, &proc_row, &zero, &total_proc_rows);
	int num_vrhs_cols = numroc_(&one, &block_size, &proc_col, &zero, &total_proc_cols);

	//************************************
	std::ofstream file;
	std::string file_name = std::to_string(proc_row) + std::to_string(proc_col) +".txt"; 
	file.open(file_name);
	file << "vrhs rows -> " <<num_vrhs_rows << std::endl;
	file << "vrhs cols -> "<<num_vrhs_cols << std::endl;
	//************************************

	// Create local vrhs matrix
  file << num_vrhs_rows << std::endl;
  file << num_vrhs_cols << std::endl;

  
     std::complex<double> *vrhs = new std::complex<double>[num_vrhs_rows * num_vrhs_cols]();
  

	mpi_fill_vrhs(vrhs,
           	      total_proc_rows,
                  total_proc_cols,
                  proc_row,
                  proc_col,
                  block_size,
                  const_map,
                  triangles,
                  edges);


    // Get amount of rows and cols for local zmn
    int num_zmn_rows = numroc_(&matrix_size, &block_size, &proc_row, &zero, &total_proc_rows);
    int num_zmn_cols = numroc_(&matrix_size, &block_size, &proc_col, &zero, &total_proc_cols);

    file << "zmn rows -> " <<num_zmn_rows << std::endl;
    file << "zmn cols -> " <<num_zmn_cols << std::endl;

    // Create local zmn matrix
    std::complex<double> *zmn = new std::complex<double>[num_zmn_rows * num_zmn_cols]();


    mpiFillZmn(zmn,
               num_zmn_cols,
               total_proc_rows,
               total_proc_cols,
               proc_row,
               proc_col,
               block_size,
               const_map,
               triangles,
               edges,
               nodes);

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            file << zmn[i + j * num_zmn_cols];
        }
        file << std::endl;
    }
    // file << "zmn count -> " << count << std::endl;

    file << vrhs[0] << std::endl;
    mpiFillIlhs(zmn,
         vrhs,
         num_zmn_rows,
         num_vrhs_rows,
         matrix_size,
         block_size,
         context);




    file << vrhs[0] << std::endl;
    // file << zmn[0] << std::endl;
    MPI_Finalize();
}
