#include"libww/ww_menu.hpp"
#include"libww/ww_context.hpp"




namespace ww{




constexpr int  g_margin = 8;


void
menu::
tick(gbstd::task_control  ctrl, menu&  m) noexcept
{
    if(!ctrl.test_timer_flag() || ctrl.test_timer())
    {
        if(gbstd::g_input.test_up() && m.m_cursor_position)
        {
          --m.m_cursor_position;

          ctrl.set_timer(200);
        }

      else
        if(gbstd::g_input.test_down() && (m.m_cursor_position < (m.m_entry_list.size()-1)))
        {
          ++m.m_cursor_position;

          ctrl.set_timer(200);
        }
    }
}


void
menu::
draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, menu&  m) noexcept
{
  cv.fill_rectangle(m.m_window.m_color,
    m.m_window.x,
    m.m_window.y,
    m.m_window.w+(g_margin*3),
    m.m_window.h+(g_margin*2));


  int  y = m.m_window.y+g_margin;

    for(auto&  ent: m.m_entry_list)
    {
      auto  color = ent.m_active_flag? gbstd::colors::white
                   :                   gbstd::colors::gray;

      cv.draw_string(color,ent.m_label.data(),
        m.m_window.x+g_margin,y);

      y += gbstd::g_font_height;
    }


  cv.draw_canvas({gbstd::g_misc_image,0,0,24,24},
    m.m_window.x-12,
    m.m_window.y+g_margin+(gbstd::g_font_height*m.m_cursor_position)+4);
}




}




