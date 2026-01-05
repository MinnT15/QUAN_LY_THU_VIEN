# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -IBook -IUser -IManagement -IUtils
OBJDIR = build
BINDIR = bin
TARGET = $(BINDIR)/QUANLYTHUVIEN.exe

# Source files
SOURCES = main.c Book/book.c User/user.c Management/management.c Utils/utils.c

# Object files
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/book.o $(OBJDIR)/user.o $(OBJDIR)/management.o $(OBJDIR)/utils.o

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	@echo Linking...
	$(CC) $(OBJECTS) -o $(TARGET)
	@echo Build complete: $(TARGET)

# Compile main.c
$(OBJDIR)/main.o: main.c Utils/utils.h Book/book.h User/user.h Management/management.h
	@echo Compiling main.c...
	$(CC) $(CFLAGS) -c main.c -o $(OBJDIR)/main.o

# Compile book.c
$(OBJDIR)/book.o: Book/book.c Book/book.h Utils/utils.h
	@echo Compiling Book/book.c...
	$(CC) $(CFLAGS) -c Book/book.c -o $(OBJDIR)/book.o

# Compile user.c
$(OBJDIR)/user.o: User/user.c User/user.h Utils/utils.h
	@echo Compiling User/user.c...
	$(CC) $(CFLAGS) -c User/user.c -o $(OBJDIR)/user.o

# Compile management.c
$(OBJDIR)/management.o: Management/management.c Management/management.h Book/book.h User/user.h Utils/utils.h
	@echo Compiling Management/management.c...
	$(CC) $(CFLAGS) -c Management/management.c -o $(OBJDIR)/management.o

# Compile utils.c
$(OBJDIR)/utils.o: Utils/utils.c Utils/utils.h
	@echo Compiling Utils/utils.c...
	$(CC) $(CFLAGS) -c Utils/utils.c -o $(OBJDIR)/utils.o

# Clean build artifacts
clean:
	@echo Cleaning build artifacts...
	rm -f $(OBJDIR)/*.o $(TARGET)
	@echo Clean complete.

# Rebuild from scratch
rebuild: clean all

# Run the program
run: $(TARGET)
	@echo Running program...
	$(TARGET)

# Help
help:
	@echo Available targets:
	@echo   all      - Build the project (default)
	@echo   clean    - Remove build artifacts
	@echo   rebuild  - Clean and build from scratch
	@echo   run      - Build and run the program
	@echo   help     - Show this help message

.PHONY: all clean rebuild run help
