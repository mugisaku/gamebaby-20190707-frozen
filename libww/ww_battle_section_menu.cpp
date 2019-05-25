#include"libww/ww_battle_section.hpp"
#include"libww/ww_context.hpp"




namespace ww{




namespace{


std::u16string_view  attack_sv(u"こうげき");
std::u16string_view   guard_sv(u"ぼうぎょ");
std::u16string_view    move_sv(u"いどう");

std::u16string_view  advance_sv(u"ぜんしん");
std::u16string_view     back_sv(u"こうたい");
std::u16string_view     stop_sv(u"ていし");

std::u16string_view    fastly_sv(u"はやく");
std::u16string_view  normally_sv(u"ふつう");
std::u16string_view    slowly_sv(u"おそく");

std::u16string_view  strongly_sv(u"つよく");
std::u16string_view    weakly_sv(u"よわく");



void
draw_sv_entry(const gbstd::menus::entry&  ent, std::u16string_view&  sv, const gbstd::canvas&  cv) noexcept
{
  cv.draw_string({gbstd::colors::white},sv,0,0);
}


}




void
battle_section::
manage_first_menu(gbstd::menus::stack&  stk, const gbstd::menus::result*  res, gbstd::menus::view&  view, battle_section&  b) noexcept
{
    if(res)
    {
      return;
    }


  auto&  cur = view.get_first_cursor();

    if(!gbstd::test_input_barrier())
    {
        if(gbstd::g_input)
        {
          gbstd::barrier_input();
        }


        if(gbstd::g_input.test_p())
        {
          b.m_second_menu_view.get_first_cursor().set_y(0);

          auto  p = b.m_second_menu_table.get_entry_pointer(0,0);

            if(cur.get_y() <= 1)
            {
              *p++ = {strongly_sv};
              *p++ = {normally_sv};
              *p++ = {  weakly_sv};
            }

          else
            {
              *p++ = {advance_sv};
              *p++ = {   back_sv};
              *p++ = {   stop_sv};
            }


          stk.open(cur.get_y(),b.m_second_menu_view,b,battle_section::manage_second_menu);
        }

      else
        if(gbstd::g_input.test_n()    ){stk.close_top(0);}
        if(gbstd::g_input.test_up()   ){cur.add_y(-1);}
        if(gbstd::g_input.test_down() ){cur.add_y( 1);}
    }
}


void
battle_section::
manage_second_menu(gbstd::menus::stack&  stk, const gbstd::menus::result*  res, gbstd::menus::view&  view, battle_section&  b) noexcept
{
    if(res)
    {
      return;
    }


  auto&  cur = view.get_first_cursor();

    if(!gbstd::test_input_barrier())
    {
        if(gbstd::g_input)
        {
          gbstd::barrier_input();
        }


        if(gbstd::g_input.test_p())
        {
            if(stk.get_opening_value() <= 1)
            {
              stk.close_top(-1);
            }

          else
            {
                if(cur.get_y() <= 1)
                {
                  b.m_third_menu_view.get_first_cursor().set_y(0);

                  auto  p = b.m_third_menu_table.get_entry_pointer(0,0);

                  *p++ = {  fastly_sv};
                  *p++ = {normally_sv};
                  *p++ = {  slowly_sv};

                  stk.open(cur.get_y(),b.m_third_menu_view,b,battle_section::manage_third_menu);
                }

              else
                {
                  stk.close_top(-1);
                }
            }
        }

      else
        if(gbstd::g_input.test_n()    ){stk.close_top(-1);}
        if(gbstd::g_input.test_up()   ){cur.add_y(-1);}
        if(gbstd::g_input.test_down() ){cur.add_y( 1);}
    }
}


void
battle_section::
manage_third_menu(gbstd::menus::stack&  stk, const gbstd::menus::result*  res, gbstd::menus::view&  view, battle_section&  b) noexcept
{
    if(res)
    {
      return;
    }


  auto&  cur = view.get_first_cursor();

    if(!gbstd::test_input_barrier())
    {
        if(gbstd::g_input)
        {
          gbstd::barrier_input();
        }


        if(gbstd::g_input.test_p())
        {
          stk.close_top(cur.get_y());
        }

      else
        if(gbstd::g_input.test_n()    ){stk.close_top(-1);}
        if(gbstd::g_input.test_up()   ){cur.add_y(-1);}
        if(gbstd::g_input.test_down() ){cur.add_y( 1);}
    }
}


void
battle_section::
initialize_menu() noexcept
{
  m_first_menu_table.resize(1,3)
                    .set_entry_width(gbstd::g_font_width*5)
                    .set_entry_height(gbstd::g_font_height);

  m_second_menu_table.resize(1,3)
                     .set_entry_width(gbstd::g_font_width*5)
                     .set_entry_height(gbstd::g_font_height);

  m_third_menu_table.resize(1,3)
                     .set_entry_width(gbstd::g_font_width*5)
                     .set_entry_height(gbstd::g_font_height);

  {
    auto  p = m_first_menu_table.get_entry_pointer(0,0);

    *p++ = {attack_sv};
    *p++ = { guard_sv};
    *p++ = {  move_sv};
  }


  m_first_menu_view.assign(m_first_menu_table)
             .set_window_color(gbstd::colors::blue)
             .set_x(32)
             .set_y(180)
             .set_callback(draw_sv_entry).get_first_cursor().show();


  m_second_menu_view.assign(m_second_menu_table)
             .set_window_color(gbstd::colors::blue)
             .set_x(32+80)
             .set_y(180)
             .set_callback(draw_sv_entry).get_first_cursor().show();


  m_third_menu_view.assign(m_third_menu_table)
             .set_window_color(gbstd::colors::blue)
             .set_x(32+140)
             .set_y(180)
             .set_callback(draw_sv_entry).get_first_cursor().show();
}



}




