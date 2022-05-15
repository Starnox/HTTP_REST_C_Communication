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

int main(int argc, char *argv[])
{
    char IP[30] = "34.241.4.235";

    char *message;
    char *response;
    int sockfd;
    // Ex 1.1: GET dummy
    // Ex 1.2: POST dummy and print response

    // Ex 2: Login

    sockfd = open_connection(IP, 8080, AF_INET, SOCK_STREAM, 0);

    char **form_data = (char **) calloc(2, sizeof(char *));
    for (int i = 0; i < 2; i++)
    {
        form_data[i] = (char *) calloc(LINELEN, sizeof(char));
    }
    strcpy(form_data[0], "username=student");
    strcpy(form_data[1], "password=student");

    message = compute_post_request(IP, "/api/v1/auth/login", "application/x-www-form-urlencoded", form_data, 2, NULL, 1);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    test();

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

    free(message);
    free(response);

    return 0;
}
