# embeddedInterview

## Description

A smart home - alarm system

## Features

- Temperature and humidity measurements via DHT11
- An LCD screen showing temperature measurements
- A servo actuating an ultrasound sensor (sentry alarm)
- A website streaming live data and allowing to turn alarm on/off

## Schematic
![Project schematic](https://github.com/nesvoboda/embeddedInterview/blob/70471fc07bd4213254b694bd6f1f7dd59d64fcf5/ei_schem.png)

## Overview
![Project overview](https://github.com/nesvoboda/embeddedInterview/blob/70471fc07bd4213254b694bd6f1f7dd59d64fcf5/ei_view.png)

## Backend scheme
![Project overview](https://github.com/nesvoboda/embeddedInterview/blob/8a3874f1d079750c47537e1aa045e8f75745b5e6/project.png)

## Code structure
`read_serial.py` runs on a Raspberry Pi
`api.py` runs on another machine that is not behind NAT
