#!/bin/bash
if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
    exit 1
fi
readonly MIN=1
readonly HOUR=3600
readonly ON="ON"
readonly OFF="OFF"
HOST="localhost"

((min=MIN*$1))
((hour=HOUR*$1))
echo "ON" `date`
# echo $min
# echo $hour
echo $2 $min second
# mosquitto_pub -h register.local -t $2 -m "1" -u chang -P "chang"
start=$(date +%s)
stop=$(($start+$min))
mosquitto_pub -h $HOST -t $2$(echo "/start") -m $start --retain -u chang -P "chang"
mosquitto_pub -h $HOST -t $2$(echo "/stop") -m $stop --retain -u chang -P "chang"

# mosquitto_pub -h $HOST -t $2 -m "1" -u chang -P "chang"
mosquitto_pub -h $HOST -t $2$(echo "/status") -m $ON --retain -u chang -P "chang"

/Users/cat/Documents/GitHub/lovehotel/timer.sh $min $2
# sleep $min
echo "OFF" `date`
# mosquitto_pub -h register.local -t $2 -m "0" -u chang -P "chang"
# mosquitto_pub -h $HOST -t $2$(echo "/stop") -m $(date +%s) --retain -u chang -P "chang"

# mosquitto_pub -h $HOST -t $2 -m "0" -u chang -P "chang"
mosquitto_pub -h $HOST -t $2$(echo "/status") -m $OFF --retain -u chang -P "chang"
