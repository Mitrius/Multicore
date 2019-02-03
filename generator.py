import sys
from random import random, randint
from scipy.linalg import eigh_tridiagonal

UPPER = 100


def generate_symmetric_matrix(size: int) -> list:
    main_diag = [random()*UPPER for elem in range(size)]
    sym_diag = [random() for elem in range(size-1)]
    return (main_diag, sym_diag)


if __name__ == "__main__":
    size = int(sys.argv[1])
    (main, diag) = generate_symmetric_matrix(size)

    with open("matrices/{0}.csv".format(size), "w") as file_out:
        main_str = ",".join([str(elem) for elem in main])
        diag_str = ",".join([str(elem) for elem in diag])
        print(size, file=file_out)
        print(main_str, file=file_out)
        print(diag_str, file=file_out)

    with open("eigvals/{0}.csv".format(size), "w") as file_out:
        eigs = eigh_tridiagonal(
            main, diag,  eigvals_only=True, tol=0.001)
        for eig in eigs:
            print(str(eig), file=file_out)
