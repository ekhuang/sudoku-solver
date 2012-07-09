#include <stdio.h>
#include <stdlib.h>

typedef struct Cell {
	int value;
	int possible[9];
} Cell;

typedef struct Puzzle {
	Cell cells[9][9];
} Puzzle;

typedef struct StackNode {
	struct StackNode *next;
	struct Puzzle puzzle;
} StackNode;

void StackPush(StackNode **head, Puzzle puzzle) {
	StackNode *node = (StackNode *)malloc(sizeof(StackNode));
	
	if (node == NULL) {
		fputs("Error: no space available for node\n", stderr);
		abort();
	}
	else {
		node->puzzle = puzzle;
		node->next = emptyStack(*head) ? NULL : *head;
		*head = node;
	}
}

Puzzle StackPop(StackNode **head) {
	if (emptyStack(*head)) {
		fputs("Error: stack underflow\n", stderr);
		abort();
	}
	else {
		StackNode *top = *head;
		Puzzle puzzle = top->puzzle;
		*head = top->next;
		free(top);
		return puzzle;
	}
}

int emptyStack(StackNode *head) {
	return (head == NULL);
}

void initialize(Puzzle *puzzle) {
	int i, j, k;
	for (i = 0; i < 9; i ++) {
		for (j = 0; j < 9; j++) {
			for (k = 0; k < 9; k++) {
				puzzle->cells[i][j].possible[k] = k+1;
			}
		}
	}
}

void read_in(Puzzle *puzzle, char *data) {
	int i, j, k;
	char c[2] = "0";
	FILE *fp;
	if (fp = fopen(data, "r")) {
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				c[0] = fgetc(fp);
				if (c[0] != EOF) {
					if (c[0] == '.') {
						puzzle->cells[i][j].value = 0;
					}
					else {
						puzzle->cells[i][j].value = atoi(c);
						for (k = 0; k < 9; k++) {
							if (puzzle->cells[i][j].value != k + 1) {	
								puzzle->cells[i][j].possible[k] = 0;
							}
						}
					}
				}
			}
		}
		fclose(fp);
	}
	else {
		fputs("Error: invalid specified file", stderr);
		abort();
	}
}

void print(Puzzle *puzzle, int printPossibles) {
	int i, j, k;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			printf("%d", puzzle->cells[i][j].value);
				if (printPossibles == 1) {
					printf("[");
					for (k = 0; k < 9; k++) {
					printf("%d", puzzle->cells[i][j].possible[k]);
					}
					printf("]");
				}
			printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

void simplify(Puzzle *puzzle) {
	int i, j;
	int ii, jj;
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				if (puzzle->cells[i][j].value != 0) {
					for (ii = 0; ii < 9; ii++) {
						if (ii != i) {
							puzzle->cells[ii][j].possible[puzzle->cells[i][j].value - 1] = 0;
						}
					}
					for (jj = 0; jj < 9; jj++) {
						if (jj != j) {
							puzzle->cells[i][jj].possible[puzzle->cells[i][j].value - 1] = 0;
						}
					}
					for (ii = 0; ii < 9; ii++) {
						for (jj = 0; jj < 9; jj++) {
							if ((ii != i) && (jj != j) && (isInQuadrant(i, j, ii, jj))){
								puzzle->cells[ii][jj].possible[puzzle->cells[i][j].value - 1] = 0;								
							}
						}
					}
				}
			}
		}
}

int isInQuadrant(int x, int y, int u, int v) {				//this could be much much shorter, in the same way that i can be less less lazier
	if (	(x >= 0) && (x <= 2) && (y >= 0) && (y <= 2) &&
		(u >= 0) && (u <= 2) && (v >= 0) && (v <= 2))
	{

		return 1;
	}
	if (	(x >= 3) && (x <= 5) && (y >= 0) && (y <= 2) &&
		(u >= 3) && (u <= 5) && (v >= 0) && (v <= 2))
	{

		return 1;
	}
	if (	(x >= 6) && (x <= 8) && (y >= 0) && (y <= 2) &&
		(u >= 6) && (u <= 8) && (v >= 0) && (v <= 2))
	{

		return 1;
	}

	if (	(x >= 0) && (x <= 2) && (y >= 3) && (y <= 5) &&
		(u >= 0) && (u <= 2) && (v >= 3) && (v <= 5))
	{

		return 1;
	}
	if (	(x >= 3) && (x <= 5) && (y >= 3) && (y <= 5) &&
		(u >= 3) && (u <= 5) && (v >= 3) && (v <= 5))
	{

		return 1;
	}
	if (	(x >= 6) && (x <= 8) && (y >= 3) && (y <= 5) &&
		(u >= 6) && (u <= 8) && (v >= 3) && (v <= 5))
	{

		return 1;
	}

	if (	(x >= 0) && (x <= 2) && (y >= 6) && (y <= 8) &&
		(u >= 0) && (u <= 2) && (v >= 6) && (v <= 8))
	{

		return 1;
	}
	if (	(x >= 3) && (x <= 5) && (y >= 6) && (y <= 8) &&
		(u >= 3) && (u <= 5) && (v >= 6) && (v <= 8))
	{

		return 1;
	}
	if (	(x >= 6) && (x <= 8) && (y >= 6) && (y <= 8) &&
		(u >= 6) && (u <= 8) && (v >= 6) && (v <= 8))
	{

		return 1;
	}

	return 0;
}

