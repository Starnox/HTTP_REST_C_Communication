#ifndef _HELPERS_
#define _HELPERS_

#define BUFLEN 4096
#define LINELEN 1000
#define SESSION_COOKIE_LEN 200

#define IP "34.241.4.235"
#define PORT 8080
#define PAYLOAD_TYPE "application/json"
#define REGISTER_ROUTE "/api/v1/tema/auth/register"
#define AUTH_ROUTE " /api/v1/tema/auth/login"
#define LIBRARY_ROUTE " /api/v1/tema/library/access"
#define BOOK_ROUTE " /api/v1/tema/library/books/"
#define ADD_BOOK_ROUTE " /api/v1/tema/library/books"
#define DEAUTH_ROUTE  "/api/v1/tema/auth/logout"

#define MAX_COMMAND_LEN 20
#define LOGIN_COMMAND "login"
#define REGISTER_COMMAND "register"
#define ENTER_LIBRARY_COMMAND "enter_library"
#define GET_BOOKS_COMMAND "get_books"
#define GET_BOOK_COMMAND "get_book"
#define ADD_BOOK_COMMAND "add_book"
#define DELETE_BOOK_COMMAND "delete_book"
#define LOGOUT_COMMAND "logout"
#define EXIT_COMMAND "exit"

typedef struct user {
    char username[100];
    char password[100];
}user;

// reopens the connection and send the message again
void reopen_connection_and_send(int *sockfd, char **response, char *message);

// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(const char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

bool check_respond(char *response);

void get_session_cookie(char *response, char *buffer);

#endif
