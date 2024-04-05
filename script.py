import sys
from functools import partial

import numpy as np
from scipy.stats import bootstrap

N, NUM_REPLICATES = (int(arg) for arg in sys.argv[1:])

print(
    bootstrap(
        data=(np.arange(N, dtype=np.float64),),
        statistic=partial(np.var, ddof=1),
        n_resamples=NUM_REPLICATES,
    ).standard_error
    ** 2
)
