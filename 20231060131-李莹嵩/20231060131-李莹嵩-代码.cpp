#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

typedef struct {
    int id;
    int weight;
    double value;
    double ratio;
} Item;

// 生成随机物品
void generate_items(Item items[], int n) {
    for (int i = 0; i < n; i++) {
        items[i].id = i + 1;
        items[i].weight = rand() % 100 + 1;
        items[i].value = (rand() % 90001) / 100.0 + 100;
        items[i].ratio = items[i].value / items[i].weight;
    }
}

// 获取当前时间（毫秒）
long long current_time_ms() {
#ifdef _WIN32
    // Windows 实现
    FILETIME ft;
    ULARGE_INTEGER time_val;
    GetSystemTimeAsFileTime(&ft);
    time_val.LowPart = ft.dwLowDateTime;
    time_val.HighPart = ft.dwHighDateTime;
    return time_val.QuadPart / 10000 - 11644473600000LL;
#else
    // Linux/macOS 实现
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

// 动态规划法求解
double dp(Item items[], int n, int capacity, bool selected[], double* exec_time) {
    long long start_time = current_time_ms();

    int total_value = 0;
    for (int i = 0; i < n; i++) {
        total_value += (int)(items[i].value * 100);
    }

    int* dp = (int*)calloc(capacity + 1, sizeof(int));
    bool** path = (bool**)malloc((n + 1) * sizeof(bool*));
    for (int i = 0; i <= n; i++) {
        path[i] = (bool*)calloc(capacity + 1, sizeof(bool));
    }

    for (int i = 1; i <= n; i++) {
        int weight = items[i - 1].weight;
        int value = (int)(items[i - 1].value * 100);
        for (int j = capacity; j >= weight; j--) {
            if (dp[j] < dp[j - weight] + value) {
                dp[j] = dp[j - weight] + value;
                path[i][j] = true;
            }
        }
    }

    double max_value = dp[capacity] / 100.0;
    int j = capacity;
    for (int i = n; i > 0; i--) {
        if (path[i][j]) {
            selected[i - 1] = true;
            j -= items[i - 1].weight;
        }
    }

    free(dp);
    for (int i = 0; i <= n; i++) free(path[i]);
    free(path);

    long long end_time = current_time_ms();
    *exec_time = (double)(end_time - start_time);
    return max_value;
}

// 比较函数（按价值重量比降序）
int compare_ratio(const void* a, const void* b) {
    Item* item1 = (Item*)a;
    Item* item2 = (Item*)b;
    if (item1->ratio < item2->ratio) return 1;
    if (item1->ratio > item2->ratio) return -1;
    return 0;
}

// 贪心法求解
double greedy(Item items[], int n, int capacity, bool selected[], double* exec_time) {
    long long start_time = current_time_ms();

    Item* sorted_items = (Item*)malloc(n * sizeof(Item));
    memcpy(sorted_items, items, n * sizeof(Item));
    qsort(sorted_items, n, sizeof(Item), compare_ratio);

    double total_value = 0;
    int current_weight = 0;
    memset(selected, 0, n * sizeof(bool));

    for (int i = 0; i < n; i++) {
        if (current_weight + sorted_items[i].weight <= capacity) {
            selected[sorted_items[i].id - 1] = true;
            current_weight += sorted_items[i].weight;
            total_value += sorted_items[i].value;
        }
    }

    free(sorted_items);

    long long end_time = current_time_ms();
    *exec_time = (double)(end_time - start_time);
    return total_value;
}

// 回溯法辅助函数
void backtrack(Item items[], int n, int capacity, int depth,
    int current_weight, double current_value,
    double* best_value, bool current_selected[],
    bool best_selected[], double* bound, long* node_count) {
    (*node_count)++;

    if (depth == n) {
        if (current_value > *best_value) {
            *best_value = current_value;
            memcpy(best_selected, current_selected, n * sizeof(bool));
        }
        return;
    }

    // 剪枝：检查是否可能超过最优解
    if (current_value + bound[depth] <= *best_value) {
        return;
    }

    // 不选择当前物品
    backtrack(items, n, capacity, depth + 1,
        current_weight, current_value,
        best_value, current_selected, best_selected, bound, node_count);

    // 选择当前物品（如果不超过容量）
    if (current_weight + items[depth].weight <= capacity) {
        current_selected[depth] = true;
        backtrack(items, n, capacity, depth + 1,
            current_weight + items[depth].weight,
            current_value + items[depth].value,
            best_value, current_selected, best_selected, bound, node_count);
        current_selected[depth] = false;
    }
}

// 回溯法求解
double backtracking(Item items[], int n, int capacity, bool selected[], double* exec_time, long* node_count) {
    long long start_time = current_time_ms();

    double best_value = 0;
    bool* current_selected = (bool*)calloc(n, sizeof(bool));
    bool* best_selected = (bool*)calloc(n, sizeof(bool));
    double* bound = (double*)malloc(n * sizeof(double));

    if (!current_selected || !best_selected || !bound) {
        printf("内存分配失败\n");
        exit(1);
    }

    // 计算上界（剩余物品的最大可能价值）
    double total_remaining = 0;
    for (int i = n - 1; i >= 0; i--) {
        total_remaining += items[i].value;
        bound[i] = total_remaining;
    }

    *node_count = 0;
    backtrack(items, n, capacity, 0, 0, 0,
        &best_value, current_selected, best_selected, bound, node_count);

    memcpy(selected, best_selected, n * sizeof(bool));

    free(current_selected);
    free(best_selected);
    free(bound);

    long long end_time = current_time_ms();
    *exec_time = (double)(end_time - start_time);
    return best_value;
}

// 蛮力法求解
double brute_force(Item items[], int n, int capacity, bool selected[], double* exec_time, unsigned long long* state_count) {
    long long start_time = current_time_ms();

    double max_value = 0;
    unsigned long long total = (n < 64) ? (1ULL << n) : 0;
    *state_count = 0;

    if (total == 0) {
        long long end_time = current_time_ms();
        *exec_time = (double)(end_time - start_time);
        return 0;
    }

    bool* temp_selected = (bool*)calloc(n, sizeof(bool));
    bool* best_selected = (bool*)calloc(n, sizeof(bool));

    for (unsigned long long i = 0; i < total; i++) {
        (*state_count)++;
        int current_weight = 0;
        double current_value = 0;
        memset(temp_selected, 0, n * sizeof(bool));

        for (int j = 0; j < n; j++) {
            if (i & (1ULL << j)) {
                if (current_weight + items[j].weight > capacity) {
                    current_value = -1;
                    break;
                }
                current_weight += items[j].weight;
                current_value += items[j].value;
                temp_selected[j] = true;
            }
        }

        if (current_value > max_value) {
            max_value = current_value;
            memcpy(best_selected, temp_selected, n * sizeof(bool));
        }
    }

    memcpy(selected, best_selected, n * sizeof(bool));
    free(temp_selected);
    free(best_selected);

    long long end_time = current_time_ms();
    *exec_time = (double)(end_time - start_time);
    return max_value;
}

// 打印结果
void print_results(Item items[], bool selected[], int n, int capacity,
    const char* algorithm, double total_value,
    double exec_time, unsigned long long states) {
    printf("\n=== %s 算法结果 (n=%d, C=%d) ===\n", algorithm, n, capacity);
    printf("总价值: %.2f\n", total_value);
    printf("执行时间: %.2f 毫秒\n", exec_time);

    if (states > 0) {
        if (strcmp(algorithm, "回溯法") == 0) {
            printf("搜索节点数: %llu\n", states);
        }
        else if (strcmp(algorithm, "蛮力法") == 0) {
            printf("状态空间大小: %llu\n", states);
        }
    }

    int selected_count = 0;
    for (int i = 0; i < n; i++) {
        if (selected[i]) selected_count++;
    }
    printf("选择的物品数量: %d\n", selected_count);

    // 对于小规模问题，输出所有选择的物品
    if (n <= 100) {
        printf("\n选择的物品清单:\n");
        printf("%-10s %-10s %-10s\n", "ID", "重量", "价值");
        for (int i = 0; i < n; i++) {
            if (selected[i]) {
                printf("%-10d %-10d %-10.2f\n", items[i].id, items[i].weight, items[i].value);
            }
        }
    }
    printf("========================================\n");
}

// 运行指定算法
void run_algorithm(Item items[], int n, int capacity, const char* algorithm) {
    bool* selected = (bool*)calloc(n, sizeof(bool));
    double total_value = 0;
    double exec_time = 0;
    unsigned long long states = 0;
    long node_count = 0;

    if (strcmp(algorithm, "动态规划") == 0) {
        total_value = dp(items, n, capacity, selected, &exec_time);
    }
    else if (strcmp(algorithm, "贪心法") == 0) {
        total_value = greedy(items, n, capacity, selected, &exec_time);
    }
    else if (strcmp(algorithm, "回溯法") == 0) {
        total_value = backtracking(items, n, capacity, selected, &exec_time, &node_count);
        states = node_count;
    }
    else if (strcmp(algorithm, "蛮力法") == 0) {
        total_value = brute_force(items, n, capacity, selected, &exec_time, &states);
    }

    print_results(items, selected, n, capacity, algorithm, total_value, exec_time, states);
    free(selected);
}

int main() {
    srand(time(NULL));

    // 测试配置
    int capacities[] = { 10000, 100000, 1000000 };
    int num_items[] = { 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
                      10000, 20000, 40000, 80000, 160000, 320000 };
    const char* algorithms[] = { "动态规划", "贪心法", "回溯法", "蛮力法" };

    int num_capacities = sizeof(capacities) / sizeof(capacities[0]);
    int num_tests = sizeof(num_items) / sizeof(num_items[0]);
    int num_algorithms = sizeof(algorithms) / sizeof(algorithms[0]);

    // 运行所有测试组合
    for (int cap_idx = 0; cap_idx < num_capacities; cap_idx++) {
        int C = capacities[cap_idx];

        for (int test_idx = 0; test_idx < num_tests; test_idx++) {
            int n = num_items[test_idx];

            // 生成物品
            printf("\n生成 %d 个物品 (容量=%d)...\n", n, C);
            Item* items = (Item*)malloc(n * sizeof(Item));
            generate_items(items, n);

            // 运行所有算法
            for (int algo_idx = 0; algo_idx < num_algorithms; algo_idx++) {
                // 跳过不切实际的组合
                if (strcmp(algorithms[algo_idx], "蛮力法") == 0 && n > 20) {
                    printf("\n跳过蛮力法 (n=%d 过大)\n", n);
                    continue;
                }
                if (strcmp(algorithms[algo_idx], "回溯法") == 0 && n > 30) {
                    printf("\n跳过回溯法 (n=%d 过大)\n", n);
                    continue;
                }
                if (strcmp(algorithms[algo_idx], "动态规划") == 0 && (long)n * C > 500000000) {
                    printf("\n跳过动态规划 (n×C=%lld > 500M)\n", (long long)n * C);
                    continue;
                }

                run_algorithm(items, n, C, algorithms[algo_idx]);
            }

            free(items);
        }
    }

    return 0;
}