

set(SOURCES
  parsers/token.cpp
  parsers/tokenizer.cpp
  parsers/tokenizer__skip.cpp
  parsers/tokenizer__read_number.cpp
  characters/unicode.cpp
  characters/point.cpp
  characters/font.cpp
  characters/text_screen.cpp
  characters/text_cursor.cpp
  characters/text_buffer.cpp
  images/image.cpp
  images/image__png.cpp
  images/canvas.cpp
  images/canvas__draw.cpp
  images/liner.cpp
  values/value.cpp
  values/value_array.cpp
  values/value_list.cpp
  values/variable.cpp
  windows/window.cpp
#  windows/window_manager.cpp
  windows/widget.cpp
  windows/components/label.cpp
#  windows/components/button.cpp
  programs/process.cpp
  programs/program.cpp
  programs/sleep.cpp
  utility.cpp
  control.cpp
#  programs/indication.cpp
#  programs/chooser.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" ${SOURCES})




