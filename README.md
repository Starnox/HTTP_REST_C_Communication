# Communication Protocols
### Student: Mihailescu Eduard-Florin 322CB

## Overview
This code is meant to solve the third challenge given by the Communication Protocols team
as part of the curriculum studying at Faculty of Automatic Control and Computers 
in Bucharest. 
As part of the challenge, I implemented a command line client that connects to a given server
and interacts with a REST API using HTTP requests constructed using C sockets.

The code solves all of the tasks given by the team and handles all edge cases and errors that might occur.
Also, I began writing my client using the solution to lab 10. 

For JSON parsing, I opted for https://github.com/nlohmann/json because it's very flexible and has
a great documentation (examples, tutorials) and so I was able to quickly get a grasp on how to use it.
Also, I found it easy to import it in my project and really liked the fact that it acts as a STL container
because I have experience with them.

### Structure
-   client.cpp -> main logic of the program
-   helpers.cpp -> common functions used everywhere throught the program
-   requests.cpp -> code for generating HTTP requests
-   jsonparser.cpp -> code for parsing different types of json outputs
-   buffer.cpp -> code logic for packet creation
-   header files

### How it works
The program runs in a loop, listening for commands predefined(register, login,
enter_library, exit, logout, get_books, get_book, add_book, delete_book).
When it finds a command it recognizes, then the appropriate function is called.

For all of these functions, the logic is pretty much the same:
-   Create the request using `compute_get_request()`, `compute_post_request()` or `compute_delete_request()`, 
-   Send the request
-   Check if the connection is still active (TTL didn't expire)
-   If the request failed, reinitialise the connection, set `sockfd` and resend the request
-   Get the response and process it accordingly

### Other Notes
I store the session cookie and the JWT token in global variables to be easily accessible.

