/*
 * uhttpd - Tiny single-threaded httpd
 *
 *   Copyright (C) 2010-2013 Jo-Philipp Wich <xm@subsignal.org>
 *   Copyright (C) 2013 Felix Fietkau <nbd@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _MIMETYPES_
#define _MIMETYPES_

struct mimetype
{
    const char *extn;
    const char *mime;
};

#define MINE_TYPE_CGI "application/cgi"
#define MINE_TYPE_DEFAULT "text/html"

// see https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
static const struct mimetype MINE_TYPES[] = {

    {"txt", "text/plain"},
    {"log", "text/plain"},
    {"js", "text/javascript"},
    {"css", "text/css"},
    {"htm", "text/html"},
    {"html", "text/html"},
    {"diff", "text/x-patch"},
    {"patch", "text/x-patch"},
    {"c", "text/x-csrc"},
    {"h", "text/x-chdr"},
    {"o", "text/x-object"},
    {"ko", "text/x-object"},

    {"bmp", "image/bmp"},
    {"gif", "image/gif"},
    {"png", "image/png"},
    {"jpg", "image/jpeg"},
    {"jpeg", "image/jpeg"},
    {"svg", "image/svg+xml"},

    {"json", "application/json"},
    {"jsonp", "application/javascript"},
    {"zip", "application/zip"},
    {"pdf", "application/pdf"},
    {"xml", "application/xml"},
    {"xsl", "application/xml"},
    {"doc", "application/msword"},
    {"ppt", "application/vnd.ms-powerpoint"},
    {"xls", "application/vnd.ms-excel"},
    {"odt", "application/vnd.oasis.opendocument.text"},
    {"odp", "application/vnd.oasis.opendocument.presentation"},
    {"pl", "application/x-perl"},
    {"sh", "application/x-shellscript"},
    {"php", "application/x-php"},
    {"deb", "application/x-deb"},
    {"iso", "application/x-cd-image"},
    {"tar.gz", "application/x-compressed-tar"},
    {"tgz", "application/x-compressed-tar"},
    {"gz", "application/x-gzip"},
    {"tar.bz2", "application/x-bzip-compressed-tar"},
    {"tbz", "application/x-bzip-compressed-tar"},
    {"bz2", "application/x-bzip"},
    {"tar", "application/x-tar"},
    {"rar", "application/x-rar-compressed"},

    {"mp3", "audio/mpeg"},
    {"ogg", "audio/x-vorbis+ogg"},
    {"wav", "audio/x-wav"},

    {"mpg", "video/mpeg"},
    {"mpeg", "video/mpeg"},
    {"avi", "video/x-msvideo"},

    {"README", "text/plain"},
    {"log", "text/plain"},
    {"cfg", "text/plain"},
    {"conf", "text/plain"},

    {"pac", "application/x-ns-proxy-autoconfig"},
    {"wpad.dat", "application/x-ns-proxy-autoconfig"},

    {"cgi", MINE_TYPE_CGI},
    {"py", MINE_TYPE_CGI},
    {"lua", MINE_TYPE_CGI},
    {"sh", MINE_TYPE_CGI},

    {NULL, NULL}};

#endif
