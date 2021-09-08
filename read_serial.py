#!/usr/bin/env python3
import serial
import json
import requests
from datetime import datetime
from time import sleep

oldSetting = True

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.flush()
    while True:
        # if ser.in_waiting > 0:
        modeRequest = requests.get("https://robo-backend.nesvo.dev/sentry")
        if modeRequest.status_code == 200:
            setting = modeRequest.json()
            if setting is False and setting != oldSetting:
                ser.write(b"sentryOff\n")
                ser.flush()
                oldSetting = setting
            if setting is True and setting != oldSetting:
                ser.write(b"sentryOn\n")
                ser.flush()
                oldSetting = setting
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
            try:
                print(json.loads(line))
                message = json.loads(line)
                message["date"] = datetime.now().isoformat()
                print(requests.post("https://robo-backend.nesvo.dev/message/", json=message))
            except json.decoder.JSONDecodeError:
                print('Failed to read message')

                
                # ser.write(b"Hello from Raspberry Pi!\n")