#include"libww/ww_battle_section.hpp"




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


  m_draw_task_list.clear();
  m_tick_task_list.clear();
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

      c.m_base_point.y = 32*n;
      c.m_field.m_hp_bar.set_position(0,32*n);
      c.m_field.m_name_point = {side.is_left()? 32:120,32*n};

      c.m_field.m_side = side;
      c.m_head = pm.m_head;
      c.m_original_body = pm.m_body;
      c.copy_body();
      c.m_status.set(battles::character::flags::active);

      m_draw_task_list.push(c        );
      m_draw_task_list.push(c.m_field);

      m_tick_task_list.push(m_clock_watch,20,c        );
      m_tick_task_list.push(m_clock_watch,20,c.m_field);

      ++n;
    }
}




}




