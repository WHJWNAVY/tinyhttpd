/* J. David's webserver */
/* This is a simple webserver.
 * Created November 1999 by J. David Blackstone.
 * CSE 4344 (Network concepts), Prof. Zeigler
 * University of Texas at Arlington
 */
/* This program compiles for Sparc Solaris 2.6.
 * To compile for Linux:
 *  1) Comment out the #include <pthread.h> line.
 *  2) Comment out the line that defines the variable newthread.
 *  3) Comment out the two lines that run pthread_create().
 *  4) Uncomment the line that runs accept_request().
 *  5) Remove -lsocket from the Makefile.
 */
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "debug.h"

#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"

#define SERVER_BASEDIR "/tmp/www/"
#define SERVER_BUFF_SZ 1024
#define SERVER_PORT 0 // 0 FOR RANDOM

void accept_request(int);
void bad_request(int);
void cat_file(int, FILE *);
void cannot_execute(int);
void error_die(const char *);
void execute_cgi(int, const char *, const char *, const char *);
int get_line(int, char *, int);
void headers(int, const char *);
void not_found(int);
void serve_file(int, const char *);
int startup(u_short *);
void unimplemented(int);

typedef enum mime_e
{
    MIME_NULL = 0,
    MIME_TEXT_PLAN,       // text/plain, .text, .txt
    MIME_TEXT_HTML,       // text/html, .htm, .html
    MIME_TEXT_CSS,        // text/css, .css
    MIME_TEXT_JAVASCRIPT, // text/javascript, .js
    MIME_APP_JSON,        // application/json, .json
    MIME_IMAGE_JPEG,      // image/jpeg, .jpeg, .jpg
    MIME_IMAGE_BMP,       // image/bmp, .bmp
    MIME_IMAGE_PNG,       // image/png, .png
    MIME_IMAGE_GIF,       // image/gif, .gif
    MIME_CGI,             // CGI, .cgi
    MIME_MAX,
} mime_t;

const char *file_suffix(const char *fspec)
{
    char *ext = NULL;
    char *et = (char *)fspec;
    while (((et = strrchr(et, '.')) != NULL) && (strlen(et) > 1))
    {
        et += 1;
        ext = et;
        //et = strrchr(et, '.');
    }

    return ext;
}

mime_t mime_types(const char *fspec)
{
    mime_t m = MIME_NULL;
    char *fext = file_suffix(fspec);
    if (fext != NULL)
    {
        if (strcasecmp(fext, "text") == 0)
        {
            m = MIME_TEXT_PLAN;
        }
        else if (strcasecmp(fext, "txt") == 0)
        {
            m = MIME_TEXT_PLAN;
        }
        else if (strcasecmp(fext, "html") == 0)
        {
            m = MIME_TEXT_HTML;
        }
        else if (strcasecmp(fext, "htm") == 0)
        {
            m = MIME_TEXT_HTML;
        }
        else if (strcasecmp(fext, "css") == 0)
        {
            m = MIME_TEXT_CSS;
        }
        else if (strcasecmp(fext, "json") == 0)
        {
            m = MIME_APP_JSON;
        }
        else if (strcasecmp(fext, "js") == 0)
        {
            m = MIME_TEXT_JAVASCRIPT;
        }
        else if (strcasecmp(fext, "jpeg") == 0)
        {
            m = MIME_IMAGE_JPEG;
        }
        else if (strcasecmp(fext, "jpg") == 0)
        {
            m = MIME_IMAGE_JPEG;
        }
        else if (strcasecmp(fext, "bmp") == 0)
        {
            m = MIME_IMAGE_BMP;
        }
        else if (strcasecmp(fext, "png") == 0)
        {
            m = MIME_IMAGE_PNG;
        }
        else if (strcasecmp(fext, "gif") == 0)
        {
            m = MIME_IMAGE_GIF;
        }
        else if (strcasecmp(fext, "cgi") == 0)
        {
            m = MIME_CGI;
        }
        else if (strcasecmp(fext, "py") == 0)
        {
            m = MIME_CGI;
        }
        else if (strcasecmp(fext, "lua") == 0)
        {
            m = MIME_CGI;
        }
        else if (strcasecmp(fext, "sh") == 0)
        {
            m = MIME_CGI;
        }
        else
        {
            m = MIME_NULL;
        }
    }

    return m;
}

