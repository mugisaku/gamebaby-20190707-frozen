#include"libww/ww_executive_context.hpp"




namespace ww{




void
executive_context::
initialize(gbstd::execution&  exec, executive_context*  ctx) noexcept
{
  ++exec;
}


void
executive_context::
loop_battle(gbstd::execution&  exec, executive_context*  ctx) noexcept
{
  ctx->m_process.push({
                        {enter_battle,ctx,true},
                     });
}


void
executive_context::
enter_battle(gbstd::execution&  exec, executive_context*  ctx) noexcept
{
  ctx->m_battle_context.startup();

  ++exec;
}


void
executive_context::
exit_battle(gbstd::execution&  exec, executive_context*  ctx) noexcept
{
  ++exec;
}




}




