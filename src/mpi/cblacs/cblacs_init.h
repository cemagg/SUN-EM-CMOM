#ifndef CBLACS_INIT_H
#define CBLACS_INIT_H

#include "../../data_structures/cblacs_config.h"

extern "C"
{
	void Cblacs_pinfo(int*, int*); 
    void Cblacs_get(int , int, int*);
    void Cblacs_gridinit(int*, const char*, int, int);
    void Cblacs_gridinfo(int, int*, int*, int*,int*);
}

void cblacsInit(CBLACSConfig &config);
void cblacsGridSize(int &total_proc_rows, int &total_proc_cols, int n_procs);
void cblacsBlockSize(int &block_size, int matrix_size);

#endif

