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
render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept
{
    if(m_length)
    {
      auto  pos = offset+m_pos;

      int  x = (is_left_to_right()? pos.x:pos.x-m_length);

      cv.fill_rectangle(m_color,x,pos.y,m_length,m_thickness);
    }
}


void
bar::
task_process(uint32_t&  delay, bar*  b) noexcept
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




