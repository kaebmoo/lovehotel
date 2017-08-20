#!/bin/bash

if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
    exit 1
fi

m=${1}-1
room=${2}
HOST="localhost"

Floor () {
  DIVIDEND=${1}
  DIVISOR=${2}
  RESULT=$(( ( ${DIVIDEND} - ( ${DIVIDEND} % ${DIVISOR}) )/${DIVISOR} ))
  echo ${RESULT}
}

Timecount(){
        s=${1}
        r=${2} # room number
        HOUR=$( Floor ${s} 60/60 )
        s=$((${s}-(60*60*${HOUR})))
        MIN=$( Floor ${s} 60 )
        SEC=$((${s}-60*${MIN}))
     while [ $HOUR -ge 0 ]; do
        while [ $MIN -ge 0 ]; do
                while [ $SEC -ge 0 ]; do
                        # mosquitto_pub -h localhost -t room/1/timer -m """$HOUR":"$MIN":"$SEC"""
                        mosquitto_pub -h $HOST -t $r/timer -m $(printf "%02d:%02d:%02d" $HOUR $MIN $SEC) -u chang -P "chang"
                        # printf "%02d:%02d:%02d\033[0K\r" $HOUR $MIN $SEC

                        SEC=$((SEC-1))
                        sleep 1

                done
                SEC=59
                MIN=$((MIN-1))
        done
        MIN=59
        HOUR=$((HOUR-1))
     done
}

Timecount $m $room
