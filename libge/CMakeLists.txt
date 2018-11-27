

set(SOURCES
  color_maker.cpp
  color_holder.cpp
  color_handler.cpp
  aniview.cpp
  previewer.cpp
  seamless_pattern_view.cpp
  drawing_recorder.cpp
  underlay_stacker.cpp
  core.cpp
  core_view.cpp
  core_display.cpp
  core_paint.cpp
  core_paint__draw.cpp
  core_paint__operate.cpp
  core_paint__make_widget.cpp
  context2.cpp
  context.cpp
  context__build_core.cpp
  context__build_cell_table.cpp
)


include_directories(./)
include_directories(../)
add_library("ge" STATIC ${SOURCES})




