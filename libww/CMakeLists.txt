

set(SOURCES
  ww_company.cpp
  ww_row.cpp
  ww_force.cpp
  ww_bar.cpp
  ww_notifier.cpp
  ww_battle_context.cpp
  ww_callbacks.cpp
)


include_directories(./)
include_directories(../)
add_library("ww" SHARED ${SOURCES})




