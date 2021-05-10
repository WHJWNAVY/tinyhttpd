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
 *  4) Uncomment the line that runs httpd_accept_request().
 *  5) Remove -lsocket from the Makefile.
 */

//#define SERVER_MUTI_THREAD 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>

#ifdef SERVER_MUTI_THREAD
#include <pthread.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "statuscode.h"
#include "mimetypes.h"
#include "debug.h"

#define IS_SPACE(x) isspace((int)(x))

#define HTTPD_HEADERS "Server: Tiny Httpd/0.2.0\r\nCache-control: no-cache\r\nPragma: no-cache\r\nExpires: 0"

#define SERVER_INDEX "index.html"
#define SERVER_BASEDIR "/tmp/www/"
#define SERVER_BUFF_SZ 1024
#define SERVER_PORT 0 // 0 FOR RANDOM

const char *httpd_file_suffix(const char *filename)
{
    char *ext = NULL;
    char *et = (char *)filename;
    if ((filename == NULL) || (strlen(filename) <= 0))
    {
        return NULL;
    }

    while (((et = strrchr(et, '.')) != NULL) && (strlen(et) > 1))
    {
        et += 1;
        ext = et;
        //et = strrchr(et, '.');
    }

    return ext;
}

#define DEF_CONTENT_TYPE MINE_TYPE_DEFAULT
char *httpd_content_type(const char *filename)
{
    char *content_type = DEF_CONTENT_TYPE;
    const struct mimetype *m = &MINE_TYPES[0];
    char *fext = httpd_file_suffix(filename);
    HTTPD_DEBUG("filename = [%s], ext = [%s]", filename, fext);
    if (fext != NULL)
    {
        while (m->extn)
        {
            if (!strcasecmp(fext, m->extn))
            {
                content_type = m->mime;
                break;
            }
            m++;
        }
    }

    HTTPD_DEBUG("filename = [%s], content_type = [%s]",
                ((filename == NULL) ? "NULL" : filename), content_type);

    return content_type;
}

int is_mime_type_cgi(const char *filename)
{
    char *mime_type = httpd_content_type(filename);

    return (!strcasecmp(mime_type, MINE_TYPE_CGI));
}

#define DEF_STATUS_TYPE STATUS_CODE_DEFAULT
char *httpd_status_type(uint32_t status_code)
{
    char *status_type = DEF_STATUS_TYPE;
    const struct statuscode *sc = &STATUS_CODE[0];

    while (sc->code)
    {
        if (status_code == sc->code)
        {
            status_type = sc->status;
            break;
        }
        sc++;
    }

    HTTPD_DEBUG("code = [%d], status = [%s]", status_code, status_type);

    return status_type;
}

/**********************************************************************/
/* Return the informational HTTP httpd_headers about a file. */
/* Parameters: the socket to print the httpd_headers on
 *             the name of the file */
/**********************************************************************/
// see https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers
void httpd_headers(int client, int status_code, const char *content_type, const char *message)
{
    char buf[SERVER_BUFF_SZ] = {0};

    char *p_buf = buf;
    p_buf += sprintf(p_buf, "HTTP/1.0 %s\r\n", httpd_status_type(status_code));
    p_buf += sprintf(p_buf, "%s\r\n", HTTPD_HEADERS);
    p_buf += sprintf(p_buf, "Content-Type: %s\r\n\r\n", content_type);

    if ((message != NULL) && (strlen(message) > 0))
    {
        p_buf += sprintf(p_buf, "%s\r\n", message);
    }
    send(client, buf, strlen(buf), 0);
    HTTPD_DEBUG("send buf = [%s]", buf);
}

/**********************************************************************/
/* Inform the client that a request it has made has a problem.
 * Parameters: client socket */
/**********************************************************************/
void httpd_error_bad_request(int client)
{
    char buf[SERVER_BUFF_SZ] = {0};
    char *p_buf = buf;

    p_buf += sprintf(p_buf, "<HTML>\r\n<HEAD>\r\n<TITLE>Bad Request</TITLE>\r\n</HEAD>\r\n<BODY>\r\n");
    p_buf += sprintf(p_buf, "<H1>Your browser sent a bad request, such as a POST without a Content-Length.</H1>");
    p_buf += sprintf(p_buf, "\r\n</BODY>\r\n</HTML>");

    httpd_headers(client, 400, DEF_CONTENT_TYPE, buf);
}

/**********************************************************************/
/* Inform the client that a CGI script could not be executed.
 * Parameter: the client socket descriptor. */
/**********************************************************************/
void httpd_error_cannot_execute(int client)
{
    char buf[SERVER_BUFF_SZ] = {0};
    char *p_buf = buf;

    p_buf += sprintf(p_buf, "<HTML>\r\n<HEAD>\r\n<TITLE>Internal Server Error</TITLE>\r\n</HEAD>\r\n<BODY>\r\n");
    p_buf += sprintf(p_buf, "<H1>Error prohibited CGI execution.</H1>");
    p_buf += sprintf(p_buf, "\r\n</BODY>\r\n</HTML>");

    httpd_headers(client, 500, DEF_CONTENT_TYPE, buf);
}

