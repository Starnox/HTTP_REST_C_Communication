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

char* create_json_book(char *title, char *author, char *genre, char *publisher,
                        int page_count) {
    json book;
    book["title"] = title;
    book["author"] = author;
    book["genre"] = genre;
    book["publisher"] = publisher;
    book["page_count"] = page_count;


    std::string json_string = book.dump(4);
    char *return_string = (char *) calloc(json_string.length() + 1, sizeof(char));
    std::strcpy(return_string, json_string.c_str());
    return return_string;
}

void get_token(char *json_string, char *buffer){
    json json_object = json::parse(json_string);
    std::string res = json_object["token"];
    strcpy(buffer, res.c_str());
}

void parse_books_and_print(char *json_string) {
    json books;
    books = json::parse((json_string-1));
    std::cout << books.dump(4) << "\n";
}