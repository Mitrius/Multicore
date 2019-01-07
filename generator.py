import numpy as np
import sys
import scipy.sparse as sp
from random import random, randint


def generate_symmetric_matrix(size: int, upper: float) -> np.ndarray:
    main_diag = [str(random()*upper) for elem in range(size)]
    sym_diag = [str(random()*upper) for elem in range(size-1)]
    return (main_diag, sym_diag)


if __name__ == "__main__":
    size = int(sys.argv[1])
    upper_limit = float(sys.argv[2])
    (main, diag) = generate_symmetric_matrix(size, upper_limit)

    with open("matrices/{0}_max_{1}.csv".format(size, upper_limit), "w") as file_out:
        main_str = ",".join(main)
        diag_str = ",".join(diag)
        print(size, file=file_out)
        print(main_str, file=file_out)
        print(diag_str, file=file_out)
