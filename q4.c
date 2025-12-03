#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAPACITY 100

typedef struct {
    int id;            
    int popularity;    
    int accessed;      
} Book;

void addBook(Book* shelf, int* currentSize, int* time, int id, int popularity, int capacity) {
    for (int i = 0; i < *currentSize; i++) {
        if (shelf[i].id == id) {
            shelf[i].popularity = popularity;
            shelf[i].accessed = (*time)++;
            return;
        }
    }

    if (*currentSize == capacity) {
        int lruIndex = 0;
        for (int i = 1; i < *currentSize; i++) {
            if (shelf[i].accessed < shelf[lruIndex].accessed) {
                lruIndex = i;
            }
        }
        for (int i = lruIndex; i < *currentSize - 1; i++) {
            shelf[i] = shelf[i + 1];
        }
        (*currentSize)--; 
    }

    shelf[*currentSize].id = id;
    shelf[*currentSize].popularity = popularity;
    shelf[*currentSize].accessed = (*time)++;
    (*currentSize)++;
}

int accessBook(Book* shelf, int currentSize, int* time, int id) {
    for (int i = 0; i < currentSize; i++) {
        if (shelf[i].id == id) {
            shelf[i].accessed = (*time)++;
            return shelf[i].popularity;
        }
    }
    return -1;
}

int main() {
    int capacity, Q;
    scanf("%d %d", &capacity, &Q);

    Book* shelf = (Book*)malloc(capacity * sizeof(Book));
    int currentSize = 0;
    int time = 0;

    for (int i = 0; i < Q; i++) {
        char operation[10];
        scanf("%s", operation);

        if (strcmp(operation, "ADD") == 0) {
            int id, popularity;
            scanf("%d %d", &id, &popularity);
            addBook(shelf, &currentSize, &time, id, popularity, capacity);
        } else if (strcmp(operation, "ACCESS") == 0) {
            int id;
            scanf("%d", &id);
            int score = accessBook(shelf, currentSize, &time, id);
            printf("%d\n", score);
        }
    }

    free(shelf);
    return 0;
}