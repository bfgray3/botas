import sys
from functools import partial

import numpy as np
from scipy.stats import bootstrap

N, NUM_REPLICATES = (int(arg) for arg in sys.argv[1:])

boot = bootstrap(
    data=(np.arange(N, dtype=np.float64),),
    statistic=partial(np.var, ddof=1),
    n_resamples=NUM_REPLICATES,
    method="percentile",
)


print(
    "variance: ",
    boot.standard_error**2,
    "; confidence_interval: (",
    boot.confidence_interval.low,
    ", ",
    boot.confidence_interval.high,
    ")",
    sep="",
)
