# lovehotel
Control Electrical in Room


control wifi switch via mosquitto (mqtt server)


topic : room/1 ; room number 1 ; 1 = ON, 0 = OFF wifi switch receive message from mqtt

topic : room/1/status ; status (ON, OFF) wifi switch send status back to mqtt

topic : room/1/start ; start time

topic : room/1/stop ; stop time

topic : room/currenttime ; current time

topic : room/1/timer ; countdown timer
