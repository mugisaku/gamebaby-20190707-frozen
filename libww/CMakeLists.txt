

set(SOURCES
  ww_entry.cpp
  ww_company.cpp
  ww_bar.cpp
  ww_notifier.cpp
  ww_battle_context.cpp
  ww_battle_context_callbacks.cpp
  ww_executive_context.cpp
)


include_directories(./)
include_directories(../)
add_library("ww" SHARED ${SOURCES})




