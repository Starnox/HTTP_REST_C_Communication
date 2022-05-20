#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include <string>
#include "jsonparser.h"
#include <iostream>

bool logged_in;
char *session_cookie;
char *token;

void login(int *sockfd) {
    if(strlen(session_cookie) != 0) {
        std::cout << "You are already logged in!\n";
        return;
    }

    char *message, *response;
    char username[LINELEN];
    char password[LINELEN];

    // get the username
    std::cout << "username=";
    std::cin.getline(username, LINELEN);

    // get the password
    std::cout << "password=";
    std::cin.getline(password, LINELEN);

    // create post command
    char **form_data = (char **) calloc(1, sizeof(char *));
    form_data[0] = create_json_user(username, password);

    message = compute_post_request(IP, AUTH_ROUTE, PAYLOAD_TYPE, form_data, 1, NULL, 1, NULL);

    send_to_server(*sockfd, message);
    response = receive_from_server(*sockfd);
    if(strlen(response) == 0)  // check if the connection is still active
        reopen_connection_and_send(sockfd, &response, message);


    if(!check_respond(response))
        std::cout << "The credentials don't match the database" << "\n";
    else {
        std::cout << "Logged in" << "\n";
        // extract session cookie
        get_session_cookie(response, session_cookie);
        // marked the user as logged in
        logged_in = true;
    }

    free(message);
    free(response);
    free(form_data[0]);
    free(form_data);
}

void reg(int *sockfd) {
    char *message, *response;
    char username[LINELEN];
    char password[LINELEN];

    // get the username
    std::cout << "username=";
    std::cin.getline(username, LINELEN);

    // get the password
    std::cout << "password=";
    std::cin.getline(password, LINELEN);

    // create post command
    char **form_data = (char **) calloc(1, sizeof(char *));
    form_data[0] = create_json_user(username, password);
    
    message = compute_post_request(IP, REGISTER_ROUTE, PAYLOAD_TYPE, form_data, 1, NULL, 1, NULL);
    //puts(message);
    send_to_server(*sockfd, message);
    response = receive_from_server(*sockfd);

    if(strlen(response) == 0)  // check if the connection is still active
        reopen_connection_and_send(sockfd, &response, message);
    

    if(!check_respond(response))
        std::cout << "The username is already taken" << "\n";
    else {
        std::cout << "Sign up successfuly" << "\n";
    }

    free(message);
    free(response);
    free(form_data[0]);
    free(form_data);
}

void enter_library(int *sockfd) {
    char *message, *response;
    char **cookies = (char **) calloc(1,sizeof(char*));
    cookies[0] = session_cookie;
    message = compute_get_request(IP, LIBRARY_ROUTE ,NULL, cookies, 1, NULL);

    send_to_server(*sockfd, message);
    response = receive_from_server(*sockfd);

    if(strlen(response) == 0)  // check if the connection is still active
        reopen_connection_and_send(sockfd, &response, message);

    if(!check_respond(response)) {
        std::cout << "You are not logged in\n";
       
    }
    else {
        char *json_response = basic_extract_json_response(response);
        get_token(json_response, token);
        std::cout << "Entered the library succesfully with token: \n" << token << "\n";
    }

    free(cookies);
    free(message);
    free(response);
}

void get_books(int *sockfd) {
    if(strlen(token) == 0) {
        std::cout << "You don't have access to the library\n";
        return;
    }
    char *message, *response;
    message = compute_get_request(IP, BOOK_ROUTE ,NULL, NULL, 1, token);

    send_to_server(*sockfd, message);
    response = receive_from_server(*sockfd);

    if(strlen(response) == 0)  // check if the connection is still active
        reopen_connection_and_send(sockfd, &response, message);

    if(!check_respond(response)) {
        std::cout << "You don't have access to the library\n";
       
    }
    else {
        char *json_response = basic_extract_json_response(response);
        if(json_response != NULL)
            parse_books_and_print(json_response);
        else
            std::cout << "No books in library!\n";
    }

    free(message);
    free(response);
}

void get_book(int *sockfd) {
    if(strlen(token) == 0) {
        std::cout << "You don't have access to the library\n";
        return;
    }
    char id[LINELEN];
    int int_id;

    std::cout << "id=";
    std::cin.getline(id, LINELEN);

    int_id = atoi(id);
    if(int_id == 0) { // check id intregrity
        std::cout << "ID must be an integer\n";
        return;
    }

    char *message, *response;
    char book_route_id[LINELEN];
    sprintf(book_route_id, "%s/%d", BOOK_ROUTE, int_id);
    
    message = compute_get_request(IP, book_route_id ,NULL, NULL, 1, token);

    send_to_server(*sockfd, message);
    response = receive_from_server(*sockfd);

    if(strlen(response) == 0)  // check if the connection is still active
        reopen_connection_and_send(sockfd, &response, message);

    if(!check_respond(response)) {
        std::cout << "ID is not found in the database\n";
       
    }
    else {
        char *json_response = basic_extract_json_response(response);
        if(json_response != NULL)
            parse_books_and_print(json_response);
    }

    free(message);
    free(response);
}

