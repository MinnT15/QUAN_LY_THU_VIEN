#include "book.h"

/* Book Management Functions */

ErrorCode add_book(Library *lib, const char *title, const char *author,
                   const char *genre) {
  if (!is_valid_string(title) || !is_valid_string(author) ||
      !is_valid_string(genre)) {
    return ERROR_INVALID_INPUT;
  }

  if (lib->book_count >= MAX_BOOKS) {
    return ERROR_MAX_BOOKS_REACHED;
  }

  Book *new_book = &lib->books[lib->book_count];
  new_book->id = generate_book_id(lib);
  strncpy(new_book->title, title, MAX_TITLE_LENGTH - 1);
  new_book->title[MAX_TITLE_LENGTH - 1] = '\0';
  strncpy(new_book->author, author, MAX_AUTHOR_LENGTH - 1);
  new_book->author[MAX_AUTHOR_LENGTH - 1] = '\0';
  strncpy(new_book->genre, genre, MAX_GENRE_LENGTH - 1);
  new_book->genre[MAX_GENRE_LENGTH - 1] = '\0';
  new_book->status = BOOK_AVAILABLE;
  new_book->borrower_id = NO_BORROWER;

  lib->book_count++;
  return SUCCESS;
}

ErrorCode update_book(Library *lib, int book_id, const char *title,
                      const char *author, const char *genre) {
  if (!is_valid_string(title) || !is_valid_string(author) ||
      !is_valid_string(genre)) {
    return ERROR_INVALID_INPUT;
  }

  Book *book = find_book_by_id(lib, book_id);
  if (book == NULL) {
    return ERROR_BOOK_NOT_FOUND;
  }

  strncpy(book->title, title, MAX_TITLE_LENGTH - 1);
  book->title[MAX_TITLE_LENGTH - 1] = '\0';
  strncpy(book->author, author, MAX_AUTHOR_LENGTH - 1);
  book->author[MAX_AUTHOR_LENGTH - 1] = '\0';
  strncpy(book->genre, genre, MAX_GENRE_LENGTH - 1);
  book->genre[MAX_GENRE_LENGTH - 1] = '\0';

  return SUCCESS;
}

ErrorCode delete_book(Library *lib, int book_id) {
  int index = -1;
  for (int i = 0; i < lib->book_count; i++) {
    if (lib->books[i].id == book_id) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    return ERROR_BOOK_NOT_FOUND;
  }

  if (lib->books[index].status == BOOK_BORROWED) {
    return ERROR_BOOK_ALREADY_BORROWED;
  }

  for (int i = index; i < lib->book_count - 1; i++) {
    lib->books[i] = lib->books[i + 1];
  }
  lib->book_count--;

  return SUCCESS;
}

Book *find_book_by_id(Library *lib, int book_id) {
  for (int i = 0; i < lib->book_count; i++) {
    if (lib->books[i].id == book_id) {
      return &lib->books[i];
    }
  }
  return NULL;
}

/* Book Search Functions */

void search_books_by_title(Library *lib, const char *title) {
  if (!is_valid_string(title)) {
    printf("Invalid search term!\n");
    return;
  }

  printf("\n=== Search by Title: '%s' ===\n", title);
  bool found = false;

  for (int i = 0; i < lib->book_count; i++) {
    if (stristr(lib->books[i].title, title) != NULL) {
      printf("ID: %d | Title: %s | Author: %s | Genre: %s | Status: %s\n",
             lib->books[i].id, lib->books[i].title, lib->books[i].author,
             lib->books[i].genre,
             lib->books[i].status == BOOK_AVAILABLE ? "Available" : "Borrowed");
      found = true;
    }
  }

  if (!found) {
    printf("No books found!\n");
  }
}

void search_books_by_author(Library *lib, const char *author) {
  if (!is_valid_string(author)) {
    printf("Invalid search term!\n");
    return;
  }

  printf("\n=== Search by Author: '%s' ===\n", author);
  bool found = false;

  for (int i = 0; i < lib->book_count; i++) {
    if (stristr(lib->books[i].author, author) != NULL) {
      printf("ID: %d | Title: %s | Author: %s | Genre: %s | Status: %s\n",
             lib->books[i].id, lib->books[i].title, lib->books[i].author,
             lib->books[i].genre,
             lib->books[i].status == BOOK_AVAILABLE ? "Available" : "Borrowed");
      found = true;
    }
  }

  if (!found) {
    printf("No books found!\n");
  }
}

void search_books_by_genre(Library *lib, const char *genre) {
  if (!is_valid_string(genre)) {
    printf("Invalid search term!\n");
    return;
  }

  printf("\n=== Search by Genre: '%s' ===\n", genre);
  bool found = false;

  for (int i = 0; i < lib->book_count; i++) {
    if (stristr(lib->books[i].genre, genre) != NULL) {
      printf("ID: %d | Title: %s | Author: %s | Genre: %s | Status: %s\n",
             lib->books[i].id, lib->books[i].title, lib->books[i].author,
             lib->books[i].genre,
             lib->books[i].status == BOOK_AVAILABLE ? "Available" : "Borrowed");
      found = true;
    }
  }

  if (!found) {
    printf("No books found!\n");
  }
}
