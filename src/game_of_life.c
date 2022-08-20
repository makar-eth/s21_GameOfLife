// Copyright 2022 <valeryje><brighton><haileyan>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#define height 25
#define width 80

void makeField(int **a);
void next_move(int **point, int **arr_temp);
void side_move(int **point, int **arr_temp);
void array_initialize(int ***a);
int check(int **a, int sum, int i, int j);
void reading(int **a);
void redistricting(int **a, int **a_tmp, int *flag);
void delay(int number_of_seconds, int speed);
int population(int **arr);
void cleaner(int **arr);
void info_print(int *k, int **arr);
void input_speed(int *v);
int byte_counter();

int main() {
    int **a = NULL, **arr_temp = 0, k = 0, flag = 0, speed = 0;
    array_initialize(&a);
    array_initialize(&arr_temp);
    reading(a);
//    int *p = freopen("/dev/tty", "r", stdin);
//    *p = *p + 1;
    if (freopen("/dev/tty", "r", stdin) != NULL) {
        printf("Введите скоростной режим (1-5): ");
        scanf("%d", &speed);
        if (speed > 5) {
            speed = 5;
        } else if (speed < 1) {
            speed = 0;
        }
        while (flag !=2) {
            if (flag == 1) {
                flag = 2;
            }
            makeField(a);
            input_speed(&speed);
            next_move(a, arr_temp);
            side_move(a, arr_temp);
            redistricting(a, arr_temp, &flag);
            info_print(&k, a);
            delay(1, speed);
        }
        printf("Игра закончена\n");
        cleaner(a);
        cleaner(arr_temp);
    }
    return 0;
}

void makeField(int **a) {
    printf("\033[0d\033[2J");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (a[i][j] == 0) {
                printf("_");
            } else {
                printf("*");
             }
        }
        printf("\n");
    }
}

int population(int **arr) {
    int sum = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            sum += arr[i][j];
        }
    }
    return sum;
}

int byte_counter() {
    int read_bytes;
    ioctl(0, FIONREAD, &read_bytes);
    return read_bytes;
}

void input_speed(int *v) {
    system("/bin/stty raw");
    if (byte_counter()) {
        char c = getchar();
        if (c == '1') {
            *v = 1;
        }
        if (c  == '2') {
            *v = 2;
        }
        if (c == '3') {
            *v = 3;
        }
        if (c == '4') {
            *v = 4;
        }
        if (c == '5') {
            *v = 5;
        }
    }
    system("/bin/stty cooked");
}

void cleaner(int **arr) {
    for (int i = 0; i < height; i++) {
        free(arr[i]);
    }
    free(arr);
}

void next_move(int **point, int **arr_temp) {
    int sum;
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            sum = point[i-1][j-1] + point[i-1][j] + \
            point[i-1][j+1] + point[i][j-1] + point[i][j+1] + \
            point[i+1][j-1] + point[i+1][j] + point[i+1][j+1];
            arr_temp[i][j] = check(point, sum, i, j);
        }
    }
}

void side_move(int **point, int **arr_temp) {
    int sum;
    for (int i = 0; i < height; i = i + height - 1) {
        for (int j = 0; j < width; j = j + width - 1) {
            sum = point[i][abs(j-1)] + point[abs(i-1)][abs(j-1)] + \
            point[abs(i-1)][j] + point[i][abs(j-(width-1))] + \
            point[abs(i-1)][abs(j-(width-1))] + point[abs(i-(height-1))][j] + \
            point[abs(i-(height-1))][abs(j-1)] + point[abs(i-(height-1))][abs(j-(width-1))];
            arr_temp[i][j] = check(point, sum, i, j);
        }
    }
    for (int i = 1; i < height - 1; i++) {
        for (int j = 0; j < width; j = j + width - 1) {
            sum = point[i-1][j] + point[i-1][abs(j-1)] + point[i][abs(j-1)] + \
            point[i+1][abs(j-1)] + point[i+1][j] + point[i-1][abs(j-(width-1))] + \
            point[i][abs(j-(width-1))] + point[i+1][abs(j-(width-1))];
            arr_temp[i][j] = check(point, sum, i, j);
        }
    }
    for (int i = 0; i < height; i = i + height - 1) {
        for (int j = 1; j < width - 1; j++) {
            sum = point[i][j-1] + point[abs(i-1)][j-1] + point[abs(i-1)][j] + \
            point[abs(i-1)][j+1] + point[i][j+1] + point[abs(i-(height-1))][j-1] + \
            point[abs(i-(height-1))][j] + point[abs(i-(height-1))][j+1];
            arr_temp[i][j] = check(point, sum, i, j);
        }
    }
}

void array_initialize(int ***a) {
    *a = calloc(height, sizeof(int*));
    for (int i = 0; i < height; i++) {
        *(*a + i) = calloc(width, sizeof(int));
    }
}

void reading(int **a) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            scanf("%d", &a[i][j]);
        }
    }
}

int check(int **a, int sum, int i, int j) {
    int res = 0;
    if (a[i][j] == 1 && (sum == 3 || sum == 2)) {
        res = 1;
    } else if (a[i][j] == 0 && sum == 3) {
        res = 1;
    }
    return res;
}

void redistricting(int **a, int **a_tmp, int *flag) {
    if (*flag != 2) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (a[i][j] == a_tmp[i][j]) {
                    *flag = *flag + 1;
                }
                a[i][j] = a_tmp[i][j];
            }
        }
        if (*flag == height * width) {
            *flag = 1;
        } else {
            *flag = 0;
        }
    }
}

void delay(int number_of_seconds, int speed) {
    int milli_seconds = (110000 - 15000 * speed) * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds) {}
}

void info_print(int *k, int **arr) {
    *k = *k + 1;
    printf("Ход: %d\n", *k);
    printf("Популяция: %d\n", population(arr));
}
