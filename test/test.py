#!/usr/bin/env python

import os

from integration_test import CMoMTest

if __name__ == "__main__":
    TWIDTH = os.get_terminal_size().columns

    print(TWIDTH * "=")
    print("CMoM".center(TWIDTH))
    print(TWIDTH * "=")
    
    CMoMTest("cbfm_debug", cbfm=True)

















