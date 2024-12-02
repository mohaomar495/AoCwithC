#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct{
	int left;
	int right;
}Row;

typedef struct{
	Row *rows;
	int row_count;
} RowList;

int *sort_arr(int array[], int n);
void print_arr(int array[], int n);
int *distanceList(int array_1[], int array_2[], int n);
RowList readRows();
int totalDistance(int distanceList[], int n);
int *similarityList(int array_1[], int array_2[], int n);




int main(void){
	RowList rowData = readRows();
	int n_left = rowData.row_count;
	int n_right = rowData.row_count;

	int *leftList = malloc(n_left * sizeof(int));
	int *rightList = malloc(n_right * sizeof(int));

	for (int i = 0; i < n_left; i++){
		leftList[i] = rowData.rows[i].left;
		rightList[i] = rowData.rows[i].right;
	}

	int *orderedLeftList = 0;
	int *orderedRightList = 0;
	int *dist = 0;
	int totalDist = 0;
	int totalSim = 0;
	int *simList = 0;

	orderedLeftList = sort_arr(leftList, n_left);
	orderedRightList = sort_arr(rightList, n_right);
	dist = distanceList(orderedLeftList, orderedRightList, n_left);
	totalDist = totalDistance(dist, n_left);
	simList = similarityList(orderedLeftList, orderedRightList, n_left);
	totalSim = totalDistance(simList, n_left);


	/*
	printf("Original leftList\n");
	print_arr(leftList, n_left);
	printf("Original rightList\n");
	print_arr(rightList, n_right);

	printf("\n");

	printf("The new orderedLists: \n");
	printf("Ordered Left List: \n");
	print_arr(orderedLeftList, n_left);

	printf("Ordered Right List: \n");
	print_arr(orderedRightList, n_right);

	printf("\n");

	printf("Distance: \n");
	print_arr(dist, n_left);

	printf("\n");
	*/

	printf("Total distance among all pairs: %d\n", totalDist);
	printf("Total similarity among all pairs: %d\n", totalSim);


	free(rowData.rows);
	free(leftList);
	free(rightList);
	free(orderedLeftList);
	free(orderedRightList);
	free(simList);
	free(dist);
	return (0);
}



int *similarityList(int array_1[], int array_2[], int n){
	int *result = (int *) malloc(n * sizeof(int));
	if (!result){
		printf("Memory allocation failed\n");
		exit(1);
	}

	for (int i = 0; i < n; i++){
		int occurrence = 0;
		for (int j = 0; j < n; j++){
			if (array_1[i] == array_2[j]){
				occurrence++;
			}
		}
		result[i] = occurrence * array_1[i];
	}
	
	return result;
}



RowList readRows(){
	int capacity = 6;
	int row_count = 0;

	Row *rows = malloc(capacity * sizeof(Row));
	if (!rows){
		printf("Memory allocation failed\n");
		exit(1);
	}

	printf("Enter data as 'left right' pairs. Press Ctrl+D to end input: \n");

	while (1){
		int left, right;
		if (scanf("%d %d", &left, &right) != 2){
			break;
		}

		// resisze if necessary.
		if (row_count >= capacity){
			capacity *= 2;
			rows = realloc(rows, capacity * sizeof(Row));
			if (!rows){
				printf("Memory reallocation failed\n");
				exit(1);
			}
		}
		rows[row_count].left = left;
		rows[row_count].right = right;
		row_count++;
	}
	RowList result = {rows, row_count};
	return result;
}


int totalDistance(int array[], int n){
	int totalDist = 0;
	for (int i = 0; i < n; i++){
		totalDist += array[i];
	}

	return totalDist;
}


int *distanceList(int array_1[], int array_2[], int n){
	int *dist = (int *) malloc(n * sizeof(int));

	for (int i = 0; i < n; i++){
		dist[i] = abs(array_1[i] - array_2[i]);
	}
	return dist;
}


int *sort_arr(int array[], int n){
	int *sorted = (int *) malloc(n * sizeof(int));
	
	if (sorted == NULL){
		printf("Memory allocation failed!\n");
		exit(1);
	}

	// copy elements from the original array
	for (int i = 0; i < n; i++){
		sorted[i] = array[i];
	}


	for (int i = 0; i < n - 1; i++){
		for (int j = 0; j < n - i - 1; j++){
			if (sorted[j] > sorted[j +  1]){
				int temp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = temp;
			}
		}
	}

	return sorted;
}


void print_arr(int array[], int n){
	for (int i = 0; i < n; i++){
		printf("%d ", array[i]);
	}
	printf("\n");
}