/**********************************************************************/
/* A request has caused a call to accept() on the server port to
 * return.  Process the request appropriately.
 * Parameters: the socket connected to the client */
/**********************************************************************/
void accept_request(int client)
{
    char buf[SERVER_BUFF_SZ] = {0};
    int numchars = 0;
    char method[255] = {0};
    char url[255] = {0};
    char path[512] = {0};
    size_t i = 0, j = 0;
    struct stat st = {0};
    int cgi = 0; /* becomes true if server decides this is a CGI
                    * program */
    char *query_string = NULL;
    mime_t mime = MIME_NULL;

    numchars = get_line(client, buf, sizeof(buf));

    HTTPD_DEBUG("get_line [%s], len[%d]", buf, numchars);

    i = 0;
    j = 0;
    while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
    {
        method[i] = buf[j];
        i++;
        j++;
    }
    method[i] = '\0';

    HTTPD_DEBUG("method = [%s]", method);

    if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
    {
        unimplemented(client);
        return;
    }

    if (strcasecmp(method, "POST") == 0)
    {
        cgi = 1;
    }

    i = 0;
    while (ISspace(buf[j]) && (j < sizeof(buf)))
    {
        j++;
    }
    while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
    {
        url[i] = buf[j];
        i++;
        j++;
    }
    url[i] = '\0';

    HTTPD_DEBUG("url = [%s]", url);

    if (strcasecmp(method, "GET") == 0)
    {
        query_string = url;
        while ((*query_string != '?') && (*query_string != '\0'))
        {
            query_string++;
        }
        if (*query_string == '?')
        {
            cgi = 1;
            *query_string = '\0';
            query_string++;
            HTTPD_DEBUG("query_string = [%s]", query_string);
        }
    }

    sprintf(path, "%shtdocs%s", SERVER_BASEDIR, url);
    if (path[strlen(path) - 1] == '/')
    {
        strcat(path, "index.html");
    }

    HTTPD_DEBUG("path = [%s]", path);

    if (stat(path, &st) == -1)
    {
        DEBUG_PERROR("stat");
        while ((numchars > 0) && strcmp("\n", buf))
        { /* read & discard headers */
            numchars = get_line(client, buf, sizeof(buf));
            HTTPD_DEBUG("get_line [%s], len[%d]", buf, numchars);
        }
        not_found(client);
    }
    else
    {
        if ((st.st_mode & S_IFMT) == S_IFDIR)
        {
            strcat(path, "/index.html");
            HTTPD_DEBUG("path = [%s]", path);
        }

        if (mime_types(path) == MIME_CGI)
        {
            if ((st.st_mode & S_IXUSR) ||
                (st.st_mode & S_IXGRP) ||
                (st.st_mode & S_IXOTH))
            {
                cgi = 1;
                HTTPD_DEBUG("cgi = [%d]", cgi);
            }
        }

        if (!cgi)
        {
            HTTPD_DEBUG("serve_file [%s]", path);
            serve_file(client, path);
        }
        else
        {
            HTTPD_DEBUG("execute_cgi [%s] [%s] [%s]", path, method, query_string);
            execute_cgi(client, path, method, query_string);
        }
    }

    close(client);
}

/**********************************************************************/
/* Inform the client that a request it has made has a problem.
 * Parameters: client socket */
/**********************************************************************/
void bad_request(int client)
{
    char buf[SERVER_BUFF_SZ] = {0};

    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
    HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "<P>Your browser sent a bad request, ");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, sizeof(buf), 0);
}

/**********************************************************************/
/* Put the entire contents of a file out on a socket.  This function
 * is named after the UNIX "cat" command, because it might have been
 * easier just to do something like pipe, fork, and exec("cat").
 * Parameters: the client socket descriptor
 *             FILE pointer for the file to cat */
/**********************************************************************/
void cat_file(int client, FILE *resource)
{
    size_t count = 0;
    char buf[SERVER_BUFF_SZ] = {0};

#if 1
    while (!feof(resource))
    {
        count = fread(buf, 1, sizeof(buf), resource);
        HTTPD_DEBUG("send buf len[%d]", count);
        send(client, buf, count, 0);
        bzero(buf, sizeof(buf));
    }
#else
    fgets(buf, sizeof(buf), resource);
    while (!feof(resource))
    {
        HTTPD_DEBUG("send buf [%s]", buf);
        send(client, buf, strlen(buf), 0);
        fgets(buf, sizeof(buf), resource);
    }
#endif
}

