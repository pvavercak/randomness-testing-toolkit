set(rtt_USE_SQL OFF CACHE BOOL "Build with/without mysql functionality. If ON, requires MySQL C++ wrapper to be installed.")
if(rtt_USE_SQL)
  add_compile_definitions(-DRTT_USE_SQL)
endif()
