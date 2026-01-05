#ifndef USER_H
#define USER_H

#include "../Utils/utils.h"

/* User Management Functions */
ErrorCode add_user(Library *lib, const char *name);
ErrorCode update_user(Library *lib, int user_id, const char *name);
ErrorCode delete_user(Library *lib, int user_id);
User *find_user_by_id(Library *lib, int user_id);

#endif /* USER_H */
