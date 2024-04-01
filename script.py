from functools import partial

import numpy as np
from scipy.stats import bootstrap

N = 50_000
NUM_REPLICATES = 100_000

print(
    bootstrap(
        data=(np.arange(N, dtype=np.float64),),
        statistic=partial(np.var, ddof=1),
        n_resamples=NUM_REPLICATES,
    ).standard_error
    ** 2
)
