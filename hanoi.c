#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//32 == 21.985 total, 33 == 42.581
//

struct tower
{
	unsigned char **tower;
	unsigned char rows;
	unsigned char heights[3];
};

struct tower *fill_tower(char *string);
void hanoi(struct tower *tower, unsigned int col, unsigned int col_target, unsigned int number);
void hanoi_iter(struct tower *tower);
void print_tower(struct tower *tower);
void destroy_tower(struct tower *tower);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Wrong Input\n");
		exit(1);
	}
	struct tower *pointer = fill_tower(argv[1]);
	//hanoi(pointer, 0, 2, pointer->rows);
	hanoi_iter(pointer);
	print_tower(pointer);
	printf("NUMBER: %u\n", pointer->rows);
	destroy_tower(pointer);
	return 0;
}

struct tower *fill_tower(char *string)
{
	unsigned int i, number = strlen((char *) string);
	struct tower *pointer = malloc(sizeof(struct tower));
	pointer->tower = malloc(number * sizeof(char *));

	for (i = 0; i < number; i++)
	{
		pointer->tower[i] = malloc(3 * sizeof(char));
		pointer->tower[i][0] = string[i];
		pointer->tower[i][1] = '0';
		pointer->tower[i][2] = '0';
	}
	pointer->rows = number;
	pointer->heights[0] = number;
	pointer->heights[1] = pointer->heights[2] = 0;
	return pointer;
}

static inline __attribute__((always_inline)) unsigned char get_elem(struct tower *tower, unsigned int col)
{
	if (tower->heights[col] == 0)
		return '0';
	return tower->tower[tower->rows - tower->heights[col]][col];
	//return tower->tower[tower->rows - tower->heights[col] - !(tower->heights[col])][col];
}

static inline __attribute__((always_inline)) void shift_elem(struct tower *tower, unsigned int col, unsigned int col_target)
{
	/*if (tower->heights[col] == 0)
		return;*/
	tower->tower[tower->rows - 1 - tower->heights[col_target]][col_target] = tower->tower[tower->rows - tower->heights[col]][col];

	tower->tower[tower->rows - tower->heights[col]][col] = '0';

	tower->heights[col]--;
	tower->heights[col_target]++;

}

void hanoi(struct tower *tower, unsigned int col, unsigned int col_target, unsigned int number)
{
	if (number == 1) {
		shift_elem(tower, col, col_target);
		return;
	}
	unsigned char buffer = 3 - (col + col_target); //compute 'buffer'-column
	hanoi(tower, col, buffer, number - 1); //move 'tower - 1' to the 'buffer'-column
	shift_elem(tower, col, col_target);
	hanoi(tower, buffer, col_target, number - 1); //move 'tower - 1' from 'buffer'-column to target-column
}

void hanoi_iter(struct tower *tower)
{
	unsigned char direction = 1;
	unsigned char ignore_col = 0;
	unsigned char other_col1;
	unsigned char other_col2;
	unsigned char new_col;

	//if number of elements is odd then direction is "left" else "right"
	if (tower->rows % 2)
		direction = 2;

	while (tower->tower[1][2] == '0') {
		new_col = (ignore_col + direction) % 3;
		shift_elem(tower, ignore_col, new_col);
		ignore_col = new_col;
		other_col1 = (ignore_col + 1) % 3;
		other_col2 = (ignore_col + 2) % 3;

		if (get_elem(tower, other_col1) == '0') {
			shift_elem(tower, other_col2, other_col1);
			continue;
		}
		if (get_elem(tower, other_col2) == '0') {
			shift_elem(tower, other_col1, other_col2);
			continue;
		}
		if (get_elem(tower, other_col1) < get_elem(tower, other_col2)) {
			shift_elem(tower, other_col1, other_col2);
			continue;
		}
		shift_elem(tower, other_col2, other_col1);
	}
}

void print_tower(struct tower *tower)
{
	unsigned int i, number = tower->rows;

	for (i = 0; i < number; i++) {
		printf("%c %c %c\n", tower->tower[i][0], tower->tower[i][1], tower->tower[i][2]);
	}
	printf("\n");
}

void destroy_tower(struct tower *tower)
{
	unsigned int i, size = tower->rows;

	for (i = 0; i < size; i++)
		free(tower->tower[i]);
	free(tower->tower);
	free(tower);
}
