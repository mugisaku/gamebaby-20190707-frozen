#include"libww/ww_context.hpp"




namespace ww{




context::
context() noexcept
{
}




void
context::
push_spilling_text(gbstd::color  color, std::u16string_view  sv, gbstd::point  center, uint32_t  time) noexcept
{
  auto  ptr = spilling_text::produce(m_spilling_text_counter);

  auto  text_width = static_cast<int>(gbstd::g_font_width*sv.size());

  ptr->set_text(sv)
      .set_color(color)
      .set_time(time)
      .reset({center.x-(text_width/2),center.y-8},-32);
/*

  m_task_list.push(*ptr).set_draw<spilling_text>()
                        .set_tick<spilling_text>(m_clock_control,20)
                        .set_collect<spilling_text>();
*/
}


void
context::
tick(context&  ctx) noexcept
{
}


void
context::
draw(const gbstd::canvas&  cv, context&  ctx) noexcept
{
}




}




