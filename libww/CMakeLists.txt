

set(SOURCES
  ww_character.cpp
  ww_bar.cpp
  ww_string_display.cpp
  ww_spilling_text.cpp
  ww_party.cpp
  ww_manage_section.cpp
  ww_battle_section.cpp
  ww_battle_section_menu.cpp
  ww_context.cpp
  ww_context_callbacks.cpp
)


include_directories(./)
include_directories(../)
add_library("ww" SHARED ${SOURCES})




