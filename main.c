#include "Book/book.h"
#include "Management/management.h"
#include "User/user.h"
#include "Utils/utils.h"


/* Main Program */

void print_menu() {
  printf("\n========================================\n");
  printf("   LIBRARY MANAGEMENT SYSTEM\n");
  printf("========================================\n");
  printf(" 1. Add book\n");
  printf(" 2. Update book\n");
  printf(" 3. Delete book\n");
  printf(" 4. Add user\n");
  printf(" 5. Update user\n");
  printf(" 6. Delete user\n");
  printf(" 7. Borrow book\n");
  printf(" 8. Return book\n");
  printf(" 9. Search books by title\n");
  printf(" 10. Search books by author\n");
  printf(" 11. Search books by genre\n");
  printf(" 12. Display available books\n");
  printf(" 13. Display user information\n");
  printf(" 14. Display all books\n");
  printf(" 15. Display all users\n");
  printf(" 16. Display statistics\n");
  printf(" 17. Display overdue books\n");
  printf(" 0. Exit\n");
  printf("========================================\n");
}

int main() {
  Library library;
  init_library(&library);

  /* Load data from file */
  ErrorCode load_result = load_library_from_file(&library, FILENAME);
  if (load_result == SUCCESS) {
    printf("Library data loaded successfully!\n");
  } else if (load_result == ERROR_FILE_IO) {
    printf("Warning: Could not load library data. Starting fresh.\n");
  }

  /* Add sample data if library is empty */
  if (library.book_count == 0) {
    add_book(&library, "Clean Code", "Robert C. Martin", "Programming");
    add_book(&library, "Design Patterns", "Gang of Four", "Programming");
    add_book(&library, "The Pragmatic Programmer", "Andrew Hunt",
             "Programming");
    add_user(&library, "John Doe");
    add_user(&library, "Jane Smith");
    printf("Sample data added.\n");
  }

  int choice;
  char title[MAX_TITLE_LENGTH];
  char author[MAX_AUTHOR_LENGTH];
  char genre[MAX_GENRE_LENGTH];
  char name[MAX_NAME_LENGTH];
  int id, book_id, user_id;
  ErrorCode result;

  while (1) {
    print_menu();
    choice = get_integer_input("Choose function: ", 0, 17);

    switch (choice) {
    case 1:
      get_string_input(title, MAX_TITLE_LENGTH, "Enter title: ");
      get_string_input(author, MAX_AUTHOR_LENGTH, "Enter author: ");
      get_string_input(genre, MAX_GENRE_LENGTH, "Enter genre: ");
      result = add_book(&library, title, author, genre);
      printf("%s\n", get_error_message(result));
      if (result == SUCCESS)
        save_library_to_file(&library, FILENAME);
      break;

    case 2:
      id = get_integer_input("Enter book ID: ", 1, 999999);
      get_string_input(title, MAX_TITLE_LENGTH, "Enter new title: ");
      get_string_input(author, MAX_AUTHOR_LENGTH, "Enter new author: ");
      get_string_input(genre, MAX_GENRE_LENGTH, "Enter new genre: ");
      result = update_book(&library, id, title, author, genre);
      printf("%s\n", get_error_message(result));
      if (result == SUCCESS)
        save_library_to_file(&library, FILENAME);
      break;

    case 3:
      id = get_integer_input("Enter book ID to delete: ", 1, 999999);
      result = delete_book(&library, id);
      printf("%s\n", get_error_message(result));
      if (result == SUCCESS)
        save_library_to_file(&library, FILENAME);
      break;

    case 4:
      get_string_input(name, MAX_NAME_LENGTH, "Enter user name: ");
      result = add_user(&library, name);
      printf("%s\n", get_error_message(result));
      if (result == SUCCESS)
        save_library_to_file(&library, FILENAME);
      break;

    case 5:
      id = get_integer_input("Enter user ID: ", 1, 999999);
      get_string_input(name, MAX_NAME_LENGTH, "Enter new name: ");
      result = update_user(&library, id, name);
      printf("%s\n", get_error_message(result));
      if (result == SUCCESS)
        save_library_to_file(&library, FILENAME);
      break;

    case 6:
      id = get_integer_input("Enter user ID to delete: ", 1, 999999);
      result = delete_user(&library, id);
      printf("%s\n", get_error_message(result));
      if (result == SUCCESS)
        save_library_to_file(&library, FILENAME);
      break;

    case 7:
      user_id = get_integer_input("Enter user ID: ", 1, 999999);
      book_id = get_integer_input("Enter book ID: ", 1, 999999);
      result = borrow_book(&library, user_id, book_id);
      printf("%s\n", get_error_message(result));
      if (result == SUCCESS)
        save_library_to_file(&library, FILENAME);
      break;

    case 8:
      user_id = get_integer_input("Enter user ID: ", 1, 999999);
      book_id = get_integer_input("Enter book ID: ", 1, 999999);
      result = return_book(&library, user_id, book_id);
      printf("%s\n", get_error_message(result));
      if (result == SUCCESS)
        save_library_to_file(&library, FILENAME);
      break;

    case 9:
      get_string_input(title, MAX_TITLE_LENGTH, "Enter title to search: ");
      search_books_by_title(&library, title);
      break;

    case 10:
      get_string_input(author, MAX_AUTHOR_LENGTH, "Enter author to search: ");
      search_books_by_author(&library, author);
      break;

    case 11:
      get_string_input(genre, MAX_GENRE_LENGTH, "Enter genre to search: ");
      search_books_by_genre(&library, genre);
      break;

    case 12:
      display_available_books(&library);
      break;

    case 13:
      id = get_integer_input("Enter user ID: ", 1, 999999);
      display_user_info(&library, id);
      break;

    case 14:
      display_all_books(&library);
      break;

    case 15:
      display_all_users(&library);
      break;

    case 16:
      display_statistics(&library);
      break;

    case 17:
      display_overdue_books(&library);
      break;

    case 0:
      save_library_to_file(&library, FILENAME);
      printf("Data saved. Thank you for using the system!\n");
      return 0;

    default:
      printf("Invalid choice!\n");
    }
  }

  return 0;
}