/**********************************************************************/
/* Give a client a 404 not found status message. */
/**********************************************************************/
void httpd_error_not_found(int client)
{
    char buf[SERVER_BUFF_SZ] = {0};
    char *p_buf = buf;

    p_buf += sprintf(p_buf, "<HTML>\r\n<HEAD>\r\n<TITLE>Not Found</TITLE>\r\n</HEAD>\r\n<BODY>\r\n");
    p_buf += sprintf(p_buf, "<H1>The server could not fulfill your request because the resource specified is unavailable or nonexistent.</H1>");
    p_buf += sprintf(p_buf, "\r\n</BODY>\r\n</HTML>");

    httpd_headers(client, 404, DEF_CONTENT_TYPE, buf);
}

/**********************************************************************/
/* Inform the client that the requested web method has not been
 * implemented.
 * Parameter: the client socket */
/**********************************************************************/
void httpd_error_unimplemented(int client)
{
    char buf[SERVER_BUFF_SZ] = {0};
    char *p_buf = buf;

    p_buf += sprintf(p_buf, "<HTML>\r\n<HEAD>\r\n<TITLE>Method Not Implemented</TITLE>\r\n</HEAD>\r\n<BODY>\r\n");
    p_buf += sprintf(p_buf, "<H1>HTTP request method not supported.</H1>");
    p_buf += sprintf(p_buf, "\r\n</BODY>\r\n</HTML>");

    httpd_headers(client, 501, DEF_CONTENT_TYPE, buf);
}

/**********************************************************************/
/* Print out an error message with perror() (for system errors; based
 * on value of errno, which indicates system call errors) and exit the
 * program indicating an error. */
