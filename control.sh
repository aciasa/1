#!/bin/bash

DIALOG=${DIALOG=dialog}
TITLE="UART UI"
DATABASE="test.db"

function send_to_socket() {
    local input=$(echo $1)
    echo $input | nc localhost 8080
}

function query_database() {
    sqlite3 $DATABASE "SELECT * FROM test_table"
}

$DIALOG --clear --title "$TITLE" \
        --menu "Choose an action:" 15 50 4 \
        "1" "Send data to socket" \
        "2" "Query database" 2>choice.txt

choice=$(cat choice.txt)

case $choice in
    1)
        $DIALOG --inputbox "Enter data to send:" 8 40 2>input.txt
        input=$(cat input.txt)
        send_to_socket $input
        ;;
    2)
        result=$(query_database)
        $DIALOG --msgbox "$result" 20 50
        ;;
esac

rm -f choice.txt input.txt
clear