void add_book(int *sockfd) {
    if(strlen(token) == 0) {
        std::cout << "You don't have access to the library\n";
        return;
    }
    char title[LINELEN];
    char author[LINELEN];
    char genre[LINELEN];
    char publisher[LINELEN];
    char page_count[LINELEN];

    // get the title
    std::cout << "title=";
    std::cin.getline(title, LINELEN);

    // get the author
    std::cout << "author=";
    std::cin.getline(author, LINELEN);

    // get the genre
    std::cout << "genre=";
    std::cin.getline(genre, LINELEN);

    // get the publisher
    std::cout << "publisher=";
    std::cin.getline(publisher, LINELEN);

    // get the page_count
    std::cout << "page_count=";
    std::cin.getline(page_count, LINELEN);

    int int_page_count = atoi(page_count);
    if(int_page_count == 0) {
        std::cout << "You need to enter an integer for the page count\n";
        return;
    }

    char *message = NULL, *response= NULL;

    // create post command
    char **form_data = (char **) calloc(1, sizeof(char *));
    form_data[0] = create_json_book(title, author, genre, publisher, int_page_count);
        
    message = compute_post_request(IP, ADD_BOOK_ROUTE, PAYLOAD_TYPE, form_data, 1, NULL, 1, token);
    send_to_server(*sockfd, message);
    response = receive_from_server(*sockfd);

    if(strlen(response) == 0)  // check if the connection is still active
        reopen_connection_and_send(sockfd, &response, message);
    

    if(!check_respond(response))
        std::cout << "Add book failed" << "\n";
    else {
        std::cout << "Book added succesfully" << "\n";
    }

    if(message != NULL )
        free(message);
    if(response != NULL)
        free(response);
    if(form_data[0] != NULL)
        free(form_data[0]);
    free(form_data);

}

void delete_book(int *sockfd) {
    if(strlen(token) == 0) {
        std::cout << "You don't have access to the library\n";
        return;
    }
    char id[LINELEN];
    int int_id;

    std::cout << "id=";
    std::cin.getline(id, LINELEN);

    int_id = atoi(id);
    if(int_id == 0) { // check id intregrity
        std::cout << "ID must be an integer\n";
        return;
    }

    char *message, *response;
    char book_route_id[LINELEN];
    sprintf(book_route_id, "%s/%d", BOOK_ROUTE, int_id);
    
    message = compute_delete_request(IP, book_route_id, NULL, 1, token);

    send_to_server(*sockfd, message);
    response = receive_from_server(*sockfd);

    if(strlen(response) == 0)  // check if the connection is still active
        reopen_connection_and_send(sockfd, &response, message);

    if(!check_respond(response)) {
        std::cout << "ID is not found in the database\n";
       
    }
    else {
        std::cout << "Book deleted succesfully\n";
    }

    free(message);
    free(response);
}

void logout(int *sockfd) {
    char *message, *response;
    char **cookies = (char **) calloc(1,sizeof(char*));
    cookies[0] = session_cookie;
    message = compute_get_request(IP, DEAUTH_ROUTE,NULL, cookies, 1, NULL);

    send_to_server(*sockfd, message);
    response = receive_from_server(*sockfd);

    if(strlen(response) == 0)  // check if the connection is still active
        reopen_connection_and_send(sockfd, &response, message);

    if(!check_respond(response))
        std::cout << "You are not logged in\n";
    else {
        std::cout << "Logged out succesfully\n";
    }

    memset(session_cookie, 0, SESSION_COOKIE_LEN);
    memset(token, 0, TOKEN_LEN);
    free(cookies);
    free(message);
    free(response);
}


int main()
{
    int sockfd;
    sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);

    // initialisation of global variables
    session_cookie = (char *) calloc(SESSION_COOKIE_LEN, sizeof(char));
    token = (char *) calloc(TOKEN_LEN, sizeof(char));
    logged_in = false;

    // Main loop of the program listen for commands while the command is not exit
    
    char command[LINELEN];
    std::cin.getline(command, MAX_COMMAND_LEN);
    while(strcmp(command, EXIT_COMMAND) != 0) {

        if(strcmp(command, LOGIN_COMMAND) == 0)
            login(&sockfd);
        else if(strcmp(command, REGISTER_COMMAND) == 0)
            reg(&sockfd);
        else if(strcmp(command, ENTER_LIBRARY_COMMAND) == 0)
            enter_library(&sockfd);
        else if(strcmp(command, GET_BOOKS_COMMAND) == 0)
            get_books(&sockfd);
        else if(strcmp(command, GET_BOOK_COMMAND) == 0)
            get_book(&sockfd);
        else if(strcmp(command, ADD_BOOK_COMMAND) == 0)
            add_book(&sockfd);
        else if(strcmp(command, DELETE_BOOK_COMMAND) == 0)
            delete_book(&sockfd);
        else if(strcmp(command, LOGOUT_COMMAND) == 0)
            logout(&sockfd);

        std::cin.getline(command, MAX_COMMAND_LEN);
    }

    free(session_cookie);
    free(token);

    return 0;
}
