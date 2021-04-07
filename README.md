# Tiny HTTPd

> Fork from [Tiny HTTPd](http://tinyhttpd.sourceforge.net/)

## MDN HTTP

* [Tiny HTTPd README](README)
* [HTTP Headers](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers)
* [常见 MIME 类型列表](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types)
* [HTTP 响应代码](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Status)

## make & run

```bash
make clean && make && rm httpd_debug.log && ./httpd
```

## debug

```log
===[1617782460][DEBUG]=== main(httpd.c:791) httpd running on port 56113
===[1617782470][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [GET / HTTP/1.1
], len[15]
===[1617782470][DEBUG]=== httpd_accept_request(httpd.c:639) method = [GET]
===[1617782470][DEBUG]=== httpd_accept_request(httpd.c:666) url = [/]
===[1617782470][DEBUG]=== httpd_accept_request(httpd.c:690) path = [/tmp/www/htdocs/index.html]
===[1617782470][DEBUG]=== httpd_accept_request(httpd.c:723) httpd_serve_file [/tmp/www/htdocs/index.html]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Host: 127.0.0.1:56113
], len[22]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:78.0) Gecko/20100101 Firefox/78.0
], len[91]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
], len[83]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Accept-Language: zh-CN,en-US;q=0.5
], len[35]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Accept-Encoding: gzip, deflate
], len[31]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [DNT: 1
], len[7]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Connection: keep-alive
], len[23]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Upgrade-Insecure-Requests: 1
], len[29]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Pragma: no-cache
], len[17]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Cache-Control: no-cache
], len[24]
===[1617782470][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [
], len[1]
===[1617782470][DEBUG]=== httpd_status_code(httpd.c:259) code = [200], status = [200 OK]
===[1617782470][DEBUG]=== httpd_headers(httpd.c:307) send buf = [HTTP/1.0 200 OK
Server: Tiny Httpd/0.2.0
Cache-control: no-cache
Pragma: no-cache
Expires: 0
Content-Type: text/html

]
===[1617782470][DEBUG]=== httpd_cat_file(httpd.c:453) send buf len[218]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [GET /favicon.ico HTTP/1.1
], len[26]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:639) method = [GET]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:666) url = [/favicon.ico]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:690) path = [/tmp/www/htdocs/favicon.ico]
===[1617782471][ERROR]=== httpd_accept_request(httpd.c:694) [stat], errno[2], strerror[No such file or directory]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Host: 127.0.0.1:56113
], len[22]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:78.0) Gecko/20100101 Firefox/78.0
], len[91]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept: image/webp,*/*
], len[23]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept-Language: zh-CN,en-US;q=0.5
], len[35]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept-Encoding: gzip, deflate
], len[31]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [DNT: 1
], len[7]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Connection: keep-alive
], len[23]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Pragma: no-cache
], len[17]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Cache-Control: no-cache
], len[24]
===[1617782471][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [
], len[1]
===[1617782471][DEBUG]=== httpd_status_code(httpd.c:259) code = [404], status = [404 Not Found]
===[1617782471][DEBUG]=== httpd_headers(httpd.c:307) send buf = [HTTP/1.0 404 Not Found
Server: Tiny Httpd/0.2.0
Cache-control: no-cache
Pragma: no-cache
Expires: 0
Content-Type: text/html

<HTML>
<HEAD>
<TITLE>Not Found</TITLE>
</HEAD>
<BODY>
<H1>The server could not fulfill your request because the resource specified is unavailable or nonexistent.</H1>
</BODY>
</HTML>
]
===[1617782474][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [POST /color.cgi HTTP/1.1
], len[25]
===[1617782474][DEBUG]=== httpd_accept_request(httpd.c:639) method = [POST]
===[1617782474][DEBUG]=== httpd_accept_request(httpd.c:666) url = [/color.cgi]
===[1617782474][DEBUG]=== httpd_accept_request(httpd.c:690) path = [/tmp/www/htdocs/color.cgi]
===[1617782474][DEBUG]=== httpd_accept_request(httpd.c:717) cgi = [1]
===[1617782474][DEBUG]=== httpd_accept_request(httpd.c:728) httpd_execute_cgi [/tmp/www/htdocs/color.cgi] [POST] [(null)]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [GET /favicon.ico HTTP/1.1
], len[26]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:639) method = [GET]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:666) url = [/favicon.ico]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:690) path = [/tmp/www/htdocs/favicon.ico]
===[1617782475][ERROR]=== httpd_accept_request(httpd.c:694) [stat], errno[2], strerror[No such file or directory]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Host: 127.0.0.1:56113
], len[22]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:78.0) Gecko/20100101 Firefox/78.0
], len[91]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept: image/webp,*/*
], len[23]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept-Language: zh-CN,en-US;q=0.5
], len[35]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept-Encoding: gzip, deflate
], len[31]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [DNT: 1
], len[7]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Connection: keep-alive
], len[23]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Pragma: no-cache
], len[17]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Cache-Control: no-cache
], len[24]
===[1617782475][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [
], len[1]
===[1617782475][DEBUG]=== httpd_status_code(httpd.c:259) code = [404], status = [404 Not Found]
===[1617782475][DEBUG]=== httpd_headers(httpd.c:307) send buf = [HTTP/1.0 404 Not Found
Server: Tiny Httpd/0.2.0
Cache-control: no-cache
Pragma: no-cache
Expires: 0
Content-Type: text/html

<HTML>
<HEAD>
<TITLE>Not Found</TITLE>
</HEAD>
<BODY>
<H1>The server could not fulfill your request because the resource specified is unavailable or nonexistent.</H1>
</BODY>
</HTML>
]
===[1617782479][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [GET /check.cgi HTTP/1.1
], len[24]
===[1617782479][DEBUG]=== httpd_accept_request(httpd.c:639) method = [GET]
===[1617782479][DEBUG]=== httpd_accept_request(httpd.c:666) url = [/check.cgi]
===[1617782479][DEBUG]=== httpd_accept_request(httpd.c:690) path = [/tmp/www/htdocs/check.cgi]
===[1617782479][DEBUG]=== httpd_accept_request(httpd.c:717) cgi = [1]
===[1617782479][DEBUG]=== httpd_accept_request(httpd.c:728) httpd_execute_cgi [/tmp/www/htdocs/check.cgi] [GET] []
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [GET /favicon.ico HTTP/1.1
], len[26]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:639) method = [GET]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:666) url = [/favicon.ico]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:690) path = [/tmp/www/htdocs/favicon.ico]
===[1617782480][ERROR]=== httpd_accept_request(httpd.c:694) [stat], errno[2], strerror[No such file or directory]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Host: 127.0.0.1:56113
], len[22]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:78.0) Gecko/20100101 Firefox/78.0
], len[91]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept: image/webp,*/*
], len[23]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept-Language: zh-CN,en-US;q=0.5
], len[35]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept-Encoding: gzip, deflate
], len[31]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [DNT: 1
], len[7]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Connection: keep-alive
], len[23]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Pragma: no-cache
], len[17]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Cache-Control: no-cache
], len[24]
===[1617782480][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [
], len[1]
===[1617782480][DEBUG]=== httpd_status_code(httpd.c:259) code = [404], status = [404 Not Found]
===[1617782480][DEBUG]=== httpd_headers(httpd.c:307) send buf = [HTTP/1.0 404 Not Found
Server: Tiny Httpd/0.2.0
Cache-control: no-cache
Pragma: no-cache
Expires: 0
Content-Type: text/html

<HTML>
<HEAD>
<TITLE>Not Found</TITLE>
</HEAD>
<BODY>
<H1>The server could not fulfill your request because the resource specified is unavailable or nonexistent.</H1>
</BODY>
</HTML>
]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [GET /hello.py HTTP/1.1
], len[23]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:639) method = [GET]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:666) url = [/hello.py]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:690) path = [/tmp/www/htdocs/hello.py]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:717) cgi = [1]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:728) httpd_execute_cgi [/tmp/www/htdocs/hello.py] [GET] []
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [GET /favicon.ico HTTP/1.1
], len[26]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:639) method = [GET]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:666) url = [/favicon.ico]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:690) path = [/tmp/www/htdocs/favicon.ico]
===[1617782489][ERROR]=== httpd_accept_request(httpd.c:694) [stat], errno[2], strerror[No such file or directory]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Host: 127.0.0.1:56113
], len[22]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:78.0) Gecko/20100101 Firefox/78.0
], len[91]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept: image/webp,*/*
], len[23]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept-Language: zh-CN,en-US;q=0.5
], len[35]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept-Encoding: gzip, deflate
], len[31]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [DNT: 1
], len[7]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Connection: keep-alive
], len[23]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Pragma: no-cache
], len[17]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Cache-Control: no-cache
], len[24]
===[1617782489][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [
], len[1]
===[1617782489][DEBUG]=== httpd_status_code(httpd.c:259) code = [404], status = [404 Not Found]
===[1617782489][DEBUG]=== httpd_headers(httpd.c:307) send buf = [HTTP/1.0 404 Not Found
Server: Tiny Httpd/0.2.0
Cache-control: no-cache
Pragma: no-cache
Expires: 0
Content-Type: text/html

<HTML>
<HEAD>
<TITLE>Not Found</TITLE>
</HEAD>
<BODY>
<H1>The server could not fulfill your request because the resource specified is unavailable or nonexistent.</H1>
</BODY>
</HTML>
]
===[1617782502][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [GET /test0.jpg HTTP/1.1
], len[24]
===[1617782502][DEBUG]=== httpd_accept_request(httpd.c:639) method = [GET]
===[1617782502][DEBUG]=== httpd_accept_request(httpd.c:666) url = [/test0.jpg]
===[1617782502][DEBUG]=== httpd_accept_request(httpd.c:690) path = [/tmp/www/htdocs/test0.jpg]
===[1617782502][DEBUG]=== httpd_accept_request(httpd.c:723) httpd_serve_file [/tmp/www/htdocs/test0.jpg]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Host: 127.0.0.1:56113
], len[22]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:78.0) Gecko/20100101 Firefox/78.0
], len[91]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
], len[83]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Accept-Language: zh-CN,en-US;q=0.5
], len[35]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Accept-Encoding: gzip, deflate
], len[31]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [DNT: 1
], len[7]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Connection: keep-alive
], len[23]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Upgrade-Insecure-Requests: 1
], len[29]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Pragma: no-cache
], len[17]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [Cache-Control: no-cache
], len[24]
===[1617782502][DEBUG]=== httpd_serve_file(httpd.c:477) httpd_get_line [
], len[1]
===[1617782502][DEBUG]=== httpd_status_code(httpd.c:259) code = [200], status = [200 OK]
===[1617782502][DEBUG]=== httpd_headers(httpd.c:307) send buf = [HTTP/1.0 200 OK
Server: Tiny Httpd/0.2.0
Cache-control: no-cache
Pragma: no-cache
Expires: 0
Content-Type: image/jpeg

]
===[1617782502][DEBUG]=== httpd_cat_file(httpd.c:453) send buf len[1024]
===[1617782502][DEBUG]=== httpd_cat_file(httpd.c:453) send buf len[1024]
===[1617782525][DEBUG]=== httpd_cat_file(httpd.c:453) send buf len[1024]
===[1617782525][DEBUG]=== httpd_cat_file(httpd.c:453) send buf len[150]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [GET /favicon.ico HTTP/1.1
], len[26]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:639) method = [GET]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:666) url = [/favicon.ico]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:690) path = [/tmp/www/htdocs/favicon.ico]
===[1617782525][ERROR]=== httpd_accept_request(httpd.c:694) [stat], errno[2], strerror[No such file or directory]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Host: 127.0.0.1:56113
], len[22]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:78.0) Gecko/20100101 Firefox/78.0
], len[91]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept: image/webp,*/*
], len[23]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept-Language: zh-CN,en-US;q=0.5
], len[35]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Accept-Encoding: gzip, deflate
], len[31]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [DNT: 1
], len[7]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Connection: keep-alive
], len[23]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Pragma: no-cache
], len[17]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [Cache-Control: no-cache
], len[24]
===[1617782525][DEBUG]=== httpd_accept_request(httpd.c:698) httpd_get_line [
], len[1]
===[1617782525][DEBUG]=== httpd_status_code(httpd.c:259) code = [404], status = [404 Not Found]
===[1617782525][DEBUG]=== httpd_headers(httpd.c:307) send buf = [HTTP/1.0 404 Not Found
Server: Tiny Httpd/0.2.0
Cache-control: no-cache
Pragma: no-cache
Expires: 0
Content-Type: text/html

<HTML>
<HEAD>
<TITLE>Not Found</TITLE>
</HEAD>
<BODY>
<H1>The server could not fulfill your request because the resource specified is unavailable or nonexistent.</H1>
</BODY>
</HTML>
]
===[1617782529][DEBUG]=== httpd_accept_request(httpd.c:627) httpd_get_line [SET /favicon.ico HTTP/1.1
], len[26]
===[1617782529][DEBUG]=== httpd_accept_request(httpd.c:639) method = [SET]
===[1617782529][DEBUG]=== httpd_status_code(httpd.c:259) code = [501], status = [501 Not Implemented]
===[1617782529][DEBUG]=== httpd_headers(httpd.c:307) send buf = [HTTP/1.0 501 Not Implemented
Server: Tiny Httpd/0.2.0
Cache-control: no-cache
Pragma: no-cache
Expires: 0
Content-Type: text/html

<HTML>
<HEAD>
<TITLE>Method Not Implemented</TITLE>
</HEAD>
<BODY>
<H1>HTTP request method not supported.</H1>
</BODY>
</HTML>
]

```