void finalize(Puzzle *puzzle) {
	int i, j, k;
	int values, final;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			values = 0;
			for (k = 0; k < 9; k++) {
				if (puzzle->cells[i][j].possible[k] != 0) {
					values++;
					final = puzzle->cells[i][j].possible[k];
				}
			}
			if (values == 1) {
				puzzle->cells[i][j].value = final;
			}
		}
	}
}

int puzzleChanged(Puzzle *current, Puzzle *previous) {
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (current->cells[i][j].value != previous->cells[i][j].value) {
				return 1;
			} 
		}
	}
	return 0;
}

int puzzleFilled(Puzzle *puzzle) {
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (puzzle->cells[i][j].value == 0) {
				return 0;
			}
		}
	}
	return 1;
}

int puzzleCorrect(Puzzle *puzzle){	//not very robust, doesn't check quadrants
	return (
		(rowSum(puzzle, 0) == 45) &&
		(rowSum(puzzle, 1) == 45) &&
		(rowSum(puzzle, 2) == 45) &&
		(rowSum(puzzle, 3) == 45) &&
		(rowSum(puzzle, 4) == 45) &&
		(rowSum(puzzle, 5) == 45) &&
		(rowSum(puzzle, 6) == 45) &&
		(rowSum(puzzle, 7) == 45) &&
		(rowSum(puzzle, 8) == 45) &&
		(colSum(puzzle, 0) == 45) &&
		(colSum(puzzle, 1) == 45) &&
		(colSum(puzzle, 2) == 45) &&
		(colSum(puzzle, 3) == 45) &&
		(colSum(puzzle, 4) == 45) &&
		(colSum(puzzle, 5) == 45) &&
		(colSum(puzzle, 6) == 45) &&
		(colSum(puzzle, 7) == 45) &&
		(colSum(puzzle, 8) == 45)
	);
}

int rowSum(Puzzle *puzzle, int row) {
	int i, sum = 0;
	for (i = 0; i < 9; i++) {
		sum += puzzle->cells[i][row].value;
	}
	return sum;
}

int colSum(Puzzle *puzzle, int col) {
	int i, sum = 0;
	for (i = 0; i < 9; i++) {
		sum += puzzle->cells[col][i].value;
	}
	return sum;
}
	

void pushGuesses(StackNode **head, Puzzle puzzle) {
	int i, j, k, l;
	Puzzle newCopy;

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (puzzle.cells[i][j].value == 0) {
				for (k = 0; k < 9; k++) {
					if (puzzle.cells[i][j].possible[k] != 0) {
						newCopy = puzzle;
						newCopy.cells[i][j].value = k + 1;
						for (l = 0; l < 9; l++) {
							newCopy.cells[i][j].possible[l] = 0;
						}
						newCopy.cells[i][j].possible[k] = k + 1;
						StackPush(head, newCopy);
					} 
				}
				return;
			}
		}
	}
}

int main(int argc, char** argv) {
	Puzzle puzzle;
	Puzzle previous_puzzle;
	StackNode *head = NULL;

	initialize(&puzzle);
	read_in(&puzzle, argv[1]);
	StackPush(&head, puzzle);
	print(&puzzle, 0);
	
	while (!emptyStack(head)) {
		puzzle = StackPop(&head);
		while (puzzleChanged(&puzzle, &previous_puzzle)) {
			previous_puzzle = puzzle;
			simplify(&puzzle);
			finalize(&puzzle);
		}
		if (!puzzleFilled(&puzzle)) {
			pushGuesses(&head, puzzle);
		}
		else {
			if (puzzleCorrect(&puzzle)) {
				print(&puzzle, 0);
			}
		}
	}

	return 0;
}
