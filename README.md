# ESP32 MetRose

## diagramm to do
```mermaid
 classDiagram
 direction LR
    note "MetRose"
    wifi <-- main
    station <-- main
    station --> tisseo
    mapper --> leds 
    leds --> led_controller
    mapper --> station

    class mapper{
        +void from_station_to_led()
    }

    class station {
        int MAX_DEPARTURES
        int MAX_NAME_LEN
        int NUM_STATIONS
        int STOP_AREA_ID_LEN
        str LINE
        int TERMINUS_1
        int TERMINUS_2
        struct station_t
        stations_t stations[NUM_STATIONS]
        +void init_stations()
        +void reset_station(int station_index)
        +void update_station_departure(int station_index)
    }

    class wifi{
        + wifi_init_sta()
        - http_event_handler()
    }

    class leds{
        uint8_t leds[]
        uint16_t fade_value
        uint8_t fade_incr
        bool fade_incr_up
        + leds_init()
        + update_leds()
        + blink_led(uint16_t led_index)
        + fade_update()
        - is_in_range()
        - handle_terminus()
    }

    class led_controller{
        + led_manager_init(int led_count)
        +void setPixel(int i, uint8_t r, g, b)
        void show()
    }

    class tisseo{
        datetimes_list_str_t
        + get_stops_schedules(int station_index)
        + extract_departures_times(cJSON *resp, int station_index)
        + convert_datetimes_str_to_timestamp(datetimes_list_str_t, int terminus)
    }
```

## To Do
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
- api call to get next departures for each station
- 3 state of led: OFF, ON, BLINK

### To Do
- fix terminus bug: we only have departures, no arrival for end of line
- implement fading
- control led intensity/ better led control
- code refactor for resuse later with metro and/or paris

