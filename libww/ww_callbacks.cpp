#include"libww/ww.hpp"




namespace ww{




void
attack(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  auto&  btlctx = ctx->m_attack_context;

  auto&   actor = *btlctx.m_actor;
  auto&  target = *btlctx.m_target;

    if(actor.m_animation_phase == 0)
    {
        if(actor.m_animation_counter++ < 2)
        {
          actor.move_to_advance(2);
        }

      else
        {
          actor.reset_animation(1);
        }
    }


  exec.pop();
}


void
fight(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
    if(ctx->m_actor_queue.empty())
    {
      ctx->pump_queue();
    }


    if(ctx->m_actor_queue.size())
    {
      auto&  actor = *ctx->m_actor_queue.front();

      ctx->m_actor_queue.pop_front();

      actor.m_entry_flag = false;

      force*       own = actor.m_force;
      force*  opponent = (own->m_side == side::left)? &ctx->m_right_force
                       :                              &ctx->m_left_force;

      ww::row*  buf[8] = {0};

      static gbstd::uniform_rand  rand;

      auto  n = opponent->get_rows(front_pos,buf,8);

        if(n >= 1)
        {
          rand.reset(0,n-1);

          auto&  target = *buf[rand()];

          ctx->m_attack_context.reset(actor,target);

          exec.push({"",attack,ctx});
        }
    }

  else
    {
      exec.pop();
    }
}


void
judge(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  auto  l = ctx->m_left_force.can_continue_fight();
  auto  r = ctx->m_right_force.can_continue_fight();

    if(l && r)
    {
      exec.push({"",fight,ctx});
    }

  else
    {
      exec.clear();
    }
}




}




