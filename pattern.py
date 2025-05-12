import os
import sys
import time
import threading
import random
import datetime
import pandas as pd
from typing import List, Dict, Tuple, Optional


# slide window for dbtop
# 1. find wvtop and wvbot, top is 1.3atr,  bot left 1.3 right 0.7
# 2. first down time < 20k
# 3. when signal in second uping is pbtm or kwith big vol,
#    check it is < top, check the box , check the vol

class DbTop:
  def dbtop_cond(df):
    # df window is 50k
    # 1. find t and b

    # 2.
    pass
