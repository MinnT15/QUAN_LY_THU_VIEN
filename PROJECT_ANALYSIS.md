# Phân Tích Toàn Bộ Dự Án: Hệ Thống Quản Lý Thư Viện

**Tác giả**: Embedded C Course Project  
**Ngày tạo**: 2026-01-04  
**Phiên bản**: 1.0

---

## 📋 Mục Lục

1. [Tổng Quan Dự Án](#1-tổng-quan-dự-án)
2. [Kiến Trúc Hệ Thống](#2-kiến-trúc-hệ-thống)
3. [Module Utils - Foundation](#3-module-utils---foundation)
4. [Module Book](#4-module-book)
5. [Module User](#5-module-user)
6. [Module Management](#6-module-management)
7. [Module Main](#7-module-main)
8. [Kỹ Thuật Lập Trình](#8-kỹ-thuật-lập-trình)
9. [Build và Deployment](#9-build-và-deployment)

---

## 1. Tổng Quan Dự Án

### 1.1. Mô Tả

Hệ thống quản lý thư viện là một ứng dụng console được viết bằng C, tuân thủ Clean Code principles và C Coding Convention. Dự án được tổ chức theo kiến trúc modular với 4 modules độc lập.

### 1.2. Tính Năng Chính

- ✅ **Quản lý sách**: CRUD operations với genre support
- ✅ **Quản lý người dùng**: CRUD operations với borrow tracking
- ✅ **Mượn/trả sách**: Date tracking, due date, overdue detection
- ✅ **Tìm kiếm**: Case-insensitive search by title/author/genre
- ✅ **Báo cáo**: Statistics, overdue books
- ✅ **Data persistence**: Auto save/load từ file

### 1.3. Thống Kê Dự Án

| Metric | Giá trị |
|--------|---------|
| **Tổng số files** | 9 files (.c và .h) |
| **Tổng dòng code** | ~1,000 dòng |
| **Số modules** | 4 modules |
| **Số functions** | 35+ functions |
| **Số constants** | 9 constants |
| **Error codes** | 11 error types |

---

## 2. Kiến Trúc Hệ Thống

### 2.1. Cấu Trúc Thư Mục

```
QUANLYTHUVIEN/
├── Utils/              # Foundation module
│   ├── utils.h         # Constants, types, prototypes
│   └── utils.c         # Utility implementations
├── Book/               # Book management module
│   ├── book.h          # Book function prototypes
│   └── book.c          # Book implementations
├── User/               # User management module
│   ├── user.h          # User function prototypes
│   └── user.c          # User implementations
├── Management/         # Business logic module
│   ├── management.h    # Management prototypes
│   └── management.c    # Borrow/return/display
├── build/              # Object files
├── bin/                # Executable
├── main.c              # Entry point
├── Makefile            # Build automation
└── README.md           # Documentation
```

### 2.2. Module Dependencies

```
main.c
  ├─> Utils/utils.h
  ├─> Book/book.h
  ├─> User/user.h
  └─> Management/management.h

Management/management.c
  ├─> Management/management.h
  ├─> Book/book.h
  ├─> User/user.h
  └─> Utils/utils.h

Book/book.c
  ├─> Book/book.h
  └─> Utils/utils.h

User/user.c
  ├─> User/user.h
  └─> Utils/utils.h

Utils/utils.c
  └─> Utils/utils.h
```

**Nguyên tắc**: `utils.h` là foundation - tất cả modules có thể include nó.

---

## 3. Module Utils - Foundation

### 3.1. utils.h - Constants và Types

```c
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
```

**Phân tích**:
- **Header Guards**: Ngăn multiple inclusion
- **Constants**: Định nghĩa tất cả magic numbers
- **BookStatus**: 2 trạng thái sách
- **ErrorCode**: 11 loại lỗi
- **Book**: 6 fields (id, title, author, genre, status, borrower_id)
- **User**: 5 fields (id, name, borrowed_book_ids[], borrow_dates[], borrowed_count)
- **Library**: Central data structure

### 3.2. utils.c - Core Functions

**Xem file đầy đủ**: [Utils/utils.c](file:///d:/COURSE/Embedded%20C/BT/QUANLYTHUVIEN/Utils/utils.c)

**Các functions quan trọng**:

1. **`init_library()`**: Khởi tạo library với counters = 0, next_id = 1
2. **`generate_book_id()` / `generate_user_id()`**: Tạo ID unique (fix ID conflict issue)
3. **`stristr()`**: Case-insensitive string search
4. **`get_integer_input()`**: Safe integer input với validation
5. **`get_string_input()`**: Safe string input, trim whitespace
6. **`save_library_to_file()`**: Lưu data ra file
7. **`load_library_from_file()`**: Load data từ file
8. **Date utilities**: `format_time()`, `calculate_due_date()`, `is_overdue()`

---

## 4. Module Book

### 4.1. book.h

```c
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
```

### 4.2. book.c - Key Functions

**Xem file đầy đủ**: [Book/book.c](file:///d:/COURSE/Embedded%20C/BT/QUANLYTHUVIEN/Book/book.c)

**`add_book()`**:
- Validate input (title, author, genre)
- Check MAX_BOOKS limit
- Generate unique ID
- Copy strings an toàn với `strncpy()`
- Set status = BOOK_AVAILABLE, borrower_id = NO_BORROWER

**`delete_book()`**:
- Tìm book by ID
- **Không cho xóa** nếu đang borrowed
- Shift array để xóa

**Search functions**:
- Sử dụng `stristr()` cho case-insensitive search
- Hỗ trợ partial match

---

## 5. Module User

### 5.1. user.h

```c
#ifndef USER_H
#define USER_H

#include "../Utils/utils.h"

ErrorCode add_user(Library *lib, const char *name);
ErrorCode update_user(Library *lib, int user_id, const char *name);
ErrorCode delete_user(Library *lib, int user_id);
User *find_user_by_id(Library *lib, int user_id);

#endif /* USER_H */
```

### 5.2. user.c - Key Functions

**Xem file đầy đủ**: [User/user.c](file:///d:/COURSE/Embedded%20C/BT/QUANLYTHUVIEN/User/user.c)

**`delete_user()`**:
- **Không cho xóa** user đang mượn sách
- Business rule quan trọng

---

## 6. Module Management

### 6.1. management.h

```c
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
```

### 6.2. management.c - Business Logic

**Xem file đầy đủ**: [Management/management.c](file:///d:/COURSE/Embedded%20C/BT/QUANLYTHUVIEN/Management/management.c)

**`borrow_book()` - Logic**:
1. Validate user exists
2. Validate book exists
3. Check book available
4. Check user borrow limit (MAX_BORROWED_BOOKS = 5)
5. Update book status và borrower_id
6. **Add borrow_date = time(NULL)** ← Tracking ngày mượn
7. Increment borrowed_count

**`display_user_info()` - Advanced**:
- Display user info
- Loop qua borrowed books
- Calculate due date = borrow_date + 14 days
- Check nếu overdue
- Display với tag [OVERDUE!] nếu quá hạn

**`display_overdue_books()`**:
- Loop qua tất cả users
- Check từng borrowed book
- Calculate days overdue
- Display chi tiết

---

## 7. Module Main

### 7.1. main.c - Entry Point

**Xem file đầy đủ**: [main.c](file:///d:/COURSE/Embedded%20C/BT/QUANLYTHUVIEN/main.c)

**Main flow**:
```c
int main() {
  Library library;
  init_library(&library);
  
  // Load data
  load_library_from_file(&library, FILENAME);
  
  // Add sample data if empty
  if (library.book_count == 0) {
    add_book(&library, "Clean Code", "Robert C. Martin", "Programming");
    // ...
  }
  
  // Menu loop
  while (1) {
    print_menu();
    choice = get_integer_input("Choose function: ", 0, 17);
    
    switch (choice) {
      case 1: // Add book
      case 2: // Update book
      // ...
      case 0: // Exit
        save_library_to_file(&library, FILENAME);
        return 0;
    }
  }
}
```

**Pattern**: Mỗi case:
1. Get input
2. Call function
3. Display result
4. **Auto-save nếu SUCCESS**

---

## 8. Kỹ Thuật Lập Trình

### 8.1. Memory Management

**Static Allocation**:
```c
Book books[MAX_BOOKS];  // Fixed size array
```

**Ưu điểm**: Đơn giản, phù hợp embedded systems  
**Nhược điểm**: Fixed size, có thể lãng phí memory

### 8.2. String Handling

**Safe String Copy**:
```c
strncpy(book->title, title, MAX_TITLE_LENGTH - 1);
book->title[MAX_TITLE_LENGTH - 1] = '\0';  // ← Quan trọng!
```

**Tại sao**: `strncpy()` không tự động null-terminate

### 8.3. Error Handling

**Enum-based Pattern**:
```c
ErrorCode result = add_book(...);
if (result == SUCCESS) {
  save_library_to_file(...);
} else {
  printf("%s\n", get_error_message(result));
}
```

### 8.4. Input Validation

**Layered Validation**:
1. Type validation (`scanf()` return value)
2. Range validation (min/max)
3. Empty string validation
4. Business logic validation

---

## 9. Build và Deployment

### 9.1. Makefile

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -IBook -IUser -IManagement -IUtils

all: bin/QUANLYTHUVIEN.exe

bin/QUANLYTHUVIEN.exe: build/*.o
	$(CC) $^ -o $@
```

**Commands**:
- `make` - Build
- `make clean` - Clean
- `make run` - Build and run

### 9.2. Code::Blocks

File `.cbp` đã cấu hình:
- Tất cả source files
- Include directories
- Compiler flags: `-Wall -std=c99`

---

## Tổng Kết

### Điểm Mạnh

1. ✅ **Kiến trúc Modular**: Clean separation
2. ✅ **Error Handling**: 11 error types
3. ✅ **Input Validation**: Robust
4. ✅ **Data Persistence**: Auto save/load
5. ✅ **Code Quality**: Clean Code, C conventions

### Complexity Analysis

| Operation | Time | Space |
|-----------|------|-------|
| Add/Update | O(1) | O(1) |
| Find/Delete | O(n) | O(1) |
| Search | O(n) | O(1) |
| File I/O | O(n) | O(n) |

### Constants Reference

| Constant | Value | Purpose |
|----------|-------|---------|
| MAX_BOOKS | 100 | Max books in library |
| MAX_USERS | 50 | Max users |
| MAX_BORROWED_BOOKS | 5 | Max books per user |
| BORROW_PERIOD_DAYS | 14 | Loan period (2 weeks) |
| NO_BORROWER | -1 | No borrower constant |

---

**Kết luận**: Dự án hoàn chỉnh, professional, educational value cao!

**Xem code đầy đủ tại**: `d:\COURSE\Embedded C\BT\QUANLYTHUVIEN\`
