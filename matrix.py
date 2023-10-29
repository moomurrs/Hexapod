def mat_mult(A:list, B:list) -> list:
    row_a:int = len(A)
    col_a:int = len(A[0])
    row_b:int = len(B)
    col_b:int = len(B[0])

    if(col_a != row_b):
        print("Error! Matrix sizes are not compatible")
        return None
    
    C:list = []
    for row in range(row_a):
        curr_row:list = []
        for col in range(col_b):
            curr_row.append(0)
        C.append(curr_row)

    for i in range(row_a):
        for j in range(col_b):
            curr_val:int = 0
            for k in range(col_a):
                C[i][j] += A[i][k] * B[k][j]
    return C

def print_mat(A:list) -> None:
    for row in range(len(A)):
        print("[ ", end="")
        for col in range(len(A[0])):
            print(A[row][col], end=" ")
        print("]")