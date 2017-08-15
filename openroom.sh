#!/bin/bash
if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
    exit 1
fi
readonly MIN=60
readonly HOUR=3600
((min=MIN*$1))
((hour=HOUR*$1))
echo "ON" `date`
# echo $min
# echo $hour
echo $2 $min second
mosquitto_pub -h register.local -t $2 -m "1" -u chang -P "chang"
sleep $min
echo "OFF" `date`
mosquitto_pub -h register.local -t $2 -m "0" -u chang -P "chang"
