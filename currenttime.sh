#!/bin/bash

while true
do
  currenttime=$(date +%s)
  mosquitto_pub -h register.local -t room/currenttime -m $currenttime --retain -u chang -P "chang"
  sleep 1
done
