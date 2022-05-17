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

void login(int *sockfd) {
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

    message = compute_post_request(IP, AUTH_ROUTE, PAYLOAD_TYPE, form_data, 1, NULL, 1);

    send_to_server(*sockfd, message);
    response = receive_from_server(*sockfd);
    if(strlen(response) == 0)  // check if the connection is still active
        reopen_connection_and_send(sockfd, &response, message);

    puts(response);

    if(!check_respond(response))
        std::cout << "The credentials don't match the database" << "\n";
    else {
        std::cout << "Logged in" << "\n";
        // extract session cookie
        get_session_cookie(response, session_cookie);
        // marked the user as logged in
        logged_in = true;
        printf("%s\n", session_cookie);
    }

    /*
    // extract json and parse
    char *json_extract = basic_extract_json_response(response);

    puts(json_extract);
    json json_object = json::parse(json_extract);

    if(json_object.contains("error"))
    */

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
    
    message = compute_post_request(IP, REGISTER_ROUTE, PAYLOAD_TYPE, form_data, 1, NULL, 1);
    //puts(message);
    send_to_server(*sockfd, message);
    response = receive_from_server(*sockfd);

    if(strlen(response) == 0)  // check if the connection is still active
        reopen_connection_and_send(sockfd, &response, message);
    
    puts(response);

    if(!check_respond(response))
        std::cout << "The username is already taken" << "\n";
    else {
        std::cout << "Sign up successfuly" << "\n";
    }

    /*
    char *json_extract = basic_extract_json_response(response);
    puts(json_extract);
    json json_object = json::parse(json_extract);

    if(json_object.contains("error")) // check for error message
        std::cout << "Username is already taken" << "\n";
    else
        std::cout << "Account created succesfully" << "\n";
    */


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

    puts(response);
    if(!check_respond(response)) {
        std::cout << "You are not logged in\n";
       
    }
    else {
        char *token = NULL;
        std::cout << "Entered the library succesfully with Token:\n" << "" << "\n";
    }

    free(cookies);
    free(message);
    free(response);
}

void get_books(int sockfd) {

}

void get_book(int sockfd) {

}

void add_book(int sockfd) {

}

void delete_book(int sockfd) {

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

    puts(response);
    if(!check_respond(response))
        std::cout << "You are not logged in\n";
    else {
        std::cout << "Logged out succesfully\n";
    }

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
            get_books(sockfd);
        else if(strcmp(command, GET_BOOK_COMMAND) == 0)
            get_book(sockfd);
        else if(strcmp(command, ADD_BOOK_COMMAND) == 0)
            add_book(sockfd);
        else if(strcmp(command, DELETE_BOOK_COMMAND) == 0)
            delete_book(sockfd);
        else if(strcmp(command, LOGOUT_COMMAND) == 0)
            logout(&sockfd);

        std::cin.getline(command, MAX_COMMAND_LEN);
    }
    
    /*
    char **form_data = (char **) calloc(2, sizeof(char *));
    for (int i = 0; i < 2; i++)
    {
        form_data[i] = (char *) calloc(LINELEN, sizeof(char));
    }
    sprintf(form_data[0], "username=%s", "student");
    sprintf(form_data[1], "password=%s", "student");

    message = compute_post_request(IP, "/api/v1/auth/login", "application/x-www-form-urlencoded", form_data, 2, NULL, 1);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    test();
    */


    // // Ex 3: GET weather key
    // char **cookies = (char **) calloc(1, sizeof(char *));
    // cookies[0] = (char *) calloc(LINELEN, sizeof(char));

    // strcpy(cookies[0], "connect.sid=s%3Aoy5V6CLDYPGv8SqYq6yzvXL-BpCKiCC1.BEK0tJsEWKyuecGp7ADt%2FNheOkMF7Ov3EcngkQmgHnE");
    // message = compute_get_request("37.139.20.5", "/api/v1/weather/key", NULL, cookies, 1);
    // puts(message);
    // send_to_server(sockfd, message);
    // response = receive_from_server(sockfd);
    // puts(response);
    // close(sockfd);

    // // Ex 4: GET weather data from OpenWeather API
    // sockfd = open_connection("37.139.20.5", 80, AF_INET, SOCK_STREAM, 0);
    // message = compute_get_request("api.openweathermap.org", "/data/2.5/weather", "lat=44.7398&lon=22.2767&appid=b912dd495585fbf756dc6d8f415a7649", NULL, 0);
    // puts(message);
    // send_to_server(sockfd, message);
    // response = receive_from_server(sockfd);
    // puts(response);
    // close(sockfd);

    // Ex 5: POST weather data for verification
    // sockfd = open_connection("127.0.0.1", 8080, AF_INET, SOCK_STREAM, 0);
    // char **json_data = calloc(1, sizeof(char *));
    // json_data[0] = calloc(LINELEN, sizeof(char));
    // strcpy(json_data[0], basic_extract_json_response(response));
    // message = compute_post_request("localhost", "/api/v1/weather/44.7398/22.2767", "application/json", json_data, 1, cookies, 1);
    // puts(message);
    // send_to_server(sockfd, message);
    // response = receive_from_server(sockfd);
    // puts(response);
    // close(sockfd);

    // Ex 6: Logout
    
    // if(message != NULL)
    //     free(message);
    // if(response != NULL)
    //     free(response);

    free(session_cookie);

    return 0;
}
