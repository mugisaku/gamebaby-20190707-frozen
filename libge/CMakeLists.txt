

set(SOURCES
  color_maker.cpp
  color_holder.cpp
  aniview.cpp
  seamless_pattern_view.cpp
  arrange_view.cpp
  parts_table.cpp
  drawing_recorder.cpp
  underlay_stacker.cpp
  core.cpp
  core_display.cpp
  core_paint.cpp
  core_paint__draw.cpp
  core_paint__operate.cpp
  core__create_sub_widget.cpp
  context.cpp
  context__build_core.cpp
  context__build_cell_table.cpp
)


include_directories(./)
include_directories(../)
add_library("ge" SHARED ${SOURCES})




