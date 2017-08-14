#!/bin/bash
mosquitto_pub -h localhost -t room1 -m "1" -u chang -P "chang"
sleep 1m
mosquitto_pub -h localhost -t room1 -m "0" -u chang -P "chang"
