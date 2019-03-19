

set(SOURCES
  onch_sound.cpp
  onch_definition.cpp
  onch_table.cpp
  onch_space.cpp
)


include_directories(./)
include_directories(../)
add_library("onch" ${SOURCES})




