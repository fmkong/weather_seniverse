set(COMPONENT_ADD_INCLUDEDIRS
    include
    include/seniverse_weather_parse
    )

set(COMPONENT_SRCS
    src/seniverse.c
    src/seniverse_weather_parse/seniverse_cJson_utils.c
    src/seniverse_weather_parse/seniverse_weather_now.c
    src/seniverse_weather_parse/seniverse_weather_location.c
    src/seniverse_weather_parse/seniverse_weather.c
    src/seniverse_weather_parse/seniverse_weather_daily.c
    src/seniverse_weather_parse/seniverse_weather_hourly.c
    )

set(COMPONENT_PRIV_INCLUDEDIRS
    include/seniverse_weather_parse
    )

set(COMPONENT_REQUIRES esp-tls json)

set(COMPONENT_EMBED_TXTFILES server_root_cert.pem)

register_component()
