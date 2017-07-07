/*
 ============================================================================
 Name        : 3.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//��� ����� ������� � ����� �����: https://code-live.ru/post/cpp-life-game/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// ��������� ��������� ��� �������� ��������� ������
struct point {
    unsigned is_live:1;
};

/* ������ �������� ���� */
#define __WORLD_HEIGHT__ 5

/* ������ �������� ���� */
#define __WORLD_WIDTH__ 5

// ������� ���� �������� 5x5 ������
struct point world[__WORLD_WIDTH__][__WORLD_HEIGHT__];

/*
 * ������������� ������� ��������� ���� ���������������� ����������
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
 * ���������� ����� ������ �� ������� ����
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
 * ��������� ��������� ������� ����� (����������� ���� 1 �������)
*/
void read_point_neighbors(signed int nb[][2], unsigned int x, unsigned int y)
{
    unsigned int i, j;
    unsigned int k = 0;
    for (i = x - 1; i <= x + 1; i++) {
        for (j = y - 1; j <= y + 1; j++) {
            if (i == x && j == y) { //���� ������ � �������� �����, ����������
                continue;
            }
            nb[k][0] = i;
            nb[k][1] = j;
            k++;
        }
    }
}

/*
 * ���������� ����� ������� � ������ � ������������ x, y
 */
unsigned int count_live_neighbors(struct point world[][__WORLD_HEIGHT__], unsigned int x, unsigned int y)
{
    unsigned int count = 0;
    unsigned int i;
    signed int nb[8][2]; //������ ��������� 8-�� �������� �����
    signed int _x, _y;

    read_point_neighbors(nb, x, y); //�������� ���������� �������� �����

    for (i = 0; i < 8; i++) {
        _x = nb[i][0];
        _y = nb[i][1];

        if (_x < 0 || _y < 0) { //�������� �� ����� �� ������� �������� ���� ������ � �����
            continue;
        }
        if (_x >= __WORLD_WIDTH__ || _y >= __WORLD_HEIGHT__) { //�������� ����� � ������
            continue;
        }

        if (world[_x][_y].is_live == 1) {
            count++;
        }
    }

    return count;
}

/*
 * ������������� ��������� ��������� �������� ����
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
 * ����������� �������� ����. ������������ ��� ���������� ����������� ���������
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
 * ��������� ������� ����� �������� � ����������� ���������
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
 * ������� �� ����� ������� ����
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

    do { //���� ���� �� ��� ���, ���� �� ���� �� ��������� ����� ������ ��� �� ����� ��������� �������
        print_world(world); //������� ������� ������� ����
        copy_world(world, prev_world); //��������
        next_generation(world, prev_world); //���������� ��������� ��������� ������

        is_optimal = cmp_world(world, prev_world) == 0; //���������, ��������� �� �������
        live_points = get_live_count(world); //������� ���������� ����� ������

        int i = 0;
        for (i = 0; i < __WORLD_WIDTH__; i++)
        	printf("//");
        printf("\n");

        if (is_optimal) //���� �������
        	printf("���������� ����������� ������������\n");

        if (live_points == 0) //���� ��� ������ ������
        	printf("��� ������ ������\n");
    } while (live_points != 0 && !is_optimal);
	return EXIT_SUCCESS;
}
