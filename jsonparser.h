#ifndef _JSONPARSER_H
#define _JSONPARSER_H 1

#include <nlohmann/json.hpp>
#include <iostream>
#include "helpers.h"

using json = nlohmann::json;

char* create_json_user(char *username, char *password);

#endif