#include"libww/ww.hpp"




namespace ww{




void
enable_blink(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->m_blink_context.enable();

  exec.pop();
}


void
disable_blink(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->m_blink_context.disable();

  exec.pop();
}


void
move_to_advance(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->move_to_advance(4);

  exec.pop();
}


void
move_to_back(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->move_to_back(4);

  exec.pop();
}


void
set_white(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->m_white_flag = true;

  exec.pop();
}


void
unset_white(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->m_white_flag = false;

  exec.pop();
}


void
judge_hit(gbstd::execution&  exec, attack_context*  ctx) noexcept
{
  auto&   actor = *ctx->m_actor;
  auto&  target = *ctx->m_target;

  static gbstd::uniform_rand     hit_rand(  1,  10);
  static gbstd::uniform_rand  damage_rand(600,3000);

    if(hit_rand() <= 8)
    {
      target.m_variable.m_hp -= damage_rand();

        if(target.m_variable.m_hp <= 0)
        {
          target.m_variable.m_hp = 0;
        }

      else
        {
          target.m_process.push_reverse(
            {enable_blink,
             move_to_back,
             move_to_back,
             move_to_advance,
             move_to_advance,
             disable_blink},&target,true);
        }
    }


  exec.pop();
}


void
wait_for_finish_to_attack(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  auto&  btlctx = ctx->m_attack_context;

  auto&   actor = *btlctx.m_actor;
  auto&  target = *btlctx.m_target;

    if(!gbstd::wait({&actor.m_process,&target.m_process}))
    {
      exec.pop();
    }
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

           actor.m_process.set_interval(40);
          target.m_process.set_interval(40);

          actor.m_process.push({unset_white,&actor}     );
          actor.m_process.push({  set_white,&actor},true);

          actor.m_process.push({judge_hit,&ctx->m_attack_context});


          exec.push({wait_for_finish_to_attack,ctx},true);
        }
    }

  else
    {
      exec.pop();
    }
}


void
move_to_up(gbstd::execution&  exec, force*  f) noexcept
{
    for(auto&  row: f->m_rows)
    {
        if(row.m_variable.get_hp())
        {
          row.m_current_pos.y -= 2;
        }
    }


  exec.pop();
}


void
move_to_down(gbstd::execution&  exec, force*  f) noexcept
{
    for(auto&  row: f->m_rows)
    {
        if(row.m_variable.get_hp())
        {
          row.m_current_pos.y += 2;
        }
    }


  exec.pop();
}


void
start(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
}


void
judge(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  auto  l = ctx->m_left_force.can_continue_fight();
  auto  r = ctx->m_right_force.can_continue_fight();

    if(l && r)
    {
      exec.push({fight,ctx});
    }

  else
    if(l)
    {
      auto&  lf = ctx->m_left_force;

      exec.replace({start,ctx},true);

      exec.push({move_to_up,move_to_up,move_to_down,move_to_down,
                 move_to_up,move_to_up,move_to_down,move_to_down,
                 move_to_up,move_to_up,move_to_down,move_to_down,
                 },&lf,true);

      exec.pop();
    }

  else
    if(r)
    {
      auto&  rf = ctx->m_right_force;

      exec.replace({start,ctx},true);

      exec.push({move_to_up,move_to_up,move_to_down,move_to_down,
                 move_to_up,move_to_up,move_to_down,move_to_down,
                 move_to_up,move_to_up,move_to_down,move_to_down,
                 },&rf,true);

      exec.pop();
    }

  else
    {
      exec.pop();
    }
}




}