/**********************************************************************/
/* Inform the client that a CGI script could not be executed.
 * Parameter: the client socket descriptor. */
/**********************************************************************/
void cannot_execute(int client)
{
    char buf[SERVER_BUFF_SZ] = {0};

    sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
    HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Print out an error message with perror() (for system errors; based
 * on value of errno, which indicates system call errors) and exit the
 * program indicating an error. */
/**********************************************************************/
void error_die(const char *sc)
{
    perror(sc);
    DEBUG_PERROR(sc);
    exit(1);
}

/**********************************************************************/
/* Execute a CGI script.  Will need to set environment variables as
 * appropriate.
 * Parameters: client socket descriptor
 *             path to the CGI script */
/**********************************************************************/
void execute_cgi(int client, const char *path,
                 const char *method, const char *query_string)
{
    char buf[SERVER_BUFF_SZ] = {0};
    int cgi_output[2] = {0};
    int cgi_input[2] = {0};
    pid_t pid = 0;
    int status = 0;
    int i = 0;
    char c = 0;
    int numchars = 1;
    int content_length = -1;

    buf[0] = 'A';
    buf[1] = '\0';
    if (strcasecmp(method, "GET") == 0)
    {
        while ((numchars > 0) && strcmp("\n", buf))
        { /* read & discard headers */
            numchars = get_line(client, buf, sizeof(buf));
        }
    }
    else /* POST */
    {
        numchars = get_line(client, buf, sizeof(buf));
        while ((numchars > 0) && strcmp("\n", buf))
        {
            buf[15] = '\0';
            if (strcasecmp(buf, "Content-Length:") == 0)
            {
                content_length = atoi(&(buf[16]));
            }
            numchars = get_line(client, buf, sizeof(buf));
        }
        if (content_length == -1)
        {
            bad_request(client);
            return;
        }
    }

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);

    if (pipe(cgi_output) < 0)
    {
        cannot_execute(client);
        return;
    }
    if (pipe(cgi_input) < 0)
    {
        cannot_execute(client);
        return;
    }

    if ((pid = fork()) < 0)
    {
        cannot_execute(client);
        return;
    }
    if (pid == 0) /* child: CGI script */
    {
        char meth_env[255]={0};
        char query_env[255]={0};
        char length_env[255]={0};

        dup2(cgi_output[1], 1);
        dup2(cgi_input[0], 0);
        close(cgi_output[0]);
        close(cgi_input[1]);
        sprintf(meth_env, "REQUEST_METHOD=%s", method);
        putenv(meth_env);
        if (strcasecmp(method, "GET") == 0)
        {
            sprintf(query_env, "QUERY_STRING=%s", query_string);
            putenv(query_env);
        }
        else
        { /* POST */
            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
            putenv(length_env);
        }
        execl(path, path, NULL);
        exit(0);
    }
    else
    { /* parent */
        close(cgi_output[1]);
        close(cgi_input[0]);
        if (strcasecmp(method, "POST") == 0)
        {
            for (i = 0; i < content_length; i++)
            {
                recv(client, &c, 1, 0);
                write(cgi_input[1], &c, 1);
            }
        }
        while (read(cgi_output[0], &c, 1) > 0)
        {
            send(client, &c, 1, 0);
        }

        close(cgi_output[0]);
        close(cgi_input[1]);
        waitpid(pid, &status, 0);
    }
}

/**********************************************************************/
/* Get a line from a socket, whether the line ends in a newline,
 * carriage return, or a CRLF combination.  Terminates the string read
 * with a null character.  If no newline indicator is found before the
 * end of the buffer, the string is terminated with a null.  If any of
 * the above three line terminators is read, the last character of the
 * string will be a linefeed and the string will be terminated with a
 * null character.
 * Parameters: the socket descriptor
 *             the buffer to save the data in
 *             the size of the buffer
 * Returns: the number of bytes stored (excluding null) */
/**********************************************************************/
int get_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n = 0;

    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        /* DEBUG printf("%02X\n", c); */
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                if ((n > 0) && (c == '\n'))
                {
                    recv(sock, &c, 1, 0);
                }
                else
                {
                    c = '\n';
                }
            }
            buf[i] = c;
            i++;
        }
        else
        {
            c = '\n';
        }
    }
    buf[i] = '\0';

    return (i);
}

/**********************************************************************/
/* Return the informational HTTP headers about a file. */
/* Parameters: the socket to print the headers on
 *             the name of the file */
