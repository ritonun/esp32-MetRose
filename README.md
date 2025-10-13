# ESP32 MetRose

## To Do
- fix memory leak in station.c -> end of fn update_station_departure, cJSON_Delete(resp) create a hard error, but without it there is memory leak so...
- add stat about api utilisation (nb of call, etc...)

## Use
Create a config.h file in main/ with values:
```C
#pragma once

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

## Functionnality
### Current functionnality
- make an api call to a specific station & line
- parse the json answer to get the next departures datetimes
- convert them into timestamps

### To Do
- mapper to map each station/next departures to an I/O led
- I/O ON or OFF dependant on time and presence of a train/tram
- monitoring of memory consumption