/**********************************************************************/
void httpd_error_die(const char *sc)
{
    perror(sc);
    DEBUG_PERROR(sc);
    exit(1);
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
int httpd_get_line(int sock, char *buf, int size)
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
/* Put the entire contents of a file out on a socket.  This function
 * is named after the UNIX "cat" command, because it might have been
 * easier just to do something like pipe, fork, and exec("cat").
 * Parameters: the client socket descriptor
 *             FILE pointer for the file to cat */
/**********************************************************************/
void httpd_cat_file(int client, FILE *resource)
{
    size_t count = 0;
    char buf[SERVER_BUFF_SZ] = {0};

    while (!feof(resource))
    {
        count = fread(buf, 1, sizeof(buf), resource);
        HTTPD_DEBUG("send buf len[%d]", count);
        send(client, buf, count, 0);
        bzero(buf, sizeof(buf));
    }
}

/**********************************************************************/
/* Send a regular file to the client.  Use httpd_headers, and report
 * errors to client if they occur.
 * Parameters: a pointer to a file structure produced from the socket
 *              file descriptor
 *             the name of the file to serve */
/**********************************************************************/
void httpd_serve_file(int client, const char *filename)
{
    FILE *resource = NULL;
    int numchars = 1;
    char buf[SERVER_BUFF_SZ] = {0};

    buf[0] = 'A';
    buf[1] = '\0';
    while ((numchars > 0) && strcmp("\n", buf))
    { /* read & discard httpd_headers */
        numchars = httpd_get_line(client, buf, sizeof(buf));
        //HTTPD_DEBUG("httpd_get_line [%s], len[%d]", buf, numchars);
    }

    resource = fopen(filename, "rb");
    if (resource == NULL)
    {
        HTTPD_DEBUG("file [%s] not found", filename);
        httpd_error_not_found(client);
    }
    else
    {
        httpd_headers(client, 200, httpd_content_type(filename), NULL);
        httpd_cat_file(client, resource);
    }
    fclose(resource);
}

/**********************************************************************/
/* Execute a CGI script.  Will need to set environment variables as
 * appropriate.
 * Parameters: client socket descriptor
 *             path to the CGI script */
/**********************************************************************/
void httpd_execute_cgi(int client, const char *path,
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
        { /* read & discard httpd_headers */
            numchars = httpd_get_line(client, buf, sizeof(buf));
        }
    }
    else /* POST */
    {
        numchars = httpd_get_line(client, buf, sizeof(buf));
        while ((numchars > 0) && strcmp("\n", buf))
        {
            buf[15] = '\0';
            if (strcasecmp(buf, "Content-Length:") == 0)
            {
                content_length = atoi(&(buf[16]));
            }
            numchars = httpd_get_line(client, buf, sizeof(buf));
        }
        if (content_length == -1)
        {
            httpd_error_bad_request(client);
            return;
        }
    }

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);

    if (pipe(cgi_output) < 0)
    {
        httpd_error_cannot_execute(client);
        return;
    }
    if (pipe(cgi_input) < 0)
    {
        httpd_error_cannot_execute(client);
        return;
    }

    if ((pid = fork()) < 0)
    {
        httpd_error_cannot_execute(client);
        return;
    }
    if (pid == 0) /* child: CGI script */
    {
        char meth_env[255] = {0};
        char query_env[255] = {0};
        char length_env[255] = {0};

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
/* A request has caused a call to accept() on the server port to
 * return.  Process the request appropriately.
 * Parameters: the socket connected to the client */
/**********************************************************************/
void httpd_accept_request(int client)
{
    char buf[SERVER_BUFF_SZ] = {0};
    int numchars = 0;
    char method[255] = {0};
    char url[255] = {0};
    char path[512] = {0};
    size_t i = 0, j = 0;
    struct stat st = {0};
    int cgi = 0; /* becomes true if server decides this is a CGI program */
    char *query_string = NULL;

    numchars = httpd_get_line(client, buf, sizeof(buf));

    //HTTPD_DEBUG("httpd_get_line [%s], len[%d]", buf, numchars);

    i = 0;
    j = 0;
    while (!IS_SPACE(buf[j]) && (i < sizeof(method) - 1))
    {
        method[i] = buf[j];
        i++;
        j++;
    }
    method[i] = '\0';

    HTTPD_DEBUG("method = [%s]", method);

    if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
    {
        httpd_error_unimplemented(client);
        close(client);
        return;
    }

    if (strcasecmp(method, "POST") == 0)
    {
        cgi = 1;
    }

    i = 0;
    while (IS_SPACE(buf[j]) && (j < sizeof(buf)))
    {
        j++;
    }
    while (!IS_SPACE(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
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
        strcat(path, SERVER_INDEX);
    }

    HTTPD_DEBUG("path = [%s]", path);

    if (stat(path, &st) == -1)
    {
        DEBUG_PERROR("stat");
        while ((numchars > 0) && strcmp("\n", buf))
        { /* read & discard httpd_headers */
            numchars = httpd_get_line(client, buf, sizeof(buf));
            //HTTPD_DEBUG("httpd_get_line [%s], len[%d]", buf, numchars);
        }
        httpd_error_not_found(client);
    }
    else
    {
        if ((st.st_mode & S_IFMT) == S_IFDIR)
        {
            strcat(path, "/" SERVER_INDEX);
            HTTPD_DEBUG("path = [%s]", path);
        }

        if (is_mime_type_cgi(path))
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
            HTTPD_DEBUG("httpd_serve_file [%s]", path);
            httpd_serve_file(client, path);
        }
        else
        {
            HTTPD_DEBUG("httpd_execute_cgi [%s] [%s] [%s]", path, method, query_string);
            httpd_execute_cgi(client, path, method, query_string);
        }
    }

    close(client);
}

/**********************************************************************/
/* This function starts the process of listening for web connections
 * on a specified port.  If the port is 0, then dynamically allocate a
 * port and modify the original port variable to reflect the actual
 * port.
 * Parameters: pointer to variable containing the port to connect on
 * Returns: the socket */
/**********************************************************************/
int httpd_startup(u_short *port)
{
    int httpd = 0;
    struct sockaddr_in name = {0};

    httpd = socket(PF_INET, SOCK_STREAM, 0);
    if (httpd < 0)
    {
        httpd_error_die("socket");
    }
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
    {
        httpd_error_die("bind");
    }
    if (*port == 0) /* if dynamically allocating a port */
    {
        int namelen = sizeof(name);
        if (getsockname(httpd, (struct sockaddr *)&name, &namelen) < 0)
        {
            httpd_error_die("getsockname");
        }
        *port = ntohs(name.sin_port);
    }
    if (listen(httpd, 5) < 0)
    {
        httpd_error_die("listen");
    }
    return (httpd);
}

/**********************************************************************/

int main(void)
{
    int server_sock = -1;
    u_short port = SERVER_PORT;
    int client_sock = -1;
    struct sockaddr_in client_name = {0};
    int client_name_len = sizeof(client_name);
#ifdef SERVER_MUTI_THREAD
    pthread_t newthread;
#endif

    server_sock = httpd_startup(&port);
    printf("httpd running on port %d\n", port);
    HTTPD_DEBUG("httpd running on port %d", port);

    while (1)
    {
        client_sock = accept(server_sock,
                             (struct sockaddr *)&client_name,
                             &client_name_len);
        if (client_sock < 0)
        {
            httpd_error_die("accept");
        }

#ifdef SERVER_MUTI_THREAD
        if (pthread_create(&newthread, NULL, httpd_accept_request, client_sock) != 0)
        {
            perror("pthread_create");
            DEBUG_PERROR("pthread_create");
        }
#else
        httpd_accept_request(client_sock);
#endif
    }

    close(server_sock);

    return (0);
}
