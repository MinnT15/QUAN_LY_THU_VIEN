#ifndef UTILS_H
#define UTILS_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* Constants */
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define MAX_NAME_LENGTH 50
#define MAX_GENRE_LENGTH 30
#define MAX_BOOKS 100
#define MAX_USERS 50
#define MAX_BORROWED_BOOKS 5
#define DATE_LENGTH 20
#define BORROW_PERIOD_DAYS 14
#define NO_BORROWER -1
#define FILENAME "library_data.txt"

/* Type Definitions */
typedef enum { BOOK_AVAILABLE, BOOK_BORROWED } BookStatus;

typedef enum {
  SUCCESS,
  ERROR_BOOK_NOT_FOUND,
  ERROR_USER_NOT_FOUND,
  ERROR_BOOK_ALREADY_BORROWED,
  ERROR_BOOK_NOT_BORROWED,
  ERROR_MAX_BOOKS_REACHED,
  ERROR_MAX_USERS_REACHED,
  ERROR_INVALID_INPUT,
  ERROR_USER_BORROW_LIMIT,
  ERROR_FILE_IO,
  ERROR_OVERDUE_BOOK
} ErrorCode;

typedef struct {
  int id;
  char title[MAX_TITLE_LENGTH];
  char author[MAX_AUTHOR_LENGTH];
  char genre[MAX_GENRE_LENGTH];
  BookStatus status;
  int borrower_id;
} Book;

typedef struct {
  int id;
  char name[MAX_NAME_LENGTH];
  int borrowed_book_ids[MAX_BORROWED_BOOKS];
  time_t borrow_dates[MAX_BORROWED_BOOKS];
  int borrowed_count;
} User;

typedef struct {
  Book books[MAX_BOOKS];
  int book_count;
  int next_book_id;
  User users[MAX_USERS];
  int user_count;
  int next_user_id;
} Library;

/* Utility Functions */
void init_library(Library *lib);
int generate_book_id(Library *lib);
int generate_user_id(Library *lib);
bool is_valid_string(const char *str);
const char *get_error_message(ErrorCode error);

/* Input Handling */
int get_integer_input(const char *prompt, int min, int max);
void get_string_input(char *buffer, int size, const char *prompt);
void clear_input_buffer(void);

/* File I/O */
ErrorCode save_library_to_file(Library *lib, const char *filename);
ErrorCode load_library_from_file(Library *lib, const char *filename);

/* Date Utilities */
void format_time(time_t time_val, char *buffer, size_t size);
time_t calculate_due_date(time_t borrow_date, int days);
int is_overdue(time_t due_date);

/* String Utilities */
char *stristr(const char *haystack, const char *needle);

#endif /* UTILS_H */
