#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_file(char *filename, int **map, int n);
void depth_first_search();
void result_analyze();

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int height;
    int max_branch;
    int leaf;
} Results;

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

// main関数やload_file関数はそのまま

// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
// この関数を丸ごと入れ替える
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void depth_first_search(int n, int **map)
{
    printf("\n深さ優先探索を開始します (開始ノード: 0)...\n");

    // --- 必要なデータ構造を動的に確保 ---
    int* stack = (int*)malloc(n * sizeof(int));      // スタック (ノード番号を格納)
    int* visited = (int*)calloc(n, sizeof(int));     // 訪問記録 (callocで0に初期化)
    int* parent = (int*)malloc(n * sizeof(int));     // 親ノード記録 (全域木)
    
    if (stack == NULL || visited == NULL || parent == NULL) {
        fprintf(stderr, "エラー: DFS用のメモリ確保に失敗しました。\n");
        exit(1);
    }
    
    // parent配列を-1で初期化 (-1は親がいないことを示す)
    for (int i = 0; i < n; i++) {
        parent[i] = -1;
    }

    int top = -1; // スタックの頂上を-1で初期化 (空の状態)

    // --- 探索開始 (始点はノード0とする) ---
    int start_node = 0;
    
    // 1. 始点をスタックに積む (プッシュ)
    stack[++top] = start_node;
    // 2. 始点を訪問済みにする
    visited[start_node] = 1;

    // 3. スタックが空になるまでループ
    while (top != -1) {
        
        // 4. スタックから現在地を取り出す (ポップ)
        int u = stack[top--];
        printf("%d\n", u);

        // 5. 現在地(u)から行ける、未訪問の隣接ノード(v)を探す
        //    課題の指示通り「番号の若い順」に探すため、vを0から昇順でループ
        for (int v = 0; v < n; v++) {
            // uからvへの道(map[u][v]==1)があり、かつvが未訪問(visited[v]==0)の場合
            if (map[u][v] == 1 && visited[v] == 0) {
                // 6. vを訪問済みにする
                visited[v] = 1;
                // 7. vの親はuであると記録する
                parent[v] = u;
                // 8. vをスタックに積む (プッシュ)
                stack[++top] = v;
            }
        }
    }
    printf("探索が完了しました。\n\n");
    
    // --- 全域木の解析 ---
    // analyze_tree(n, parent); // ←後でこの関数を実装する

    // --- 確保したメモリを解放 ---
    free(stack);
    free(visited);
    free(parent);
}

// void result_analyze()
// {

// }

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

    depth_first_search(n, map);

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
