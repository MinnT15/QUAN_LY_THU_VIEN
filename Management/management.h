#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "../Book/book.h"
#include "../User/user.h"
#include "../Utils/utils.h"


/* Borrow/Return Management */
ErrorCode borrow_book(Library *lib, int user_id, int book_id);
ErrorCode return_book(Library *lib, int user_id, int book_id);

/* Display Functions */
void display_available_books(Library *lib);
void display_user_info(Library *lib, int user_id);
void display_all_books(Library *lib);
void display_all_users(Library *lib);
void display_statistics(Library *lib);
void display_overdue_books(Library *lib);

#endif /* MANAGEMENT_H */
