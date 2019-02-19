#include"libww/ww_company.hpp"




namespace ww{




namespace{
constexpr int  g_bar_length_max = 100;
}




company::
company(int&  counter) noexcept:
m_play_counter(counter)
{
}




void
company::
set_debug() const noexcept
{
}




void
company::
reset_hp_bar() noexcept
{
    if(m_original)
    {
      auto  hp     = m_variable.get_hp();
      auto  hp_max = m_variable.get_hp_max();

      auto  l = gbstd::fixed_t(g_bar_length_max)/hp_max*hp;

      m_hp_bar.set_target_length(l);
    }
}


void
company::
pause_motion() noexcept
{
  m_status.set(flags::motion_paused);
}


void
company::
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
company::
rewind_motion() noexcept
{
  m_motion_frame_index = 0;

//  m_motion_timer = m_time.get();

    if(m_status.test(flags::motion_finished))
    {
      m_status.unset(flags::motion_finished);
    }
}


void
company::
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
company::
set_tag(battle_side  side, int  i, gbstd::color  color) noexcept
{
  m_tag = tag(side,i,color);

  int  y = 32+(g_cell_height*i);

  constexpr int  name_w = (gbstd::g_font_width*(8+6));
  constexpr int   bar_w = 256;

  m_front_point  = gbstd::point(-((g_center_space_width/2)+(g_cell_width/2)),y);
  m_back_point   = m_front_point-gbstd::point(g_cell_width,0);
  m_backup_point =  m_back_point-gbstd::point(g_cell_width,0);
  m_name_point   =  m_back_point+gbstd::point(-64,0);

  m_hp_bar.set_position(m_back_point+gbstd::point(-64,gbstd::g_font_height));

    if(m_tag.is_right())
    {
      m_front_point.x  =  -m_front_point.x;
      m_back_point.x   =   -m_back_point.x;
      m_backup_point.x = -m_backup_point.x;

      m_name_point.x = -m_name_point.x-name_w;


      int  x = m_hp_bar.get_position().x;

      m_hp_bar.set_position({-x,y+gbstd::g_font_height});
    }
}




void
company::
step(uint32_t  time) noexcept
{
  m_process.step();

  m_blink_context.step();

    if( m_status.test(flags::motion_valid ) &&
       !m_status.test(flags::motion_paused))
    {
        while(time >= m_motion_timer)
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
                }


              break;
            }
        }
    }
}


void
company::
reset() noexcept
{
  auto&  hp_bar = m_hp_bar;

  m_ap = 0;

  m_white_flag = false;

  m_status.clear();

  hp_bar.set_target_length(0);
  hp_bar.set_length(       0);

  clear_motion();

    if(m_original)
    {
      m_variable = *m_original;

      m_current_pos = m_backup_point;

      hp_bar.set_mode(m_tag.is_left()? bar::mode::left_to_right:bar::mode::right_to_left);
      hp_bar.set_thickness(8);
      hp_bar.set_color(gbstd::colors::white);
      hp_bar.set_increment(g_bar_length_max,16);

      reset_hp_bar();

      auto  pt = m_variable.is_front_position()? m_front_point
                : m_variable.is_back_position()? m_back_point
                :                                m_backup_point;

      add_motion(pt,32,1000);

      m_status.set(flags::motion_valid);
    }
}


void
company::
reset(entry&  org) noexcept
{
  m_original = &org;

  reset();
}


void
company::
render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept
{
    if(!m_original)
    {
      return;
    }


  auto  cur_pos = m_current_pos+offset;

    if(!m_blink_context.is_valid() || m_blink_context.is_visible())
    {
      m_variable.render(m_white_flag? gbstd::colors::white:m_tag.get_color(),{cv,cur_pos.x,cur_pos.y,32,32});
    }


  gbstd::string_form  sf;

  auto  nam_pt = m_name_point+offset;

  cv.draw_string(gbstd::colors::white,sf("%s",m_variable.get_name().data()),nam_pt.x,nam_pt.y);

  cv.draw_string(gbstd::colors::white,sf("%6d",m_variable.get_hp()),nam_pt.x+(gbstd::g_font_width*8),nam_pt.y);

  m_hp_bar.render(offset,cv);
}




}




