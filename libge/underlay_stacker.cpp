#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;




void
underlay_stacker::
psh(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  ctx = *evt->get_userdata<context>();

      ctx.m_display.push_underlay(ctx.m_source_image,ctx.m_current_index,ctx.get_cell_width(),ctx.get_cell_height());

      ctx.m_underlay_stacker->update_counter_label();

      ctx.m_core->request_redraw();
    }
}


void
underlay_stacker::
pop(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  ctx = *evt->get_userdata<context>();

        if(ctx.m_core->get_display().get_number_of_underlays())
        {
          ctx.m_core->get_display().pop_underlay();

          ctx.m_underlay_stacker->update_counter_label();

          ctx.m_core->request_redraw();
        }
    }
}


void
underlay_stacker::
swi(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  ctx = *evt->get_userdata<context>();

        if(ctx.m_core->get_display().test_whether_show_underlay())
        {
          ctx.m_core->get_display().hide_underlay();

          ctx.m_underlay_stacker->m_switching_label->modify_text(u"show");
        }

      else
        {
          ctx.m_core->get_display().show_underlay();

          ctx.m_underlay_stacker->m_switching_label->modify_text(u"hide");
        }


      ctx.m_core->request_redraw();
    }
}




underlay_stacker::
underlay_stacker(context&  ctx) noexcept
{
  m_switching_label = new label(u"hide",colors::black);
  m_counter_label   = new label(u" 0",colors::white);


  auto  psh_btn = new button(new label(u"push",colors::black),psh);
  auto  pop_btn = new button(new label(u"pop",colors::black),pop);
  auto  swi_btn = new button(m_switching_label,swi);

  gbstd::set_userdata({this,psh_btn,pop_btn,swi_btn},&ctx);


  append_column_child({m_counter_label,psh_btn,pop_btn,swi_btn});
}




void
underlay_stacker::
update_counter_label() noexcept
{
  auto&  ctx = *get_userdata<context>();

  string_form  sf;

  m_counter_label->set_text(sf("%2d",ctx.m_core->get_display().get_number_of_underlays()));
}




}




