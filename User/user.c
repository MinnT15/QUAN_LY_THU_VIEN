#include "user.h"

/* User Management Functions */

ErrorCode add_user(Library *lib, const char *name) {
  if (!is_valid_string(name)) {
    return ERROR_INVALID_INPUT;
  }

  if (lib->user_count >= MAX_USERS) {
    return ERROR_MAX_USERS_REACHED;
  }

  User *new_user = &lib->users[lib->user_count];
  new_user->id = generate_user_id(lib);
  strncpy(new_user->name, name, MAX_NAME_LENGTH - 1);
  new_user->name[MAX_NAME_LENGTH - 1] = '\0';
  new_user->borrowed_count = 0;

  lib->user_count++;
  return SUCCESS;
}

ErrorCode update_user(Library *lib, int user_id, const char *name) {
  if (!is_valid_string(name)) {
    return ERROR_INVALID_INPUT;
  }

  User *user = find_user_by_id(lib, user_id);
  if (user == NULL) {
    return ERROR_USER_NOT_FOUND;
  }

  strncpy(user->name, name, MAX_NAME_LENGTH - 1);
  user->name[MAX_NAME_LENGTH - 1] = '\0';

  return SUCCESS;
}

ErrorCode delete_user(Library *lib, int user_id) {
  int index = -1;
  for (int i = 0; i < lib->user_count; i++) {
    if (lib->users[i].id == user_id) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    return ERROR_USER_NOT_FOUND;
  }

  if (lib->users[index].borrowed_count > 0) {
    printf("Cannot delete user with borrowed books!\n");
    return ERROR_INVALID_INPUT;
  }

  for (int i = index; i < lib->user_count - 1; i++) {
    lib->users[i] = lib->users[i + 1];
  }
  lib->user_count--;

  return SUCCESS;
}

User *find_user_by_id(Library *lib, int user_id) {
  for (int i = 0; i < lib->user_count; i++) {
    if (lib->users[i].id == user_id) {
      return &lib->users[i];
    }
  }
  return NULL;
}
