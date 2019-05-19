#include"libww/ww_battle_section.hpp"
#include"libww/ww_context.hpp"




namespace ww{




namespace{


void
initialize(piece&  p) noexcept
{
  p.m_offense_intensity  = intensity::do_normaly;
  p.m_defense_intensity  = intensity::do_normaly;
  p.m_movement_intensity = intensity::do_normaly;

  p.m_offense_power_base  = 10.0;
  p.m_defense_power_base  = 10.0;
  p.m_movement_power_base = 1.0;
  p.m_weight         = 1.0;

  p.m_hp  = 1000;

  p.m_action_counter = 0;
  p.m_animation_counter = 0;
}
}


battle_section::
battle_section() noexcept
{
  int  i = 48;

    for(auto&  ln: m_lines)
    {
      ln.m_y_position = i      ;
                        i += 16;


      ln.m_left_piece.m_line  = &ln;
      ln.m_right_piece.m_line = &ln;

      ln.m_left_piece.m_side  = battles::sides::left;
      ln.m_right_piece.m_side = battles::sides::right;

      ln.m_left_piece.m_previous_position  = 12.5;
      ln.m_left_piece.m_position           = 12.5;

      ln.m_right_piece.m_previous_position  = context::get_screen_width()-1-12.5;
      ln.m_right_piece.m_position           = context::get_screen_width()-1-12.5;

      initialize(ln.m_left_piece );
      initialize(ln.m_right_piece);
    }
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
const uint8_t  g_human_png[] =
{
#include"gra.png.txt"
};


gbstd::image
gra_img(g_human_png);


void
draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, piece&  p) noexcept
{
if(p.m_hp > 0)
{
  constexpr int  size = 16;

  int  x = static_cast<int>(p.m_position);

//  cv.fill_rectangle(p.m_side.is_left()? gbstd::colors::red:gbstd::colors::yellow,
//    x-(size/2),80,size,size*p.m_weight);

  constexpr int  table[] = {0,48,0,96};

  int  i = table[p.m_animation_counter&3];

  gbstd::canvas  src(gra_img,i,0,48,48);

    if(p.m_side.is_left()){cv.draw_canvas(          src,x-8,p.m_line->m_y_position);}
  else                    {cv.draw_canvas_reversely(src,x-8,p.m_line->m_y_position);}
}
}


void
bdraw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, battle_section&  b) noexcept
{
    for(auto&  ln: b.m_lines)
    {
      draw(ctrl,cv,ln.m_left_piece );
      draw(ctrl,cv,ln.m_right_piece);
    }
}


void
tick(gbstd::task_control  ctrl, piece&  p) noexcept
{
if(p.m_hp <= 0){return;}
++p.m_action_counter;
++p.m_animation_counter;
  p.m_previous_position = p.m_position;

  static gbstd::normal_rand    weak_r(0.4,0.3);
  static gbstd::normal_rand  normal_r(0.9,0.3);
  static gbstd::normal_rand  strong_r(1.4,0.3);

  double  off_rate = (p.m_offense_intensity == intensity::do_weakly  )? weak_r()
                    :(p.m_offense_intensity == intensity::do_normaly )? normal_r()
                    :(p.m_offense_intensity == intensity::do_strongly)? strong_r()
                    :0;

  double  def_rate = (p.m_defense_intensity == intensity::do_weakly  )? weak_r()
                    :(p.m_defense_intensity == intensity::do_normaly )? normal_r()
                    :(p.m_defense_intensity == intensity::do_strongly)? strong_r()
                    :0;

  double  mov_rate = (p.m_movement_intensity == intensity::do_weakly  )? weak_r()
                    :(p.m_movement_intensity == intensity::do_normaly )? normal_r()
                    :(p.m_movement_intensity == intensity::do_strongly)? strong_r()
                    :0;


  p.m_offense_power  = p.m_offense_power_base *off_rate;
  p.m_defense_power  = p.m_defense_power_base *def_rate;
  p.m_movement_power = p.m_movement_power_base*mov_rate;

  constexpr double  stroke = 4.0;

    if(p.m_side.is_left())
    {
      p.m_position += p.m_movement_power*stroke;
    }

  else
    {
      p.m_position -= p.m_movement_power*stroke;
    }
}


void
fix(piece&  p) noexcept
{
    if(p.m_position <= 0)
    {
      p.m_position = 0;

      p.m_movement_intensity = intensity::do_not;
    }

  else
    if(p.m_position >= context::get_screen_width())
    {
      p.m_position = context::get_screen_width();

      p.m_movement_intensity = intensity::do_not;
    }
}

void
btick(gbstd::task_control  ctrl, battle_section&  b) noexcept
{
    for(auto&  ln: b.m_lines)
    {
      tick(ctrl,ln.m_left_piece );
      tick(ctrl,ln.m_right_piece);


      auto&  l = ln.m_left_piece;
      auto&  r = ln.m_right_piece;

        if((l.m_hp > 0) && (r.m_hp > 0))
        {
          auto  ll = l.m_previous_position-8;
          auto  lr = l.m_position+8;
          auto  rl = r.m_position-8;
          auto  rr = r.m_previous_position+8;

            if((lr >= rl) && (ll <= rr))
            {
              auto  r_off = r.m_offense_power-l.m_defense_power;
              auto  l_off = l.m_offense_power-r.m_defense_power;

                if(r_off < 0){r_off = 0.1;}
                if(l_off < 0){l_off = 0.1;}

              l.m_hp -= r_off;
              r.m_hp -= l_off;

                if((l.m_hp > 0) && (r.m_hp > 0))
                {
                  auto  lw = l.m_weight+(l.m_defense_power/2)-(r.m_offense_power/2);
                  auto  rw = r.m_weight+(r.m_defense_power/2)-(l.m_offense_power /2);

                  auto  l_dist = std::abs(l.m_position-l.m_previous_position);
                  auto  r_dist = std::abs(r.m_position-r.m_previous_position);

                  l_dist *= (lw < rw)? (lw/rw):1.0        ;
                  r_dist *= (lw < rw)?         1.0:(lw/rw);

                  auto  m = ((l.m_previous_position +l_dist)
                            +(r.m_previous_position-r_dist))/2;

                  l.m_position = m-8;
                  r.m_position = m+8;

                  fix(l);
                  fix(r);
                }
            }
        }
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




