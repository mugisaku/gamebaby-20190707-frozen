

set(SOURCES
  color_maker.cpp
  color_holder.cpp
  drawing_recorder.cpp
  core.cpp
  core__do_on_mouse_act.cpp
  core__draw.cpp
  core__make_apng_stream.cpp
  core__operate.cpp
  core__create_sub_widget.cpp
  create_graphics_editor.cpp
)


include_directories(./)
include_directories(../)
add_library("ge" SHARED ${SOURCES})




