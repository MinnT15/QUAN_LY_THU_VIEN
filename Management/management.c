#include "management.h"

/* Borrow/Return Functions */

ErrorCode borrow_book(Library *lib, int user_id, int book_id) {
  User *user = find_user_by_id(lib, user_id);
  if (user == NULL) {
    return ERROR_USER_NOT_FOUND;
  }

  Book *book = find_book_by_id(lib, book_id);
  if (book == NULL) {
    return ERROR_BOOK_NOT_FOUND;
  }

  if (book->status == BOOK_BORROWED) {
    return ERROR_BOOK_ALREADY_BORROWED;
  }

  if (user->borrowed_count >= MAX_BORROWED_BOOKS) {
    return ERROR_USER_BORROW_LIMIT;
  }

  book->status = BOOK_BORROWED;
  book->borrower_id = user_id;
  user->borrowed_book_ids[user->borrowed_count] = book_id;
  user->borrow_dates[user->borrowed_count] = time(NULL);
  user->borrowed_count++;

  return SUCCESS;
}

ErrorCode return_book(Library *lib, int user_id, int book_id) {
  User *user = find_user_by_id(lib, user_id);
  if (user == NULL) {
    return ERROR_USER_NOT_FOUND;
  }

  Book *book = find_book_by_id(lib, book_id);
  if (book == NULL) {
    return ERROR_BOOK_NOT_FOUND;
  }

  if (book->status != BOOK_BORROWED || book->borrower_id != user_id) {
    return ERROR_BOOK_NOT_BORROWED;
  }

  book->status = BOOK_AVAILABLE;
  book->borrower_id = NO_BORROWER;

  int index = -1;
  for (int i = 0; i < user->borrowed_count; i++) {
    if (user->borrowed_book_ids[i] == book_id) {
      index = i;
      break;
    }
  }

  if (index != -1) {
    for (int i = index; i < user->borrowed_count - 1; i++) {
      user->borrowed_book_ids[i] = user->borrowed_book_ids[i + 1];
      user->borrow_dates[i] = user->borrow_dates[i + 1];
    }
    user->borrowed_count--;
  }

  return SUCCESS;
}

/* Display Functions */

void display_available_books(Library *lib) {
  printf("\n=== Available Books ===\n");
  bool has_available = false;

  for (int i = 0; i < lib->book_count; i++) {
    if (lib->books[i].status == BOOK_AVAILABLE) {
      printf("ID: %d | Title: %s | Author: %s | Genre: %s\n", lib->books[i].id,
             lib->books[i].title, lib->books[i].author, lib->books[i].genre);
      has_available = true;
    }
  }

  if (!has_available) {
    printf("No available books!\n");
  }
}

void display_user_info(Library *lib, int user_id) {
  User *user = find_user_by_id(lib, user_id);
  if (user == NULL) {
    printf("User not found!\n");
    return;
  }

  printf("\n=== User Information ===\n");
  printf("ID: %d | Name: %s\n", user->id, user->name);
  printf("Borrowed books count: %d\n", user->borrowed_count);

  if (user->borrowed_count > 0) {
    printf("Borrowed books list:\n");
    for (int i = 0; i < user->borrowed_count; i++) {
      Book *book = find_book_by_id(lib, user->borrowed_book_ids[i]);
      if (book != NULL) {
        char borrow_date[DATE_LENGTH];
        char due_date_str[DATE_LENGTH];
        time_t due_date =
            calculate_due_date(user->borrow_dates[i], BORROW_PERIOD_DAYS);

        format_time(user->borrow_dates[i], borrow_date, DATE_LENGTH);
        format_time(due_date, due_date_str, DATE_LENGTH);

        printf("  - [%d] %s (Author: %s)\n", book->id, book->title,
               book->author);
        printf("    Borrowed: %s | Due: %s", borrow_date, due_date_str);

        if (is_overdue(due_date)) {
          printf(" [OVERDUE!]");
        }
        printf("\n");
      }
    }
  }
}

void display_all_books(Library *lib) {
  printf("\n=== All Books ===\n");
  if (lib->book_count == 0) {
    printf("No books in library!\n");
    return;
  }

  for (int i = 0; i < lib->book_count; i++) {
    printf("ID: %d | Title: %s | Author: %s | Genre: %s | Status: %s\n",
           lib->books[i].id, lib->books[i].title, lib->books[i].author,
           lib->books[i].genre,
           lib->books[i].status == BOOK_AVAILABLE ? "Available" : "Borrowed");
  }
}

void display_all_users(Library *lib) {
  printf("\n=== All Users ===\n");
  if (lib->user_count == 0) {
    printf("No users!\n");
    return;
  }

  for (int i = 0; i < lib->user_count; i++) {
    printf("ID: %d | Name: %s | Borrowed books: %d\n", lib->users[i].id,
           lib->users[i].name, lib->users[i].borrowed_count);
  }
}

void display_statistics(Library *lib) {
  printf("\n=== Library Statistics ===\n");

  int available_books = 0;
  int borrowed_books = 0;

  for (int i = 0; i < lib->book_count; i++) {
    if (lib->books[i].status == BOOK_AVAILABLE) {
      available_books++;
    } else {
      borrowed_books++;
    }
  }

  int active_borrowers = 0;
  for (int i = 0; i < lib->user_count; i++) {
    if (lib->users[i].borrowed_count > 0) {
      active_borrowers++;
    }
  }

  printf("Total Books: %d\n", lib->book_count);
  printf("  - Available: %d\n", available_books);
  printf("  - Borrowed: %d\n", borrowed_books);
  printf("\nTotal Users: %d\n", lib->user_count);
  printf("  - Active Borrowers: %d\n", active_borrowers);
  printf("  - Inactive: %d\n", lib->user_count - active_borrowers);
}

void display_overdue_books(Library *lib) {
  printf("\n=== Overdue Books ===\n");
  bool has_overdue = false;

  for (int i = 0; i < lib->user_count; i++) {
    User *user = &lib->users[i];
    for (int j = 0; j < user->borrowed_count; j++) {
      time_t due_date =
          calculate_due_date(user->borrow_dates[j], BORROW_PERIOD_DAYS);
      if (is_overdue(due_date)) {
        Book *book = find_book_by_id(lib, user->borrowed_book_ids[j]);
        if (book != NULL) {
          char due_date_str[DATE_LENGTH];
          format_time(due_date, due_date_str, DATE_LENGTH);

          time_t now = time(NULL);
          int days_overdue = (int)((now - due_date) / (24 * 60 * 60));

          printf("Book: %s (ID: %d)\n", book->title, book->id);
          printf("  Borrower: %s (ID: %d)\n", user->name, user->id);
          printf("  Due Date: %s\n", due_date_str);
          printf("  Days Overdue: %d\n\n", days_overdue);
          has_overdue = true;
        }
      }
    }
  }

  if (!has_overdue) {
    printf("No overdue books!\n");
  }
}
