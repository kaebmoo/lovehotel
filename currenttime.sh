#!/bin/bash
HOST="192.168.9.1"
#HOST="localhost"
while true
do
  currenttime=$(date +%s)
  mosquitto_pub -h $HOST -t room/currenttime -m $currenttime --retain -u chang -P "chang"
  sleep 1
done
