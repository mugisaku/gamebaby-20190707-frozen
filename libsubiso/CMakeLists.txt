

set(SOURCES
  subiso_plane.cpp
  subiso_plane_reference_stack.cpp
  subiso_space.cpp
  subiso_space_view.cpp
  subiso_renderer.cpp
)


include_directories(./)
include_directories(../)
add_library("subiso" SHARED ${SOURCES})




