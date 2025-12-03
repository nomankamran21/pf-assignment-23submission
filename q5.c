#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INITIAL_CAPACITY 4
#define MAX_LINE_LENGTH 1024

typedef struct {
    char **lines;
    size_t size;
    size_t capacity;
} TextEditor;

TextEditor* createEditor();
void insertLine(TextEditor* editor, size_t index, const char* text);
void deleteLine(TextEditor* editor, size_t index);
void printAllLines(const TextEditor* editor);
void freeAll(TextEditor* editor);
void saveToFile(const TextEditor* editor, const char* filename);
void loadFromFile(TextEditor* editor, const char* filename);
void shrinkToFit(TextEditor* editor);

int main() {
    TextEditor *editor = createEditor();
    
    insertLine(editor, 0, "First line.");
    insertLine(editor, 1, "Second line.");
    insertLine(editor, 1, "Inserted line.");
    printAllLines(editor);
    
    saveToFile(editor, "output.txt");
    
    deleteLine(editor, 1);
    printAllLines(editor);
    
    TextEditor *newEditor = createEditor();
    loadFromFile(newEditor, "output.txt");
    printAllLines(newEditor);
    
    freeAll(editor);
    freeAll(newEditor);
    
    return 0;
}

TextEditor* createEditor() {
    TextEditor *editor = malloc(sizeof(TextEditor));
    if (!editor) exit(EXIT_FAILURE);
    editor->lines = malloc(INITIAL_CAPACITY * sizeof(char*));
    if (!editor->lines) exit(EXIT_FAILURE);
    editor->size = 0;
    editor->capacity = INITIAL_CAPACITY;
    return editor;
}

void insertLine(TextEditor* editor, size_t index, const char* text) {
    if (index > editor->size) return;
    if (editor->size >= editor->capacity) {
        editor->capacity *= 2;
        char **newLines = realloc(editor->lines, editor->capacity * sizeof(char*));
        if (!newLines) exit(EXIT_FAILURE);
        editor->lines = newLines;
    }
    
    memmove(&editor->lines[index + 1], &editor->lines[index], (editor->size - index) * sizeof(char*));
    editor->lines[index] = malloc((strlen(text) + 1) * sizeof(char));
    if (!editor->lines[index]) exit(EXIT_FAILURE);
    strcpy(editor->lines[index], text);
    editor->size++;
}

void deleteLine(TextEditor* editor, size_t index) {
    if (index >= editor->size) return;
    
    free(editor->lines[index]);
    memmove(&editor->lines[index], &editor->lines[index + 1], (editor->size - index - 1) * sizeof(char*));
    editor->size--;
    
    shrinkToFit(editor);
}

void printAllLines(const TextEditor* editor) {
    for (size_t i = 0; i < editor->size; i++) {
        printf("%zu: %s\n", i + 1, editor->lines[i]);
    }
}

void freeAll(TextEditor* editor) {
    for (size_t i = 0; i < editor->size; i++) {
        free(editor->lines[i]);
    }
    free(editor->lines);
    free(editor);
}

void saveToFile(const TextEditor* editor, const char* filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;

    for (size_t i = 0; i < editor->size; i++) {
        fprintf(file, "%s\n", editor->lines[i]);
    }
    
    fclose(file);
}

void loadFromFile(TextEditor* editor, const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        insertLine(editor, editor->size, line);
    }

    fclose(file);
}

void shrinkToFit(TextEditor* editor) {
    if (editor->size < editor->capacity) {
        char **newLines = realloc(editor->lines, editor->size * sizeof(char*));
        if (newLines) {
            editor->lines = newLines;
            editor->capacity = editor->size;
        }
    }
}