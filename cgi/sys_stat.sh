#!/bin/sh

PWD=/tmp/www/htdocs
. ${PWD}/json.sh

CPU_MODELNAME=$(cat /proc/cpuinfo | grep "model name" | sort -u | awk -F ': ' '{print $NF}')
CPU_CORES=$(cat /proc/cpuinfo | grep "cpu cores" | sort -u | awk -F ': ' '{print $NF}')
CPU_PROCESSORS=$(cat /proc/cpuinfo | grep "processor" | wc -l)

# $ cat /proc/stat
# <CPU指标:user nice system idle iowait irq softirq>
# cpu  221 0 4598 17335487 164 0 123 0 0 0
# cpu0 12 0 352 2167020 1 0 102 0 0 0
# cpu1 9 0 31 2167641 0 0 10 0 0 0
# cpu2 36 0 129 2167377 117 0 8 0 0 0
# cpu3 3 0 15 2167548 0 0 3 0 0 0
# cpu4 59 0 3170 2164138 23 0 0 0 0 0
# cpu5 20 0 21 2167605 19 0 0 0 0 0
# cpu6 47 0 821 2166566 1 0 0 0 0 0
# cpu7 35 0 59 2167590 0 0 0 0 0 0
CPU_LOADAVG=$(cat /proc/stat | head -n1 | awk '{total=0;idle=$5;for(i=2;i<=NF;i++){total+=$i}print((total-idle)/total)}')

# $ cat /proc/loadavg
# <1> 0.00 0.00 0.00 1/116 1192
# <2> 0.00: 1分钟平均负载
# <3> 0.00: 5分钟平均负载
# <4> 0.00: 15分钟平均负载
# <5> 1/116: 分子是当前正在运行的进程数,分母是总的进程数
# <6> 1192: 最近运行进程的ID
CPU_LOADPROCS=$(cat /proc/loadavg | awk 'BEGIN{FS="[/ ]+"} {printf("\"running\":%s,\"total\":%s", $4,$5)}')

# $ cat /proc/uptime
# 22289.66 178101.23
# 第一个参数是代表从系统启动到现在的时间(以秒为单位)
# 第二个参数是代表系统空闲的时间(以秒为单位)
CPU_UPTIME=$(cat /proc/uptime | awk '{print $1}')

# $ free
#               total        used        free      shared  buff/cache   available
# Mem:       12927948       89804    12757536          68       80608    12646692
# Swap:       4194304           0     4194304
MEM_LOADAVG=$(free | awk '/Mem/ {printf("{\"total\":%s,\"loadavg\":%s}\n", $2,($3/$2))}')
# {"total":12927948,"loadavg":0.0069465}

TOP_RESULT=$(top -bn1 | awk -f ${PWD}/top_result.awk)

result_content() {
    json_init

    json_start_object # root {

    json_start_object "result" true

    json_start_object "cpuinfo"
    json_add_string "model" "${CPU_MODELNAME}"
    json_add_int "cores" "${CPU_CORES}"
    json_add_int "processors" "${CPU_PROCESSORS}" true
    json_end_object  # cpuinfo }

    json_start_object "system"
    json_start_object "cpu"
    json_add_raw "" "${CPU_LOADPROCS}"
    json_add_int "loadavg" "${CPU_LOADAVG}" true
    json_end_object  # cpu }
    json_add_raw "memory" "${MEM_LOADAVG}"
    json_add_int "uptime" "${CPU_UPTIME}" true
    json_end_object  # system }

    json_add_raw "progress" "${TOP_RESULT}"

    json_start_object "request"
    local method=${REQUEST_METHOD}
    json_add_string "method" "${method}"
    if [[ "$method" == "GET" ]]; then
        json_add_string "query" "${QUERY_STRING}" true
    else
        local length=${CONTENT_LENGTH}
        if [[ $length -gt 0 ]]; then
            read -n${length} content
            json_add_raw "content" "${content}"
        fi

        json_add_int "length" "${length}" true
    fi
    json_end_object true # request }

    json_end_object true # result }

    json_end_object true # root }

    json_dump
}

push_result() {
    echo -ne "HTTP/1.0 200 OK\n"
    echo -ne "Content-Type: application/json\n"
    local json_result=$(result_content)
    local json_length=${#json_result}
    echo -ne "Content-Length: ${json_length}\n\n"
    echo -ne "$json_result"
}

push_result