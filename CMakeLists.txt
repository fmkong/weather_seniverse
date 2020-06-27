set(COMPONENT_ADD_INCLUDEDIRS
  include
  )

set(COMPONENT_SRCS
  src/seniverse.c
  )

set(COMPONENT_PRIV_INCLUDEDIRS
  src
  )

set(COMPONENT_REQUIRES esp-tls json)

set(COMPONENT_EMBED_TXTFILES server_root_cert.pem)

register_component()
