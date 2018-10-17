

set(SOURCES
  color_maker.cpp
  color_holder.cpp
  aniview.cpp
  drawing_recorder.cpp
  underlay_stacker.cpp
  core.cpp
  core__do_on_mouse_act.cpp
  core__draw.cpp
  core__operate.cpp
  core__create_sub_widget.cpp
  context.cpp
  context__build_core.cpp
  context__build_cell_table.cpp
)


include_directories(./)
include_directories(../)
add_library("ge" SHARED ${SOURCES})




