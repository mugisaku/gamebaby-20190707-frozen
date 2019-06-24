#include"libww/ww_context.hpp"
#include"libww/ww_system.hpp"




namespace ww{




context::
context(system&  sys) noexcept:
m_system(sys)
{
}




void
context::
run(gbstd::execution&  exec, context&  ctx) noexcept
{
  exec.set_background_color(gbstd::colors::black);

  ctx.m_clock.set_name("standard")
    .set_id("standard")
    .set_permil(1000)
    .turnon();

  exec.add_clock(ctx.m_clock);

  exec.push_frame({
    {"*",start_display_logo,ctx},
    {"*", wait_display_logo,ctx},
  });
}


void
context::
start_display_logo(gbstd::execution&  exec, context&  ctx) noexcept
{
  auto&  spl = ctx.m_system.create_spilling_text();

  std::u16string_view  sv = u"Game Baby";

  auto  text_width = static_cast<int>(gbstd::g_font_width*sv.size());

  int  x_center = m_screen_width/2;
  int  y_center = m_screen_height/2;

  spl.set_text(sv)
    .set_color(gbstd::colors::yellow)
    .set_time(600)
    .reset({x_center-(text_width/2),y_center-8},-32);

  exec.add_task(spl.initialize_task(ctx.m_clock));

  ++exec;
}


void
context::
wait_display_logo(gbstd::execution&  exec, context&  ctx) noexcept
{
    if(!ctx.m_system.get_spilling_text_counter())
    {
      new(&ctx.m_front_section) front_section(ctx.m_system,exec);
    }
}



}




