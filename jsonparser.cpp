#include "jsonparser.h"

char* create_json_user(char *username, char *password) {
    json user;
    user["username"] = username;
    user["password"] = password;

    std::string json_string = user.dump(4);
    char *return_string = (char *) calloc(json_string.length() + 1, sizeof(char));
    std::strcpy(return_string, json_string.c_str());
    return return_string;
}