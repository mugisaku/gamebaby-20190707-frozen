#include"libww/ww.hpp"




namespace ww{




void
enable_blink(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->m_blink_context.enable();

  ++exec;
}


void
disable_blink(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->m_blink_context.disable();

  ++exec;
}


void
move_to_advance(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->move_to_advance(4);

  ++exec;
}


void
move_to_back(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->move_to_back(4);

  ++exec;
}


void
set_white(gbstd::execution&  exec, act_context*  ctx) noexcept
{
  gbstd::string_form  sf;

  ctx->m_battle_context->push_message(sf("%sのこうげき\n",ctx->m_row->m_variable.get_name().data()),gbstd::colors::white);

  ctx->m_row->m_white_flag = true;

  ++exec;
}


void
unset_white(gbstd::execution&  exec, act_context*  ctx) noexcept
{
  ctx->m_row->m_white_flag = false;

  ++exec;
}


void
battle_context::
judge_hit(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  auto&   actor = *ctx->m_attack_context.m_actor;
  auto&  target = *ctx->m_attack_context.m_target;

  static gbstd::uniform_rand     hit_rand(  1,  10);
  static gbstd::uniform_rand  damage_rand(600,3000);

  gbstd::string_form  sf;

    if(hit_rand() <= 8)
    {
      int  damage_amount = damage_rand();

      ctx->push_message(sf("%sへ %4dのダメージ\n",target.m_variable.get_name().data(),damage_amount),gbstd::colors::white);

      target.m_variable.add_hp(-damage_amount);

      target.m_process.push({
        {enable_blink,&target,true},
        {move_to_back,&target,true},
        {move_to_back,&target,true},
        {move_to_advance,&target,true},
        {move_to_advance,&target,true},
        {disable_blink,&target,true}});


        if(target.m_variable.get_hp() <= 0)
        {
          ctx->push_message(sf("%sは　たおれた!\n\n",target.m_variable.get_name().data()),gbstd::colors::white);

          target.m_variable.set_hp(0);
        }
    }

  else
    {
      ctx->push_message(sf("ミス!\n\n"),gbstd::colors::white);
    }


  ++exec;
}


void
battle_context::
fight(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
START:
  auto  l = ctx->m_left_force.get_actor_by_ap();
  auto  r = ctx->m_right_force.get_actor_by_ap();

  auto  actor = (l && r)? (l->m_ap < r->m_ap)? r:l
               :(l     )? l
               :(r     )? r
               :nullptr;

  constexpr int  ap_max = 80;

    if(actor && (actor->m_ap >= ap_max))
    {
      exec.pop();


      force*       own = actor->m_force;
      force*  opponent = (own->m_side == side::left)? &ctx->m_right_force
                       :                              &ctx->m_left_force;

      ww::row*  buf[8] = {0};

      actor->m_ap -= ap_max;

      static gbstd::uniform_rand  rand;

      auto  n = opponent->get_rows_by_position(front_pos,buf,8);

        if(n >= 1)
        {
          rand.reset(0,n-1);

          auto  target = buf[rand()];

          ctx->m_attack_context.reset(*actor,*target);

           actor->m_process.set_interval(40);
          target->m_process.set_interval(40);


          ctx->m_act_context.m_row = actor;

          actor->m_process.push({{  set_white,&ctx->m_act_context,true},
                                 {unset_white,&ctx->m_act_context,true},
                                 {move_to_advance,actor,true},
                                 {move_to_advance,actor,true},
                                 {move_to_back,actor,true},
                                 {move_to_back,actor,true},
                                 });

          exec.push({{wait_for_players,ctx,true},
                     {judge_hit,ctx},
                     {wait_for_message,ctx,true}});

        }
    }

  else
    {
      constexpr int  v = 33;

      ctx->m_left_force.distribute_ap( v);
      ctx->m_right_force.distribute_ap(v);

      goto START;
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


  ++exec;
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


  ++exec;
}


void
battle_context::
judge(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  auto  l = ctx->m_left_force.can_continue_fight();
  auto  r = ctx->m_right_force.can_continue_fight();

    if(l && r)
    {
      exec.push({{fight,ctx,true}});

      return;
    }


  exec.replace({{start_battle,ctx,true}});

    if(l)
    {
      ctx->push_message("ひだりぐんの　しょうり\n",gbstd::colors::white);

      auto&  lf = ctx->m_left_force;

      exec.push({{move_to_up,&lf,true},
                 {move_to_up,&lf,true},
                 {move_to_down,&lf,true},
                 {move_to_down,&lf,true},
                 {move_to_up,&lf,true},
                 {move_to_up,&lf,true},
                 {move_to_down,&lf,true},
                 {move_to_down,&lf,true},
                 {move_to_up,&lf,true},
                 {move_to_up,&lf,true},
                 {move_to_down,&lf,true},
                 {move_to_down,&lf,true},
                 });
    }

  else
    if(r)
    {
      ctx->push_message("みぎぐんの　しょうり\n",gbstd::colors::white);

      auto&  rf = ctx->m_right_force;

      exec.push({{move_to_up,&rf,true},
                 {move_to_up,&rf,true},
                 {move_to_down,&rf,true},
                 {move_to_down,&rf,true},
                 {move_to_up,&rf,true},
                 {move_to_up,&rf,true},
                 {move_to_down,&rf,true},
                 {move_to_down,&rf,true},
                 {move_to_up,&rf,true},
                 {move_to_up,&rf,true},
                 {move_to_down,&rf,true},
                 {move_to_down,&rf,true},
                 });
    }

  else
    {
      ctx->push_message("りょうぐん　はいぼく\n",gbstd::colors::white);
    }
}




void
battle_context::
wait_for_message(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
    if(!ctx->m_typewriter)
    {
      ++exec;
    }
}


void
battle_context::
wait_for_players(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
    if(!ctx->m_number_of_busy_players)
    {
      ++exec;
    }
}


void
battle_context::
wait_for_press_p_key(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
    if(gbstd::g_input.test_p())
    {
      exec.replace({{judge,ctx,true}});

      ctx->ready();
    }
}




void
battle_context::
start_battle(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
    if(!gbstd::g_input.test_p())
    {
      exec.replace({{wait_for_press_p_key,ctx,true}});

      ctx->push_message("enterキーをおすと　かいし　します\n",gbstd::colors::white);
      ctx->push_message("",gbstd::colors::white);
    }
}




}




