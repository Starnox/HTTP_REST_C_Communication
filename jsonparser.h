#ifndef _JSONPARSER_H
#define _JSONPARSER_H 1

#include <nlohmann/json.hpp>
#include <iostream>
#include "helpers.h"

using json = nlohmann::json;

char* create_json_user(char *username, char *password);

char* create_json_book(char *title, char *author, char *genre, char *publisher,
                        int page_count);

void get_token(char *json_string, char *buffer);

void parse_books_and_print(char *json_string);

#endif