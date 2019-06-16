#include"libww/ww_context.hpp"




namespace ww{




void
context::
run(gbstd::execution&  exec, context&  ctx) noexcept
{
  exec.push({
    {"*",start_display_logo,ctx},
    {"*", wait_display_logo,ctx},
  });
}


void
context::
start_display_logo(gbstd::execution&  exec, context&  ctx) noexcept
{
  ctx.push_spilling_text(gbstd::colors::yellow,u"Game Baby",{m_screen_width/2,m_screen_height/2},1000);

  ++exec;
}


void
context::
wait_display_logo(gbstd::execution&  exec, context&  ctx) noexcept
{
    if(!ctx.has_active_spilling_text())
    {
//      ctx.s_manage.reset();

      ++exec;
    }
}


}




