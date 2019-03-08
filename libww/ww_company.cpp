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
clear_event_queue() noexcept
{
  m_play_counter.cancel();

  m_status.set(flags::motion_finished);

  m_event_index = 0;

  m_event_queue.clear();

  m_current_event = company_event();
}


void
company::
add_absolute_motion(real_point  dst, int  number_of_frames, uint32_t  ms) noexcept
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


  auto  x_diff = (dst.x-m_current_pos.x);
  auto  y_diff = (dst.y-m_current_pos.y);

  auto  x_u = x_diff.abs()/number_of_frames;
  auto  y_u = y_diff.abs()/number_of_frames;

    if(m_current_pos.x > dst.x){x_u = -x_u;}
    if(m_current_pos.y > dst.y){y_u = -y_u;}


  m_event_queue.emplace_back(company_events::move(number_of_frames,delay_u,x_u,y_u));

  m_play_counter.apply();
}


void
company::
add_relative_motion(real_point  off, int  number_of_frames, uint32_t  ms) noexcept
{
    if(number_of_frames <= 0)
    {
      report;
      return;
    }


  auto  delay_u = ms/number_of_frames;

    if(!delay_u)
    {
      m_current_pos += off;

      return;
    }


  auto  x_u = off.x/number_of_frames;
  auto  y_u = off.y/number_of_frames;

  m_event_queue.emplace_back(company_events::move(number_of_frames,delay_u,x_u,y_u));

  m_play_counter.apply();
}


void
company::
add_jump_motion() noexcept
{
  add_relative_motion({0,-16},8,200);
  add_relative_motion({0, 16},8,200);
}


void
company::
add_attack_motion() noexcept
{
  push(company_events::set_white());
  push(company_events::stop(80));
  push(company_events::unset_white());

    if(m_tag.is_left())
    {
      add_relative_motion({ 16,0},8,200);
      add_relative_motion({-16,0},8,200);
    }

  else
    {
      add_relative_motion({-16,0},8,200);
      add_relative_motion({ 16,0},8,200);
    }
}


void
company::
add_damage_motion() noexcept
{
  push(company_events::set_blink());

    if(m_tag.is_left())
    {
      add_relative_motion({-16,0},8,200);
      add_relative_motion({ 16,0},8,200);
    }

  else
    {
      add_relative_motion({ 16,0},8,200);
      add_relative_motion({-16,0},8,200);
    }


  push(company_events::unset_blink());
}


void
company::
set_tag(battle_side  side, int  i, gbstd::color  color) noexcept
{
  m_tag = tag(side,i,color);

  int  y = 32+(g_cell_height*i);

  constexpr int  name_w = (gbstd::g_font_width*(8+6));
  constexpr int   bar_w = 256;

  m_front_point  = real_point(-((g_center_space_width/2)+(g_cell_width/2)),y);
  m_back_point   = m_front_point-real_point(g_cell_width,0);
  m_backup_point =  m_back_point-real_point(g_cell_width,0);
  m_name_point   =  m_back_point+real_point(-64,0);

  m_hp_bar.set_position(m_back_point+real_point(-64,gbstd::g_font_height));

    if(m_tag.is_right())
    {
      m_front_point.x  =  -m_front_point.x;
      m_back_point.x   =   -m_back_point.x;
      m_backup_point.x = -m_backup_point.x;

      m_name_point.x = -m_name_point.x-name_w;


      int  x = m_hp_bar.get_position().x;

      m_hp_bar.set_position({-x,y+gbstd::g_font_height});

      m_hp_bar.set_mode(bar::mode::right_to_left);
    }

  else
    {
      m_hp_bar.set_mode(bar::mode::left_to_right);
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

  clear_event_queue();

  hp_bar.set_target_length(0);
  hp_bar.set_length(       0);

  hp_bar.set_offset(m_offset);

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

      add_absolute_motion(pt,32,1000);

      m_status.set(flags::motion_valid);
    }
}


void
company::
set_entry(entry&  org) noexcept
{
  m_original = &org;
}


void
company::
startup() noexcept
{
  auto&  tskls = gbstd::get_major_task_list();
  auto&  pails = *gbstd::get_major_painter_list();

  tskls.push(step_animation,20,this);
  pails.emplace_back(*this,render,&m_render_counter);

  m_hp_bar.startup();

  reset();
}


void
company::
cleanup() noexcept
{
  m_hp_bar.cleanup();
}




void
company::
render(const company&  c, const gbstd::canvas&  cv) noexcept
{
    if(!c.m_original)
    {
      return;
    }


    if(!c.m_blink_context.is_valid() || c.m_blink_context.is_visible())
    {
      auto  color = c.m_white_flag? gbstd::colors::white:c.m_tag.get_color();

      auto  cur_pos = c.m_current_pos+c.m_offset;

      c.m_variable.render(color,{cv,cur_pos.x,cur_pos.y,32,32});
    }


  gbstd::string_form  sf;

  auto  nam_pt = c.m_name_point+c.m_offset;

  cv.draw_string(gbstd::colors::white,sf("%s",c.m_variable.get_name().data()),nam_pt.x,nam_pt.y);

  cv.draw_string(gbstd::colors::white,sf("%6d",c.m_variable.get_hp()),nam_pt.x+(gbstd::g_font_width*8),nam_pt.y);
}




}




