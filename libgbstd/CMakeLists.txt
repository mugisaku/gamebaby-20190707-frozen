

set(SOURCES
  parsers/token.cpp
  parsers/token_block.cpp
  parsers/tokenizer.cpp
  parsers/tokenizer__skip.cpp
  parsers/tokenizer__read_number.cpp
  characters/unicode.cpp
  characters/font.cpp
  characters/text.cpp
  characters/typewriter.cpp
  images/image.cpp
  images/image__png.cpp
  images/canvas.cpp
  images/canvas__draw.cpp
  images/canvas__draw_string.cpp
  images/liner.cpp
  values/object.cpp
  values/array.cpp
  values/object.cpp
  menus/cursor.cpp
  menus/table.cpp
  menus/view.cpp
  menus/stack.cpp
  menus/window.cpp
  windows/window.cpp
  windows/widget.cpp
  windows/components/frame.cpp
  windows/components/label.cpp
  windows/components/button.cpp
  windows/components/checkbox.cpp
  windows/components/radio_button.cpp
  windows/components/iconshow.cpp
  windows/components/dial.cpp
  windows/components/menu.cpp
  windows/icons/icon.cpp
  file_ops/file_op.cpp
  file_ops/riff_chunk.cpp
  file_ops/riff_subchunk_view.cpp
  file_ops/wave.cpp
  file_ops/wave_format.cpp
  file_ops/ogg_page.cpp
  sounds/sound_device.cpp
  sounds/sine_wave_device.cpp
  sounds/square_wave_device.cpp
  sounds/triangle_wave_device.cpp
  sounds/sawtooth_wave_device.cpp
  sounds/noise_device.cpp
  sounds/short_noise_device.cpp
  sounds/scale_table.cpp
  weak_reference_counter.cpp
  instruction_queue.cpp
  clock.cpp
  task_list.cpp
  process.cpp
  utility.cpp
  utility_sincos.cpp
  control.cpp
)


include_directories(./)
include_directories(../)
add_library("gbstd" ${SOURCES})




