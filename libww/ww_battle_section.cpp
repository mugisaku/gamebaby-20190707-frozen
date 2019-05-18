#include"libww/ww_battle_section.hpp"
#include"libww/ww_context.hpp"




namespace ww{




constexpr double  g_standard_stride = 8.0;


battle_section::
battle_section() noexcept
{
  m_left_piece.m_previous_position  = 12.5;
  m_left_piece.m_position           = 12.5;
  m_right_piece.m_previous_position = context::get_screen_width()-1-12.5;
  m_right_piece.m_position          = context::get_screen_width()-1-12.5;

  m_left_piece.m_offense_power  = 2.0;
  m_left_piece.m_defense_power  = 20.0;
  m_left_piece.m_movement_power = 4.0;
  m_left_piece.m_weight         = 1.0;

  m_right_piece.m_offense_power  = 20.0;
  m_right_piece.m_defense_power  = 30.0;
  m_right_piece.m_movement_power = 1.0;
  m_right_piece.m_weight         = 5.0;

  m_left_piece.m_hp  = 1000;
  m_right_piece.m_hp = 1000;

  m_left_piece.m_base  = m_square_line;
  m_right_piece.m_base = m_square_line;

  m_left_piece.m_side  = battles::sides::left;
  m_right_piece.m_side = battles::sides::right;
}




void
battle_section::
clear() noexcept
{
  m_number_of_total_characters = 0;

  m_number_of_left_characters  = 0;
  m_number_of_right_characters = 0;

  m_number_of_playing_characters = 0;

    for(auto&  c: m_character_table)
    {
      c.m_status.clear();
    }


  m_task_list.clear();
}




namespace{
void
draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, piece&  p) noexcept
{
if(p.m_hp > 0)
{
  constexpr int  size = 16;

  int  x = static_cast<int>(p.m_position);

  cv.fill_rectangle(p.m_side.is_left()? gbstd::colors::red:gbstd::colors::yellow,
    x-(size/2),80,size,size*p.m_weight);
}
}


void
bdraw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, battle_section&  b) noexcept
{
  draw(ctrl,cv,b.m_left_piece);
  draw(ctrl,cv,b.m_right_piece);
}


void
tick(gbstd::task_control  ctrl, piece&  p) noexcept
{
  constexpr double  rate = 4.0;

if(p.m_hp <= 0){return;}

  p.m_previous_position = p.m_position;

    if(p.m_side.is_left())
    {
      p.m_position += p.m_movement_power*rate;
    }

  else
    {
      p.m_position -= p.m_movement_power*rate;
    }
}


void
btick(gbstd::task_control  ctrl, battle_section&  b) noexcept
{
  tick(ctrl,b.m_left_piece);
  tick(ctrl,b.m_right_piece);

  auto  ll = b.m_left_piece.m_previous_position-8;
  auto  lr = b.m_left_piece.m_position+8;
  auto  rl = b.m_right_piece.m_position-8;
  auto  rr = b.m_right_piece.m_previous_position+8;

    if((lr >= rl) && (ll <= rr))
    {
      auto  lw = b.m_left_piece.m_weight +(b.m_left_piece.m_defense_power /2)-(b.m_right_piece.m_offense_power/2);
      auto  rw = b.m_right_piece.m_weight+(b.m_right_piece.m_defense_power/2)-(b.m_left_piece.m_offense_power /2);

      auto  l_dist = std::abs(b.m_left_piece.m_position-b.m_left_piece.m_previous_position);
      auto  r_dist = std::abs(b.m_right_piece.m_position-b.m_right_piece.m_previous_position);

      l_dist *= (lw < rw)? (lw/rw):1.0        ;
      r_dist *= (lw < rw)?         1.0:(lw/rw);

      auto  m = ((b.m_left_piece.m_previous_position +l_dist)
                +(b.m_right_piece.m_previous_position-r_dist))/2;

      b.m_left_piece.m_position  = m-8;
      b.m_right_piece.m_position = m+8;
    }
}
}


void
battle_section::
entry_party(const party&  p, battles::side  side) noexcept
{
  m_task_list.push(*this).set_draw(bdraw).set_tick(m_clock_watch,80,btick);



return;

  int&  n = (side.is_left())? m_number_of_left_characters
           :                  m_number_of_right_characters;

    for(int  i = 0;  i < p.m_number_of_members;  ++i)
    {
      auto&  c = m_character_table[m_number_of_total_characters++];

      auto&  pm = p.m_member_table[i];

      c.m_head = pm.m_head;
      c.m_original_body = pm.m_body;
      c.copy_body();
      c.m_status.set(battles::character::flags::active);


      constexpr int  line_height = 48;

      c.m_hp_bar.set_position(side.is_left()? 0:context::get_screen_width(),line_height*n+16)
                .set_mode(side.is_left()? bar::mode::left_to_right: bar::mode::right_to_left)
                .set_thickness(10)
                .set_length(0)
                .set_desired_length(0)
                .update_increment();

      c.m_ap_bar.set_position(side.is_left()? 0:context::get_screen_width(),line_height*n+26)
                  .set_mode(side.is_left()? bar::mode::left_to_right: bar::mode::right_to_left)
                  .set_color(gbstd::colors::blue)
                  .set_thickness(4)
                  .set_length(0)
                  .set_desired_length(0)
                  .update_increment();

      c.m_hp_display   = &c.m_hp_string;
      c.m_name_display = &c.m_name_string;

      c.m_name_string.set_string(c.m_head.m_name)
                     .set_color(gbstd::colors::white);

        if(side.is_left()){c.m_name_display.set_align_to_left();}
      else                {c.m_name_display.set_align_to_right();}


      c.m_name_display.set_position({side.is_left()? 0:context::get_screen_width(),line_height*n});

      c.m_side = side;

      m_task_list.push(c).set_draw<battles::character>().set_tick<battles::character>(m_clock_watch,20);

      m_task_list.push(  c.m_hp_display).set_draw<string_display>();
      m_task_list.push(c.m_name_display).set_draw<string_display>();

      ++n;
    }
}




}




