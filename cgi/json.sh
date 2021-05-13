#!/bin/sh

JSON_BUFF=""

json_init() {
    JSON_BUFF=""
}

json_buff() {
    value=$1
    JSON_BUFF="${JSON_BUFF}${value}"
}

json_dump() {
    echo -ne ${JSON_BUFF}
}

# json_add_string <name> <value> [is_last]
json_add_string() {
    name=$1
    value=$2
    is_last=$3
    json_buff "\"$name\": \"$value\""
    if [[ -z "$is_last" ]] || [[ ! $is_last ]]; then
        json_buff ", "
    fi
    
}
# json_add_int <name> <value> [is_last]
json_add_int() {
    name=$1
    value=$2
    is_last=$3
    if [[ -z "$value" ]]; then
        value=0
    fi
    json_buff "\"$name\": $value"
    if [[ -z "$is_last" ]] || [[ ! $is_last ]]; then
        json_buff ", "
    fi
    
}

# json_add_raw <name> <value> [is_last]
json_add_raw() {
    name=$1
    value=$2
    is_last=$3

    if [[ -z "$value" ]]; then
        value="\"\""
    fi
    if [[ -z "$name" ]]; then
        json_buff "$value"
    else
        json_buff "\"$name\": $value"
    fi
    if [[ -z "$is_last" ]] || [[ ! $is_last ]]; then
        json_buff ", "
    fi
    
}

# json_start_object [name]
json_start_object() {
    name=$1
    if [[ -n "$name" ]]; then
        json_buff "\"$name\": "
    fi
    json_buff "{"
}

json_end_object() {
    is_last=$1
    json_buff "}"
    if [[ -z "$is_last" ]] || [[ ! $is_last ]]; then
        json_buff ", "
    fi
}

# json_start_array [name]
json_start_array() {
    name=$1
    if [[ -n "$name" ]]; then
        json_buff "\"$name\": "
    fi
    json_buff "["
}

json_end_array() {
    is_last=$1
    json_buff "]"
    if [[ -z "$is_last" ]] || [[ ! $is_last ]]; then
        json_buff ", "
    fi
}