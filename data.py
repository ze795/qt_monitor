import sys
import time
import threading
import random
import datetime
import pandas as pd
import akshare as ak

from typing import List, Dict, Tuple, Optional

class ATRCalculator:
    def cal_60matr(self, symbol):
        df = ak.futures_zh_minute_sina(symbol=symbol, period="60")
        atr = 11
        return atr

    def atr_cond(self, symbol, dir):
        atr = self.cal_60matr(symbol)
        data_15m = ak.futures_zh_minute_sina(symbol="RB0", period="15")
        if dir == "short":
            recent_20k_h = data_15m['high'].tail(20).max()
            cur_l = data_15m['low'].tail(1)
            if (recent_20k_h - cur_l) > 1.3 * atr:
                return 1

        if dir == "long":
            recent_20k_l = data_15m['low'].tail(20).max()
            cur_h = data_15m['high'].tail(1)
            if (cur_h - recent_20k_l) > 1.3 * atr:
                return 1


def getRealTimeP(symbol):
    return ak.futures_zh_spot(symbol=symbol, market="CF", adjust='0')['current_price']


# futures_zh_spot_df = ak.futures_zh_spot(symbol='RB0', market="CF", adjust='0')
# print(futures_zh_spot_df['current_price'])

# futures_zh_minute_sina_df = ak.futures_zh_minute_sina(symbol="RB0", period="15")
# print(futures_zh_minute_sina_df)

# futures_zh_minute_sina_df = ak.futures_zh_minute_sina(symbol="RB0", period="60")
# print(futures_zh_minute_sina_df)
