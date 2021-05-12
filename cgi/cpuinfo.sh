#!/bin/sh

PWD=/tmp/www/htdocs
. ${PWD}/json.sh

CPU_MODELNAME=$(cat /proc/cpuinfo | grep "model name" | sort -u | awk -F ': ' '{print $NF}')
CPU_CORES=$(cat /proc/cpuinfo | grep "cpu cores" | sort -u | awk -F ': ' '{print $NF}')
CPU_PROCESSORS=$(cat /proc/cpuinfo | grep "processor" | wc -l)

echo -ne "HTTP/1.0 200 OK\n"
echo -ne "Content-Type: application/json\n\n"

json_init

json_start_object # root {

json_start_object "result" true

json_start_object "cpuinfo"
json_add_string "model" "${CPU_MODELNAME}"
json_add_raw "cores" ${CPU_CORES}
json_add_raw "processors" ${CPU_PROCESSORS} true
json_end_object  # cpuinfo }

json_start_object "request"
method=${REQUEST_METHOD}
json_add_string "method" "${method}"
if [[ "$method" == "GET" ]]; then
    json_add_string "query" "${QUERY_STRING}" true
else
    length=${CONTENT_LENGTH}
    if [[ $length -gt 0 ]]; then
        json_add_raw "length" ${length}
        read -n${length} content
        json_add_raw "content" "${content}"
    fi
    json_add_raw "length" ${length} true
fi
json_end_object true # request }

json_end_object true # result }

json_end_object true # root }

json_dump