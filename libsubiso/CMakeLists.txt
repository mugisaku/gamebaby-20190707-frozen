

set(SOURCES
  subiso_plane.cpp
  subiso_plane_reference.cpp
  subiso_plane_reference_stack.cpp
  subiso_stack_map.cpp
  subiso_box.cpp
  subiso_box_view.cpp
  subiso_space.cpp
  subiso_space_view.cpp
  subiso_space_handler.cpp
  subiso_actor.cpp
)


include_directories(./)
include_directories(../)
add_library("subiso" SHARED ${SOURCES})




