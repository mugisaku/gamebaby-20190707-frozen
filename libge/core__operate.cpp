#include"libge/ge.hpp"




namespace ge{


void
core::
paste(gbstd::point  pt, bool  layer) noexcept
{
  try_to_push_nonsolid_record();


  gbstd::rectangle  src_rect = m_clipped_image.get_rectangle();
  gbstd::rectangle  dst_rect(pt,m_canvas.get_width(),m_canvas.get_height());

  auto  rect = src_rect&dst_rect;

  gbstd::canvas  cv(m_clipped_image);

    for(int  y = 0;  y < rect.h;  y += 1){
    for(int  x = 0;  x < rect.w;  x += 1){
      auto  pix = *cv.get_pixel_pointer(rect.x+x,rect.y+y);

        if(!layer || pix.color)
        {
          modify_dot(pix.color,dst_rect.x+x,dst_rect.y+y);
        }
    }}


  try_to_push_solid_record();
}




void
core::
revolve() noexcept
{
  auto  tmp = get_temporary_image();

  const int  w = std::min(m_operation_rect.w,m_operation_rect.h);

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < w;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy);

      modify_dot(pix,m_operation_rect.x+w-1-yy,m_operation_rect.y+xx);
    }}


  try_to_push_solid_record();
}


void
core::
reverse_horizontally() noexcept
{
  auto  tmp = get_temporary_image();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy);

      modify_dot(pix,m_operation_rect.x+xx,m_operation_rect.y+m_operation_rect.h-1-yy);
    }}


  try_to_push_solid_record();
}


void
core::
reverse_vertically() noexcept
{
  auto  tmp = get_temporary_image();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy);

      modify_dot(pix,m_operation_rect.x+m_operation_rect.w-1-xx,m_operation_rect.y+yy);
    }}


  try_to_push_solid_record();
}


void
core::
mirror_vertically() noexcept
{
  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h  ;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w/2;  ++xx){
      auto  pix = *m_canvas.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy);

      modify_dot(pix,m_operation_rect.x+m_operation_rect.w-1-xx,m_operation_rect.y+yy);
    }}


  try_to_push_solid_record();
}




void
core::
shift_up(bool  rotate) noexcept
{
  auto  tmp = get_temporary_image();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h-1;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w  ;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy+1);

      modify_dot(pix,m_operation_rect.x+xx,m_operation_rect.y+yy);
    }}


    if(rotate)
    {
        for(int  xx = 0;  xx < m_operation_rect.w;  ++xx)
        {
          auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y);

          modify_dot(pix,m_operation_rect.x+xx,m_operation_rect.y+m_operation_rect.h-1);
        }
    }


  try_to_push_solid_record();
}


void
core::
shift_left(bool  rotate) noexcept
{
  auto  tmp = get_temporary_image();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h  ;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w-1;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx+1,m_operation_rect.y+yy);

      modify_dot(pix,m_operation_rect.x+xx,m_operation_rect.y+yy);
    }}


    if(rotate)
    {
        for(int  yy = 0;  yy < m_operation_rect.h;  ++yy)
        {
          auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x,m_operation_rect.y+yy);

          modify_dot(pix,m_operation_rect.x+m_operation_rect.w-1,m_operation_rect.y+yy);
        }
    }


  try_to_push_solid_record();
}


void
core::
shift_right(bool  rotate) noexcept
{
  auto  tmp = get_temporary_image();

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 1;  xx < m_operation_rect.w;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx-1,m_operation_rect.y+yy);

      modify_dot(pix,m_operation_rect.x+xx,m_operation_rect.y+yy);
    }}


    if(rotate)
    {
        for(int  yy = 0;  yy < m_operation_rect.h;  ++yy)
        {
          auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+m_operation_rect.w-1,m_operation_rect.y+yy);

          modify_dot(pix,m_operation_rect.x,m_operation_rect.y+yy);
        }
    }


  try_to_push_solid_record();
}


void
core::
shift_down(bool  rotate) noexcept
{
  auto  tmp = get_temporary_image();

  try_to_push_nonsolid_record();

    for(int  yy = 1;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+yy-1);

      modify_dot(pix,m_operation_rect.x+xx,m_operation_rect.y+yy);
    }}


    if(rotate)
    {
        for(int  xx = 0;  xx < m_operation_rect.w;  ++xx)
        {
          auto  pix = *tmp.get_pixel_pointer(m_operation_rect.x+xx,m_operation_rect.y+m_operation_rect.h-1);

          modify_dot(pix,m_operation_rect.x+xx,m_operation_rect.y);
        }
    }


  try_to_push_solid_record();
}




}




