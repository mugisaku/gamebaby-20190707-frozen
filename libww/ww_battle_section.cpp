#include"libww/ww_battle_section.hpp"
#include"libww/ww_context.hpp"




namespace ww{




battle_section::
battle_section() noexcept
{
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




void
battle_section::
entry_party(const party&  p, battles::side  side) noexcept
{
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

      c.m_field.m_hp_bar.set_position(side.is_left()? 0:context::get_screen_width(),line_height*n+16)
                        .set_mode(side.is_left()? bar::mode::left_to_right: bar::mode::right_to_left)
                        .set_thickness(10)
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

      c.m_field.m_side = side;

      m_task_list.push(c        ).set_draw<battles::character>().set_tick<battles::character>(m_clock_watch,20);
      m_task_list.push(c.m_field).set_draw<battles::field    >().set_tick<battles::field    >(m_clock_watch,20);

      m_task_list.push(  c.m_hp_display).set_draw<string_display>();
      m_task_list.push(c.m_name_display).set_draw<string_display>();

      ++n;
    }
}




}




