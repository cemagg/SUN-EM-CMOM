#include "cblacs_init.h"

void cblacsInit(CBLACSConfig &config)
{
    Cblacs_get(0,0, &config.context);

    //TODO: Variable Grid and Block Size
    config.total_proc_rows = 2;
    config.total_proc_cols = 2;
    config.block_size = 3;

    Cblacs_gridinit(&config.context, "Row-major", config.total_proc_rows,
                    config.total_proc_cols);

    Cblacs_gridinfo(config.context, &config.total_proc_rows, &config.total_proc_cols,
                    &config.proc_row, &config.proc_col);

}