/**********************************************************************/
void headers(int client, const char *filename)
{
    char buf[SERVER_BUFF_SZ] = {0};
    mime_t mime = MIME_NULL;
    char *content_type = NULL;
    mime = mime_types(filename); /* could use filename to determine file type */
    switch (mime)
    {
    case MIME_TEXT_HTML:
        content_type = "text/html";
        break;
    case MIME_TEXT_CSS:
        content_type = "text/css";
        break;
    case MIME_TEXT_JAVASCRIPT:
        content_type = "text/javascript";
        break;
    case MIME_APP_JSON:
        content_type = "application/json";
        break;
    case MIME_IMAGE_JPEG:
        content_type = "image/jpeg";
        break;
    case MIME_IMAGE_BMP:
        content_type = "image/bmp";
        break;
    case MIME_IMAGE_PNG:
        content_type = "image/png";
        break;
    case MIME_IMAGE_GIF:
        content_type = "image/gif";
        break;
    default:
        content_type = "text/plain";
        break;
    }

    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    strcpy(buf, SERVER_STRING);
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: %s\r\n", content_type);
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Give a client a 404 not found status message. */
/**********************************************************************/
void not_found(int client)
{
    char buf[SERVER_BUFF_SZ] = {0};

    sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
    HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "your request because the resource specified\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "is unavailable or nonexistent.\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
}

/**********************************************************************/
/* Send a regular file to the client.  Use headers, and report
 * errors to client if they occur.
 * Parameters: a pointer to a file structure produced from the socket
 *              file descriptor
 *             the name of the file to serve */
/**********************************************************************/
void serve_file(int client, const char *filename)
{
    FILE *resource = NULL;
    int numchars = 1;
    char buf[SERVER_BUFF_SZ] = {0};

    buf[0] = 'A';
    buf[1] = '\0';
    while ((numchars > 0) && strcmp("\n", buf))
    { /* read & discard headers */
        numchars = get_line(client, buf, sizeof(buf));
        HTTPD_DEBUG("get_line [%s], len[%d]", buf, numchars);
    }

    resource = fopen(filename, "rb");
    if (resource == NULL)
    {
        HTTPD_DEBUG("file [%s] not found", filename);
        not_found(client);
    }
    else
    {
        headers(client, filename);
        cat_file(client, resource);
    }
    fclose(resource);
}

/**********************************************************************/
/* This function starts the process of listening for web connections
 * on a specified port.  If the port is 0, then dynamically allocate a
 * port and modify the original port variable to reflect the actual
 * port.
 * Parameters: pointer to variable containing the port to connect on
 * Returns: the socket */
/**********************************************************************/
int startup(u_short *port)
{
    int httpd = 0;
    struct sockaddr_in name = {0};

    httpd = socket(PF_INET, SOCK_STREAM, 0);
    if (httpd == -1)
    {
        error_die("socket");
    }
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
    {
        error_die("bind");
    }
    if (*port == 0) /* if dynamically allocating a port */
    {
        int namelen = sizeof(name);
        if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
        {
            error_die("getsockname");
        }
        *port = ntohs(name.sin_port);
    }
    if (listen(httpd, 5) < 0)
    {
        error_die("listen");
    }
    return (httpd);
}

/**********************************************************************/
/* Inform the client that the requested web method has not been
 * implemented.
 * Parameter: the client socket */
/**********************************************************************/
void unimplemented(int client)
{
    char buf[SERVER_BUFF_SZ] = {0};

    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    //HTTPD_DEBUG("send buf = [%s]", buf);
    send(client, buf, strlen(buf), 0);
}

/**********************************************************************/

int main(void)
{
    int server_sock = -1;
    u_short port = SERVER_PORT;
    int client_sock = -1;
    struct sockaddr_in client_name = {0};
    int client_name_len = sizeof(client_name);
    pthread_t newthread;

    server_sock = startup(&port);
    printf("httpd running on port %d\n", port);

    while (1)
    {
        client_sock = accept(server_sock,
                             (struct sockaddr *)&client_name,
                             &client_name_len);
        if (client_sock == -1)
        {
            error_die("accept");
        }
        /* accept_request(client_sock); */
        if (pthread_create(&newthread, NULL, accept_request, client_sock) != 0)
        {
            perror("pthread_create");
            DEBUG_PERROR("pthread_create");
        }
    }

    close(server_sock);

    return (0);
}