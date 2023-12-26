/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int row, col, row_tile; // , col_tile;
    int t0,t1,t2,t3;
    int t4,t5,t6,t7;
    // int t8,t9,t10,t11,t12,t13,t14,t15;
    // t16,t17,t18,t19;
    // int t20,t21,t22,t23,t24,t25,t26,t27,t28,t29;
    // int t30,t31; 
    for (row = 0; row < N; row = row + 8) {
        for (col = 0; col < M; col = col + 16) {
            for (row_tile = row; row_tile < row+8; row_tile += 1) {
                t0 = A[row_tile][col+0];
                t1 = A[row_tile][col+1];
                t2 = A[row_tile][col+2];
                t3 = A[row_tile][col+3];
                t4 = A[row_tile][col+4];
                t5 = A[row_tile][col+5];
                t6 = A[row_tile][col+6];
                t7 = A[row_tile][col+7];
                // t8 = A[row_tile][col+8];
                // t9 = A[row_tile][col+9];
                // t10 = A[row_tile][col+10];
                // t11 = A[row_tile][col+11];
                // t12 = A[row_tile][col+12];
                // t13 = A[row_tile][col+13];
                // t14 = A[row_tile][col+14];
                // t15 = A[row_tile][col+15];
                // t16 = A[row_tile][col+16];
                // t17 = A[row_tile][col+17];
                // t18 = A[row_tile][col+18];
                // t19 = A[row_tile][col+19];
                // t20 = A[row_tile][col+20];
                // t21 = A[row_tile][col+21];
                // t22 = A[row_tile][col+22];
                // t23 = A[row_tile][col+23];
                // t24 = A[row_tile][col+24];
                // t25 = A[row_tile][col+25];
                // t26 = A[row_tile][col+26];
                // t27 = A[row_tile][col+27];
                // t28 = A[row_tile][col+28];
                // t29 = A[row_tile][col+29];
                // t30 = A[row_tile][col+30];
                // t31 = A[row_tile][col+31];

                B[col+0][row_tile] = t0;
                B[col+1][row_tile] = t1;
                B[col+2][row_tile] = t2;
                B[col+3][row_tile] = t3;
                B[col+4][row_tile] = t4;
                B[col+5][row_tile] = t5;
                B[col+6][row_tile] = t6;
                B[col+7][row_tile] = t7;
                // B[col+8][row_tile] = t8;
                // B[col+9][row_tile] = t9;
                // B[col+10][row_tile] = t10;
                // B[col+11][row_tile] = t11;
                // B[col+12][row_tile] = t12;
                // B[col+13][row_tile] = t13;
                // B[col+14][row_tile] = t14;
                // B[col+15][row_tile] = t15;
                // B[col+16][row_tile] = t16;
                // B[col+17][row_tile] = t17;
                // B[col+18][row_tile] = t18;
                // B[col+19][row_tile] = t19;
                // B[col+20][row_tile] = t20;
                // B[col+21][row_tile] = t21;
                // B[col+22][row_tile] = t22;
                // B[col+23][row_tile] = t23;
                // B[col+24][row_tile] = t24;
                // B[col+25][row_tile] = t25;
                // B[col+26][row_tile] = t26;
                // B[col+27][row_tile] = t27;
                // B[col+28][row_tile] = t28;
                // B[col+29][row_tile] = t29;
                // B[col+30][row_tile] = t30;
                // B[col+31][row_tile] = t31;

                // for (col_tile = col; col_tile < col+32; col_tile += 1) {
                //     tmp[col_tile-col] = A[row_tile][col_tile];
                //     B[col_tile][row_tile] = tmp[col_tile-col];
                // }
            }
        }
    }

    // for (int i = 0; i < N; i += 8) {
    //     for (int j = 0; j < M; j += 8) {
    //         for (int k = i; k < i + 8; k++) {
    //             int t0 = A[k][j];
    //             int t1 = A[k][j + 1];
    //             int t2 = A[k][j + 2];
    //             int t3 = A[k][j + 3];
    //             int t4 = A[k][j + 4];
    //             int t5 = A[k][j + 5];
    //             int t6 = A[k][j + 6];
    //             int t7 = A[k][j + 7];

    //             B[j][k] = t0;
    //             B[j + 1][k] = t1;
    //             B[j + 2][k] = t2;
    //             B[j + 3][k] = t3;
    //             B[j + 4][k] = t4;
    //             B[j + 5][k] = t5;
    //             B[j + 6][k] = t6;
    //             B[j + 7][k] = t7;
    //         }
    //     }
    // }


}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

