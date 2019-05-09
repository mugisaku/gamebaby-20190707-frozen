

set(SOURCES
#  ww_entry.cpp
#  ww_company.cpp
  ww_bar.cpp
  ww_spilling_text.cpp
#  ww_context.cpp
#  ww_context_callbacks.cpp
)


include_directories(./)
include_directories(../)
add_library("ww" SHARED ${SOURCES})




