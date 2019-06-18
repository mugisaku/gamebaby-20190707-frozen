#include"libww/ww_context.hpp"
#include"libww/ww_system.hpp"




namespace ww{




context::
context(system&  sys) noexcept:
m_system(sys)
{
}




gbstd::execution_context
g_exectx;


void
context::
run(gbstd::execution&  exec, context&  ctx) noexcept
{
  ctx.m_clock.set_name("standard")
    .set_id("standard")
    .set_permil(1000)
    .turnon();

  exec.add_clock(ctx.m_clock);

  exec.push({
    {"*",start_display_logo,ctx},
    {"*", wait_display_logo,ctx},
  });
}


void
context::
start_display_logo(gbstd::execution&  exec, context&  ctx) noexcept
{
  auto  v = exec.set_jump(g_exectx);

    if(v == 0)
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

  else
    if(v == 1)
    {
      exec.unset_jump();
    }
}


void
context::
wait_display_logo(gbstd::execution&  exec, context&  ctx) noexcept
{
    if(!ctx.m_system.get_spilling_text_counter())
    {
exec.jump(g_exectx,1);

//      ++exec;
    }
}



}




