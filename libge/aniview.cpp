#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;


namespace{
const uint32_t  g_time_table[] = {1000,600,200,80};
}


void
aniview::
psh(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  ctx = *evt->get_userdata<context>();

      auto&  pt = ctx.m_current_index;

      int  w = ctx.m_cell_width ;
      int  h = ctx.m_cell_height;

      ctx.m_aniview->m_frames.emplace_back(ctx.m_source_image,w*pt.x,h*pt.y,w,h);
      ctx.m_aniview->m_points.emplace_back(pt);

      ctx.m_aniview->update_state_label();
    }
}


void
aniview::
pop(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  ctx = *evt->get_userdata<context>();

        if(ctx.m_aniview->m_frames.size())
        {
          ctx.m_aniview->m_frames.pop_back();
          ctx.m_aniview->m_points.pop_back();

          ctx.m_aniview->update_state_label();
        }
    }
}


void
aniview::
dial_callback(dial_event  evt) noexcept
{
  auto&  ctx = *evt->get_userdata<context>();

  ctx.m_aniview->m_delay_time = g_time_table[evt.get_new_value()-1];
}




aniview::
aniview(context&  ctx) noexcept:
widget(ctx.m_cell_width,ctx.m_cell_height)
{
  auto  d = new dial(1,4,dial_callback);

  auto  psh_btn = new button(new label(u"Push",colors::black),psh);
  auto  pop_btn = new button(new label( u"Pop",colors::black),pop);

  gbstd::set_userdata({this,d,psh_btn,pop_btn},&ctx);

  auto  op_col = make_column({psh_btn,pop_btn});

  m_state_label = new label(u"--/ 0",colors::black);

  m_delay_time = g_time_table[0];
  m_next_time  =               0;

  auto  speed_frame = new frame("speed",d);

  auto  urow = make_row({m_state_label,op_col});

  append_column_child({urow,speed_frame});

  clear();
}




void
aniview::
clear() noexcept
{
  m_index = 0;

  update_state_label();

  request_redraw();
}


void
aniview::
check() noexcept
{
    if(m_frames.size())
    {
        if(g_time >= m_next_time)
        {
          m_next_time = (g_time+m_delay_time);

            if(++m_index >= m_frames.size())
            {
              m_index = 0;
            }


          update_state_label();

          request_redraw();
        }
    }
}


void
aniview::
rebase(const gbstd::image&  img) noexcept
{
  m_frames.clear();

  auto&  ctx = *get_userdata<context>();

  int  w = ctx.m_cell_width ;
  int  h = ctx.m_cell_height;

    for(auto&  pt: m_points)
    {
      m_frames.emplace_back(img,w*pt.x,h*pt.y,w,h);
    }
}


void
aniview::
update_state_label() noexcept
{
    if(m_frames.empty())
    {
      m_state_label->set_text("--/ 0");
    }

  else
    {
      string_form  sf;

      m_state_label->set_text(sf("%2d/%2d",m_index+1,m_frames.size()));
    }
}


void
aniview::
render(const canvas&  cv) noexcept
{
  auto&  ctx = *get_userdata<context>();

  ctx.m_core->render_underlay(1,cv);

    if(m_index < m_frames.size())
    {
      cv.draw_canvas(m_frames[m_index],0,0);
    }
}


}




