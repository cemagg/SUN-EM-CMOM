#!/usr/bin/env python

import os

from integration_test import CMoMTest

if __name__ == "__main__":
    TWIDTH = os.get_terminal_size().columns

    print(TWIDTH * "=")
    print("CMoM".center(TWIDTH))
    print(TWIDTH * "=")
    
    CMoMTest("pec_plate")
    CMoMTest("non_origin_plane_wave_plate")
    CMoMTest("rotated_plate")
    CMoMTest("cbfm_debug", dgfm=True)
    CMoMTest("strip_dipole_array", dgfm=True)
    CMoMTest("bow_tie_array", dgfm=True)
    #CMoMTest("random_plate_array", dgfm=True)

















