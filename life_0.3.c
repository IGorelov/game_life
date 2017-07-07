/*
 ============================================================================
 Name        : 3.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

// од нагло спизжен с этого сайта: https://code-live.ru/post/cpp-life-game/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// ѕобитова€ структура дл€ хранени€ состо€ние клетки
struct point {
    unsigned is_live:1;
};

/* Ўирина игрового пол€ */
#define __WORLD_HEIGHT__ 5

/* ¬ысота игрового пол€ */
#define __WORLD_WIDTH__ 5

// »гровое поле размером 5x5 клеток
struct point world[__WORLD_WIDTH__][__WORLD_HEIGHT__];

/*
 * »нициализаци€ первого поколени€ игры псевдослучайными значени€ми
 */
void init_world(struct point world[][__WORLD_HEIGHT__])
{
	srand(time(NULL));
	int min_val = 1, max_val = 10000;

    unsigned int i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            unsigned int num = rand() % (max_val - min_val + 1) + min_val;
            if (num % 2 == 0) {
                world[i][j].is_live = 1;
            } else {
                world[i][j].is_live = 0;
            }
        }
    }
}

/*
 *  оличество живых клеток на игровом поле
*/
unsigned int get_live_count(struct point world[][__WORLD_HEIGHT__])
{
    unsigned int count = 0;
    unsigned i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            if (world[i][j].is_live == 1) {
                count++;
            }
        }
    }
    return count;
}

/*
 * ѕолучение координат соседей точки (окрестность мура 1 пор€дка)
*/
void read_point_neighbors(signed int nb[][2], unsigned int x, unsigned int y)
{
    unsigned int i, j;
    unsigned int k = 0;
    for (i = x - 1; i <= x + 1; i++) {
        for (j = y - 1; j <= y + 1; j++) {
            if (i == x && j == y) { //если попали в заданную точку, продолжаем
                continue;
            }
            nb[k][0] = i;
            nb[k][1] = j;
            k++;
        }
    }
}

/*
 *  оличество живых соседей у клетки с координатами x, y
 */
unsigned int count_live_neighbors(struct point world[][__WORLD_HEIGHT__], unsigned int x, unsigned int y)
{
    unsigned int count = 0;
    unsigned int i;
    signed int nb[8][2]; //массив координат 8-ми соседних точек
    signed int _x, _y;

    read_point_neighbors(nb, x, y); //получаем координаты соседних точек

    for (i = 0; i < 8; i++) {
        _x = nb[i][0];
        _y = nb[i][1];

        if (_x < 0 || _y < 0) { //проверка на выход из границы игрового пол€ сверху и слева
            continue;
        }
        if (_x >= __WORLD_WIDTH__ || _y >= __WORLD_HEIGHT__) { //проверка снизу и справа
            continue;
        }

        if (world[_x][_y].is_live == 1) {
            count++;
        }
    }

    return count;
}

/*
 * —генерировать следующее поколение игрового мира
 */
void next_generation(struct point world[][__WORLD_HEIGHT__], struct point prev_world[][__WORLD_HEIGHT__])
{
    unsigned int i, j;
    unsigned int live_nb;
    struct point p;

    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            p = prev_world[i][j];
            live_nb = count_live_neighbors(prev_world, i, j);

            if (p.is_live == 0) {
                if (live_nb == 3) {
                    world[i][j].is_live = 1;
                }
            } else {
                if (live_nb < 2 || live_nb > 3) {
                    world[i][j].is_live = 0;
                }
            }
        }
    }
}

/*
 *  опирование игрового мира. »спользуетс€ дл€ сохранени€ предыдущего поколени€
*/
void copy_world(struct point src[][__WORLD_HEIGHT__], struct point dest[][__WORLD_HEIGHT__])
{
    unsigned int i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            dest[i][j] = src[i][j];
        }
    }
}


/*
 * —равнение игровых миров текущего и предыдущего поколени€
 */
int cmp_world(struct point w1[][__WORLD_HEIGHT__], struct point w2[][__WORLD_HEIGHT__])
{
    unsigned int i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            if (w1[i][j].is_live != w2[i][j].is_live) {
                return -1;
            }
        }
    }
    return 0;
}

/*
 * ¬ывести на экран игровое поле
*/
void print_world(struct point world[][__WORLD_HEIGHT__])
{
    unsigned int i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            if (world[i][j].is_live == 1)
            	printf("*");
            else
            	printf("-");
            printf(" ");
        }
        printf("\n");
    }
}

int main(void) {
    struct point world[__WORLD_WIDTH__][__WORLD_HEIGHT__];
    struct point prev_world[__WORLD_WIDTH__][__WORLD_HEIGHT__];

    init_world(world);
    unsigned int live_points = -1;
    bool is_optimal = false;

    do { //цикл идет до тех пор, пока на поле не останетс€ живых клеток или не будет достигнут оптимум
        print_world(world); //выводим текущее игровое поле
        copy_world(world, prev_world); //копируем
        next_generation(world, prev_world); //генерируем следующее поколение клеток

        is_optimal = cmp_world(world, prev_world) == 0; //провер€ем, достигнут ли оптимум
        live_points = get_live_count(world); //считаем количество живых клеток

        int i = 0;
        for (i = 0; i < __WORLD_WIDTH__; i++)
        	printf("//");
        printf("\n");

        if (is_optimal) //если оптимум
        	printf("ƒостигнута оптимальна€ конфигураци€\n");

        if (live_points == 0) //если все клетки мертвы
        	printf("¬се клетки мертвы\n");
    } while (live_points != 0 && !is_optimal);
	return EXIT_SUCCESS;
}
