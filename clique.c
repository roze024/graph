#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK 10000

typedef struct {
    int row;
    int col;
} Point;

void load_file (char *filename, int **map, int n)
{
    FILE *fp;
    int row = 0, col = 0;
    int chara;

    fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("エラー: ファイルを開けません"); // より詳細なエラー理由を表示
        exit(EXIT_FAILURE);
    }

    while ((chara = fgetc(fp)) != EOF) {
        if(chara == '0' || chara == '1'){
            map[row][col] = chara - '0';;
            col++;

            if (col == n) {
                col = 0;
                row++;
            }
        }
    }

    fclose(fp);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("使い方: <ファイル名> <次元数>\n");
        return 1;
    }

    char *filename = argv[1];
    int n = atoi(argv[2]);
    int **map;

    // 「行」のポインタ配列をヒープに確保
    map = (int **)malloc(n * sizeof(int *));
    if (map == NULL) {
        perror("メモリ確保に失敗 (行)");
        return 1;
    }

    // 各行について、「列」の配列をヒープに確保
    for (int i = 0; i < n; i++) {
        map[i] = (int *)malloc(n * sizeof(int));
        if (map[i] == NULL) {
            perror("メモリ確保に失敗 (列)");
            // エラー時には確保済みのメモリを解放する
            for (int j = 0; j < i; j++) {
                free(map[j]);
            }
            free(map);
            return 1;
        }
    }

    load_file(filename, map, n);

    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         printf("%d ", map[i][j]);
    //     }
    //     printf("\n");
    // }
    // ちゃんと読み込めているか確認用のプリントコード


    for (int i = 0; i < n; i++) {
        free(map[i]); // 各行を解放
    }
    free(map); // 行ポインタの配列を解放

    return 0;
}