#include "utils.h"

/* Utility Functions */

void init_library(Library *lib) {
  lib->book_count = 0;
  lib->user_count = 0;
  lib->next_book_id = 1;
  lib->next_user_id = 1;
}

int generate_book_id(Library *lib) { return lib->next_book_id++; }

int generate_user_id(Library *lib) { return lib->next_user_id++; }

bool is_valid_string(const char *str) { return str != NULL && strlen(str) > 0; }

const char *get_error_message(ErrorCode error) {
  switch (error) {
  case SUCCESS:
    return "Operation successful";
  case ERROR_BOOK_NOT_FOUND:
    return "Book not found";
  case ERROR_USER_NOT_FOUND:
    return "User not found";
  case ERROR_BOOK_ALREADY_BORROWED:
    return "Book already borrowed";
  case ERROR_BOOK_NOT_BORROWED:
    return "Book not borrowed";
  case ERROR_MAX_BOOKS_REACHED:
    return "Maximum books limit reached";
  case ERROR_MAX_USERS_REACHED:
    return "Maximum users limit reached";
  case ERROR_INVALID_INPUT:
    return "Invalid input data";
  case ERROR_USER_BORROW_LIMIT:
    return "User borrow limit reached";
  case ERROR_FILE_IO:
    return "File I/O error";
  case ERROR_OVERDUE_BOOK:
    return "Book is overdue";
  default:
    return "Unknown error";
  }
}

/* String Utilities */

char *stristr(const char *haystack, const char *needle) {
  if (!haystack || !needle)
    return NULL;

  size_t needle_len = strlen(needle);
  if (needle_len == 0)
    return (char *)haystack;

  for (; *haystack; haystack++) {
    if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
      size_t i;
      for (i = 1; i < needle_len; i++) {
        if (tolower((unsigned char)haystack[i]) !=
            tolower((unsigned char)needle[i])) {
          break;
        }
      }
      if (i == needle_len) {
        return (char *)haystack;
      }
    }
  }
  return NULL;
}

/* Date Utilities */

void format_time(time_t time_val, char *buffer, size_t size) {
  struct tm *tm_info = localtime(&time_val);
  strftime(buffer, size, "%Y-%m-%d %H:%M", tm_info);
}

time_t calculate_due_date(time_t borrow_date, int days) {
  return borrow_date + (days * 24 * 60 * 60);
}

int is_overdue(time_t due_date) {
  time_t now = time(NULL);
  return now > due_date;
}

/* Input Handling Functions */

void clear_input_buffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

int get_integer_input(const char *prompt, int min, int max) {
  int value;
  int result;

  while (1) {
    printf("%s", prompt);
    result = scanf("%d", &value);

    if (result != 1) {
      printf("Invalid input! Please enter a number.\n");
      clear_input_buffer();
      continue;
    }

    clear_input_buffer();

    if (value < min || value > max) {
      printf("Input out of range! Please enter a value between %d and %d.\n",
             min, max);
      continue;
    }

    return value;
  }
}

void get_string_input(char *buffer, int size, const char *prompt) {
  while (1) {
    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buffer, size, stdin) != NULL) {
      /* Remove trailing newline */
      size_t len = strlen(buffer);
      if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
      }

      /* Trim leading whitespace */
      char *start = buffer;
      while (*start && isspace((unsigned char)*start)) {
        start++;
      }
      if (start != buffer) {
        memmove(buffer, start, strlen(start) + 1);
      }

      /* Trim trailing whitespace */
      len = strlen(buffer);
      while (len > 0 && isspace((unsigned char)buffer[len - 1])) {
        buffer[--len] = '\0';
      }

      /* Check if input is empty after trimming */
      if (strlen(buffer) > 0) {
        return; /* Valid input received */
      }

      /* Empty input, ask again */
      printf("Input cannot be empty. Please try again.\n");
    } else {
      /* Error reading input */
      buffer[0] = '\0';
      return;
    }
  }
}

/* File I/O Functions */

ErrorCode save_library_to_file(Library *lib, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    return ERROR_FILE_IO;
  }

  /* Save metadata */
  fprintf(file, "%d %d %d %d\n", lib->book_count, lib->next_book_id,
          lib->user_count, lib->next_user_id);

  /* Save books */
  for (int i = 0; i < lib->book_count; i++) {
    fprintf(file, "BOOK|%d|%s|%s|%s|%d|%d\n", lib->books[i].id,
            lib->books[i].title, lib->books[i].author, lib->books[i].genre,
            lib->books[i].status, lib->books[i].borrower_id);
  }

  /* Save users */
  for (int i = 0; i < lib->user_count; i++) {
    fprintf(file, "USER|%d|%s|%d", lib->users[i].id, lib->users[i].name,
            lib->users[i].borrowed_count);

    for (int j = 0; j < lib->users[i].borrowed_count; j++) {
      fprintf(file, "|%d|%ld", lib->users[i].borrowed_book_ids[j],
              (long)lib->users[i].borrow_dates[j]);
    }
    fprintf(file, "\n");
  }

  fclose(file);
  return SUCCESS;
}

ErrorCode load_library_from_file(Library *lib, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    /* File doesn't exist yet, not an error */
    return SUCCESS;
  }

  /* Load metadata */
  if (fscanf(file, "%d %d %d %d\n", &lib->book_count, &lib->next_book_id,
             &lib->user_count, &lib->next_user_id) != 4) {
    fclose(file);
    return ERROR_FILE_IO;
  }

  char line[512];
  int book_idx = 0;
  int user_idx = 0;

  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "BOOK|", 5) == 0) {
      /* Parse book */
      Book *book = &lib->books[book_idx];

      char *token = strtok(line, "|");
      token = strtok(NULL, "|"); /* ID */
      book->id = atoi(token);

      token = strtok(NULL, "|"); /* Title */
      strncpy(book->title, token, MAX_TITLE_LENGTH - 1);
      book->title[MAX_TITLE_LENGTH - 1] = '\0';

      token = strtok(NULL, "|"); /* Author */
      strncpy(book->author, token, MAX_AUTHOR_LENGTH - 1);
      book->author[MAX_AUTHOR_LENGTH - 1] = '\0';

      token = strtok(NULL, "|"); /* Genre */
      strncpy(book->genre, token, MAX_GENRE_LENGTH - 1);
      book->genre[MAX_GENRE_LENGTH - 1] = '\0';

      token = strtok(NULL, "|"); /* Status */
      book->status = atoi(token);

      token = strtok(NULL, "|\n"); /* Borrower ID */
      book->borrower_id = atoi(token);

      book_idx++;
    } else if (strncmp(line, "USER|", 5) == 0) {
      /* Parse user */
      User *user = &lib->users[user_idx];

      char *token = strtok(line, "|");
      token = strtok(NULL, "|"); /* ID */
      user->id = atoi(token);

      token = strtok(NULL, "|"); /* Name */
      strncpy(user->name, token, MAX_NAME_LENGTH - 1);
      user->name[MAX_NAME_LENGTH - 1] = '\0';

      token = strtok(NULL, "|"); /* Borrowed count */
      user->borrowed_count = atoi(token);

      for (int j = 0; j < user->borrowed_count; j++) {
        token = strtok(NULL, "|"); /* Book ID */
        user->borrowed_book_ids[j] = atoi(token);

        token = strtok(NULL, "|\n"); /* Borrow date */
        user->borrow_dates[j] = (time_t)atol(token);
      }

      user_idx++;
    }
  }

  fclose(file);
  return SUCCESS;
}
