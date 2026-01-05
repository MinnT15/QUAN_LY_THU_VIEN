#ifndef BOOK_H
#define BOOK_H

#include "../Utils/utils.h"

/* Book Management Functions */
ErrorCode add_book(Library *lib, const char *title, const char *author,
                   const char *genre);
ErrorCode update_book(Library *lib, int book_id, const char *title,
                      const char *author, const char *genre);
ErrorCode delete_book(Library *lib, int book_id);
Book *find_book_by_id(Library *lib, int book_id);

/* Book Search Functions */
void search_books_by_title(Library *lib, const char *title);
void search_books_by_author(Library *lib, const char *author);
void search_books_by_genre(Library *lib, const char *genre);

#endif /* BOOK_H */
