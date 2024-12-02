#include <stdio.h>
#include <stdlib.h>

// Struct to hold rows with varying lengths
struct RowList {
    int **rows;       // Array of pointers to rows
    int *row_lengths; // Array of row lengths
    int row_count;    // Total number of rows
};
typedef struct RowList RowList;

// Function declarations
RowList readRows();
int countSafeReportsWithDampener(RowList rowData);
int isSafe(int *row, int length);
void freeRowList(RowList rowData);

int main(void) {
    RowList rowData = readRows();
    int count_safe = countSafeReportsWithDampener(rowData);
    printf("Total safe reports with Problem Dampener: %d\n", count_safe);
    freeRowList(rowData);
    return 0;
}

// Function to check if a report is safe
int isSafe(int *row, int length) {
    int is_increasing = 1, is_decreasing = 1;

    for (int i = 0; i < length - 1; i++) {
        int diff = row[i + 1] - row[i];

        // Check if difference is out of range [1, 3]
        if (abs(diff) < 1 || abs(diff) > 3) {
            return 0; // Unsafe
        }

        // Check monotonicity
        if (diff > 0) {
            is_decreasing = 0;
        } else if (diff < 0) {
            is_increasing = 0;
        }
    }

    // The row is safe if it's either strictly increasing or decreasing
    return is_increasing || is_decreasing;
}

// Function to count safe reports considering the Problem Dampener
int countSafeReportsWithDampener(RowList rowData) {
    int count_safe = 0;

    for (int i = 0; i < rowData.row_count; i++) {
        int *row = rowData.rows[i];
        int length = rowData.row_lengths[i];

        // Check if the report is already safe
        if (isSafe(row, length)) {
            count_safe++;
            continue;
        }

        // Check if removing one level makes it safe
        int dampener_safe = 0;
        for (int j = 0; j < length; j++) {
            // Create a new row with one level removed
            int *new_row = malloc((length - 1) * sizeof(int));
            if (!new_row) {
                printf("Memory allocation failed.\n");
                exit(1);
            }

            for (int k = 0, l = 0; k < length; k++) {
                if (k != j) {
                    new_row[l++] = row[k];
                }
            }

            // Check if the new row is safe
            if (isSafe(new_row, length - 1)) {
                dampener_safe = 1;
                free(new_row);
                break;
            }

            free(new_row);
        }

        if (dampener_safe) {
            count_safe++;
        }
    }

    return count_safe;
}

// Function to read rows of varying lengths
RowList readRows() {
    int capacity = 6; // Initial capacity for rows
    int row_count = 0;

    int **rows = malloc(capacity * sizeof(int *));
    int *row_lengths = malloc(capacity * sizeof(int));
    if (!rows || !row_lengths) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    printf("Enter rows of numbers separated by spaces. Type EOF (Ctrl+D) to end input:\n");

    while (1) {
        int *row = NULL;
        int length = 0, capacity_row = 5;

        row = malloc(capacity_row * sizeof(int));
        if (!row) {
            printf("Memory allocation for row failed.\n");
            break;
        }

        // Read numbers until newline or EOF
        int num;
        while (scanf("%d", &num) == 1) {
            if (length >= capacity_row) {
                capacity_row *= 2;
                row = realloc(row, capacity_row * sizeof(int));
                if (!row) {
                    printf("Memory reallocation for row failed.\n");
                    exit(1);
                }
            }
            row[length++] = num;

            // Check if next input is a newline
            if (getchar() == '\n') {
                break;
            }
        }

        // Exit loop on EOF
        if (length == 0) {
            free(row);
            break;
        }

        // Store the row and its length
        if (row_count >= capacity) {
            capacity *= 2;
            rows = realloc(rows, capacity * sizeof(int *));
            row_lengths = realloc(row_lengths, capacity * sizeof(int));
            if (!rows || !row_lengths) {
                printf("Memory reallocation failed.\n");
                exit(1);
            }
        }

        rows[row_count] = row;
        row_lengths[row_count] = length;
        row_count++;
    }

    RowList result;
    result.rows = rows;
    result.row_lengths = row_lengths;
    result.row_count = row_count;
    return result;
}

// Function to free allocated memory
void freeRowList(RowList rowData) {
    for (int i = 0; i < rowData.row_count; i++) {
        free(rowData.rows[i]);
    }
    free(rowData.rows);
    free(rowData.row_lengths);
}

