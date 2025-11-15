idf_component_register(
    SRCS 
        "esp32WS2811.cpp"
        "Colour.cpp"
    INCLUDE_DIRS "."
    REQUIRES driver freertos
)
