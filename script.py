from functools import partial

import numpy as np
from scipy.stats import bootstrap

print(
    bootstrap(
        data=(np.arange(500),), statistic=partial(np.var, ddof=1), n_resamples=10_000
    ).standard_error
    ** 2
)
