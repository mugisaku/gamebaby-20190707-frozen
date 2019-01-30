#include"libww/ww.hpp"




namespace ww{




namespace{
const row*  g_debug_row;
}


void
row::
set_debug() const noexcept
{
  g_debug_row = this;
}




void
row::
pause_motion() noexcept
{
  m_status.set(flags::motion_paused);
}


void
row::
unpause_motion() noexcept
{
  m_status.unset(flags::motion_paused);

    if(m_motion_frame_index < m_motion_frames.size())
    {
      auto&  frm = m_motion_frames[m_motion_frame_index];

      m_motion_timer = gbstd::get_virtual_time()+frm.m_delay;
    }
}


void
row::
rewind_motion() noexcept
{
  m_motion_frame_index = 0;

  m_motion_timer = m_time.get();

    if(m_status.test(flags::motion_finished))
    {
      m_status.unset(flags::motion_finished);

      ++m_moving_row_counter;
    }
}


void
row::
add_motion(gbstd::point  dst, int  number_of_frames, uint32_t  ms) noexcept
{
    if(number_of_frames <= 0)
    {
      report;
      return;
    }


  auto  delay_u = ms/number_of_frames;

    if(!delay_u)
    {
      m_current_pos = dst;

      return;
    }


  double  x_diff = std::abs(dst.x-m_current_pos.x);
  double  y_diff = std::abs(dst.y-m_current_pos.y);

  auto  x_u = x_diff/number_of_frames;
  auto  y_u = y_diff/number_of_frames;

    if(m_current_pos.x > dst.x){x_u = -x_u;}
    if(m_current_pos.y > dst.y){y_u = -y_u;}


  double  x = m_current_pos.x;
  double  y = m_current_pos.y;

    while(number_of_frames--)
    {
      x += x_u;
      y += y_u;

      gbstd::point  pt(x,y);

      m_motion_frames.emplace_back(pt,delay_u);
    }
}


void
row::
set_force(force&  f, counter  moving_row_counter, int  i) noexcept
{
  m_moving_row_counter = moving_row_counter;

  m_force = &f;
  m_index =  i;

  int  y = 32+(g_cell_height*i);

  constexpr int  name_w = (gbstd::g_font_width*(8+6));
  constexpr int   bar_w = 256;

  m_front_point  = gbstd::point(-((g_center_space_width/2)+(g_cell_width/2)),y);
  m_back_point   = m_front_point-gbstd::point(g_cell_width,0);
  m_backup_point =  m_back_point-gbstd::point(g_cell_width,0);
  m_name_point   =  m_back_point+gbstd::point(-64,0);
  m_bar_point    =  m_back_point+gbstd::point(-64,gbstd::g_font_height);

    if(is_right())
    {
      m_front_point.x  =  -m_front_point.x;
      m_back_point.x   =   -m_back_point.x;
      m_backup_point.x = -m_backup_point.x;

      m_name_point.x = -m_name_point.x-name_w;
      m_bar_point.x  = -m_bar_point.x;
    }
}




bool  row::is_left()  const noexcept{return *m_force == battle_side::left;}
bool  row::is_right() const noexcept{return *m_force == battle_side::right;}


void
row::
step() noexcept
{
  m_process.step();

  m_blink_context.step();

    if( m_status.test(flags::motion_valid ) &&
       !m_status.test(flags::motion_paused))
    {
      auto  t = m_time.get();

        while(t >= m_motion_timer)
        {
            if(m_motion_frame_index < m_motion_frames.size())
            {
              auto&  frm = m_motion_frames[m_motion_frame_index++];

              m_current_pos = frm.m_pos;

              m_motion_timer += frm.m_delay;
            }

          else
            {
                if(!m_status.test(flags::motion_finished))
                {
                  m_status.set(flags::motion_finished);

                  --m_moving_row_counter;
                }


              break;
            }
        }
    }
}


void
row::
reset() noexcept
{
  m_ap = 0;

  m_white_flag = false;

  m_status.clear();

  clear_motion();

    if(m_original)
    {
      m_variable = *m_original;

      m_current_pos = m_backup_point;

      auto  pt = m_variable.is_front_position()? m_front_point
                : m_variable.is_back_position()? m_back_point
                :                                m_backup_point;

      add_motion(pt,32,1000);

      m_status.set(flags::motion_valid);
    }
}


void
row::
reset(force&  force, company*  org) noexcept
{
  m_force = &force;
  m_original = org;

  reset();
}


void
row::
render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept
{
    if(!m_original)
    {
      return;
    }


  auto  cur_pos = m_current_pos+offset;

    if(!m_blink_context.is_valid() || m_blink_context.is_visible())
    {
      m_variable.render(m_white_flag? gbstd::colors::white:m_force->get_color(),{cv,cur_pos.x,cur_pos.y,32,32});
    }


  gbstd::string_form  sf;

  auto  nam_pt = m_name_point+offset;
  auto  bar_pt =  m_bar_point+offset;

  cv.draw_string(gbstd::colors::white,sf("%s",m_variable.get_name().data()),nam_pt.x,nam_pt.y);

  cv.draw_string(gbstd::colors::white,sf("%6d",m_variable.get_hp()),nam_pt.x+(gbstd::g_font_width*8),nam_pt.y);

  auto  rect = is_left()? gbstd::rectangle(bar_pt.x,bar_pt.y,100,8)
                        : gbstd::rectangle(bar_pt.x,bar_pt.y,100,8);

  cv.fill_rectangle(gbstd::colors::white,rect);
//  cv.draw_string(gbstd::colors::white,sf("%6d",m_variable.get_hp()),car_pos.x,car_pos.y);
}




void
row::
callbacks::
start_motion(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->rewind_motion();

  ++exec;
}


void
row::
callbacks::
pause_motion(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->rewind_motion();

  ++exec;
}


void
row::
callbacks::
unpause_motion(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->rewind_motion();

  ++exec;
}


void
row::
callbacks::
wait_for_motion(gbstd::execution&  exec, ww::row*  row) noexcept
{
    if(row->is_motion_finished())
    {
      ++exec;
    }
}




}




