#include"libww/ww_bar.hpp"




namespace ww{




void
bar::
set_increment(int  length, int  num_frames) noexcept
{
    if(num_frames > 0)
    {
      m_increment = gbstd::fixed_t(length)/num_frames;
    }
}


void
bar::
set_target_length(int  length) noexcept
{
  m_target_length = length;
}




void
bar::
startup() noexcept
{
  auto&  tskls =     gbstd::get_major_task_list();
  auto&  pails = *gbstd::get_major_painter_list();

  tskls.push(drive,80,this);
  pails.emplace_back(*this,render);
}


void
bar::
cleanup() noexcept
{
}


void
bar::
render(const bar&  b, const gbstd::canvas&  cv) noexcept
{
    if(b.m_length)
    {
      auto  pos = b.m_offset+b.m_pos;

      int  x = (b.is_left_to_right()? pos.x:pos.x-b.m_length);

      cv.fill_rectangle(b.m_color,x,pos.y,b.m_length,b.m_thickness);
    }
}


void
bar::
drive(gbstd::task_control&  ctrl, bar*  b) noexcept
{
    if(!b->m_frozen)
    {
        if(b->m_length < b->m_target_length)
        {
          b->m_length += b->m_increment;

            if(b->m_length >= b->m_target_length)
            {
              b->m_length = b->m_target_length;
            }
        }

      else
        if(b->m_length > b->m_target_length)
        {
          b->m_length -= b->m_increment;

            if(b->m_length <= b->m_target_length)
            {
              b->m_length = b->m_target_length;
            }
        }
    }
}




}




