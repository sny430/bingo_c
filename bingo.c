/* Report 11 (Matsu) BINGO GAME */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 5     /* 縦横の大きさ */
#define M 15	/* 同じ列内の数の幅 */
#define HOLE (-1)	/* 穴 */

/* numset(): ダブりなく x[][] に乱数を入れ中央に穴を空ける */
void numset(int x[][N]) {

    int i, j, check[M][N];
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            check[i][j] = 0;
        }
    }

    /* 乱数行列を生成 */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            x[i][j] = -1;
            while (x[i][j] == -1) {
                int r = rand() % M;
                if (check[r][i] == 0) {
                    x[i][j] = M*i + r + 1;
                    check[r][i] = 1;
                }
            }
        }
    }

    /* 盤面の幅が奇数ならば中央に穴を空ける */
    if (N % 2 == 1) {
        x[N/2][N/2] = HOLE;
    }

}

/* search(): x[*pi][*pj] = num になる (*pi, *pj) を探す．その有無も *pfound に入れて返す */
void search(int num, int x[][N], int *pfound, int *pi, int *pj) {

    int i, j;
    *pfound = 0;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (num == x[i][j]) {
                *pfound = 1;
                *pi = i;
                *pj = j;
            }
        }
    }

}

/* check(): x[inum][jnum] でゲームが継続するかを判定し，継続か否かを
	    *pcont に入れて返す */
void check(int x[][N], int inum, int jnum, int *pcont) {

    int i, j, flag;

    /* 縦で揃っているかを判定 */
    flag = 1;
    for (i = 0; i < N; i++) {
        if (x[i][jnum] != HOLE) {
            flag = 0;
        }
    }
    if (flag) {
        *pcont = 0;
    }

    /* 横で揃っているかを判定 */
    flag = 1;
    for (j = 0; j < N; j++) {
        if (x[inum][j] != HOLE) {
            flag = 0;
        }
    }
    if (flag) {
        *pcont = 0;
    }

    /* 左上~右下で揃っているかを判定 */
    if (inum == jnum) {
        flag = 1;
        for (i = 0; i < N; i++) {
            if (x[i][i] != HOLE) {
                flag = 0;
            }
        }
        if (flag) {
            *pcont = 0;
        }
    }

    /* 右上~左下で揃っているかを判定 */
    if (inum + jnum == N-1) {
        flag = 1;
        for (i = 0; i < N; i++) {
            if (x[i][N-1 - i] != HOLE) {
                flag = 0;
            }
        }
        if (flag) {
            *pcont = 0;
        }
    }

}

/* numout(): 現在の x[][] の状態を出力する */
void numout(int x[][N]) {
    printf("--------------\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (x[i][j] == HOLE) {
                printf("<> ");
            } else {
                printf("%2d ", x[i][j]);
            }
        }
        printf("\n");
    }
    printf("--------------\n");
}

/* メインプログラム */
int main(void) {
    
    srand(time(NULL));

    int i, j;
    int num;    /* ビンゴマシンから出た番号 */
    int found;   /* 盤面に num があるかを示す(1：あり; 0: なし) */
    int cont;   /* BINGO になっていないかを示す(1：未達; 0: BINGO) */
    int board[N][N];    /* 盤面 */
    
    /* 最初に numset() で最初の盤面にし，numout() で出力する */
    numset(board);
    numout(board);

    cont = 1;
    while (cont == 1) {

        /* (1) 1 <= n <= 75 である num を scanf で入力する */
        printf("num = ");
        scanf("%d", &num);

        /* (2) search() で 盤面に num があるか調べる */
        search(num, board, &found, &i, &j);

        /* (3) (2) で見つかったらその場所に穴を開け，
               check() で BINGO になったか調べ，
               BINGO になったら cont を 0 にする(while 文を抜ける) */
        if (found) {
            board[i][j] = HOLE;
            check(board, i, j, &cont);
            printf("Board changed!\n");
        } else {
            printf("No number in board!\n");
        }
    
        /* (4) 毎回 numout() で変化した盤面を表示する */
        numout(board);

    }

    /* 終了表示 */
    printf("\e[?25l");
    for (i = 0; i < 30; i++) {
        printf("\x1b[38;5;%dm", rand() % 256);
        printf("****BINGO!****\n");
        time_t now = clock() + CLOCKS_PER_SEC / 10;
        while (now > clock());
        printf("\x1b[1A");
    }
    printf("\x1b[39m\e[?25h\n");

    return 0;

}