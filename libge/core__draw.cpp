#include"libge/ge.hpp"




void
core::
draw_line(gbstd::color  color, gbstd::point  a, gbstd::point  b) noexcept
{
  gbstd::liner  l(a.x,a.y,b.x,b.y);

  try_to_push_nonsolid_record();

    for(;;)
    {
      modify_dot(color,l.get_x(),l.get_y());

        if(!l.get_distance())
        {
          break;
        }


      l.step();
    }


  try_to_push_solid_record();
}


void
core::
draw_rect(gbstd::color  color, gbstd::rectangle  rect) noexcept
{
  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < rect.h;  ++yy)
    {
      modify_dot(color,rect.x         ,rect.y+yy);
      modify_dot(color,rect.x+rect.w-1,rect.y+yy);
    }


    for(int  xx = 0;  xx < rect.w;  ++xx)
    {
      modify_dot(color,rect.x+xx,rect.y         );
      modify_dot(color,rect.x+xx,rect.y+rect.h-1);
    }


  try_to_push_solid_record();
}


void
core::
fill_rect(gbstd::color  color, gbstd::rectangle  rect) noexcept
{
  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < rect.h;  ++yy){
    for(int  xx = 0;  xx < rect.w;  ++xx){
      modify_dot(color,rect.x+xx,rect.y+yy);
    }}


  try_to_push_solid_record();
}


void
core::
fill_area(gbstd::color  color, gbstd::point  pt) noexcept
{
  auto  target_color = m_canvas.get_pixel_pointer(pt.x,pt.y)->color;

    if(target_color == color)
    {
      return;
    }


  const int  w = m_editing_width ;
  const int  h = m_editing_height;

  m_canvas.fill(gbstd::color(),0,0,w,h);

  try_to_push_nonsolid_record();


  std::vector<gbstd::point>  stack;

  int  i = 0;

  auto&  offset = m_cursor_offset;

  stack.emplace_back(pt);

    while(i < stack.size())
    {
      auto  pt = stack[i++];

      auto&  pix = *m_canvas.get_pixel_pointer(pt.x,pt.y);

        if(!pix.z)
        {
          pix.z = 1;

            if(pix.color == target_color)
            {
              m_recorder.put(pix.color,offset.x+pt.x,
                                       offset.y+pt.y);

              pix.color = color;

                if(pt.x      ){stack.emplace_back((pt.x-1,pt.y  ));}
                if(pt.y      ){stack.emplace_back((pt.x  ,pt.y-1));}
                if(pt.x < w-1){stack.emplace_back((pt.x+1,pt.y  ));}
                if(pt.y < h-1){stack.emplace_back((pt.x  ,pt.y+1));}
            }
        }
    }


  try_to_push_solid_record();

  m_drawing_is_fixed = true;
}





