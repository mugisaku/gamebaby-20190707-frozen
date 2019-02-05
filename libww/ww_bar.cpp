#include"libww/ww.hpp"




namespace ww{




void
bar::
reset(int  length, int  num_frames, uint32_t  ms) noexcept
{
    if(m_length == length)
    {
      return;
    }


    if(num_frames <= 1)
    {
      m_length = length;

      return;
    }


  m_delay = ms/num_frames;

    if(!m_delay)
    {
      m_length = length;

      return;
    }


  auto  diff = (m_length-length);

  m_increment = diff.abs()/num_frames;

  m_target_length = length;

    if(m_length > m_target_length)
    {
      m_increment = -m_increment;
    }


  m_next_time = m_time.get()+m_delay;
}


void
bar::
step() noexcept
{
    if(!m_frozen)
    {
      auto  tm = m_time.get();

        while(tm >= m_next_time)
        {
            if(m_increment < 0)
            {
              m_length += m_increment;

                if(m_length <= m_target_length)
                {
                  m_length = m_target_length;

                  break;
                }

              else
                {

                  m_next_time += m_delay;
                }
            }

          else
            if(m_increment > 0)
            {
              m_length += m_increment;

                if(m_length >= m_target_length)
                {
                  m_length = m_target_length;

                  break;
                }

              else
                {
                  m_next_time += m_delay;
                }
            }

          else
            {
              break;
            }
        }
    }
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




}




