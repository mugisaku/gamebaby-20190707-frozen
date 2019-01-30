#include"libww/ww.hpp"




namespace ww{




void
enable_blink(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->get_blink_context().enable();

  ++exec;
}


void
disable_blink(gbstd::execution&  exec, ww::row*  row) noexcept
{
  row->get_blink_context().disable();

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
  ctx->m_row->set_white_flag();

  ++exec;
}


void
unset_white(gbstd::execution&  exec, act_context*  ctx) noexcept
{
  ctx->m_row->unset_white_flag();

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

      ctx->m_notifiers.emplace_back(target.get_current_position(),sf("%4d",damage_amount));

      target.get_variable().add_hp(-damage_amount);

      target.get_process().push({
        {enable_blink,&target,true},
        {move_to_back,&target,true},
        {move_to_back,&target,true},
        {move_to_advance,&target,true},
        {move_to_advance,&target,true},
        {disable_blink,&target,true}});


        if(target.get_variable().get_hp() <= 0)
        {
          target.get_variable().set_hp(0);
        }
    }

  else
    {
      ctx->m_notifiers.emplace_back(target.get_current_position(),"ミス!");
    }


  ++exec;
}


void
battle_context::
open_command_window(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
    if(gbstd::g_input.test_p())
    {
      ctx->reset_text();

      ++exec;
    }
}


void
battle_context::
show_message(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  ctx->show_text();

  ++exec;
}


void
battle_context::
hide_message(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  ctx->hide_text();

  ++exec;
}


void
battle_context::
fight(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
START:
    if(gbstd::g_input.test_n())
    {
      exec.push({{wait_for_all,ctx,true},
                 {hide_message,ctx},
                 {open_command_window,ctx,true},
                 {show_message,ctx},
               });

      return;
    }


  auto  l = ctx->m_left_force.get_actor_by_ap();
  auto  r = ctx->m_right_force.get_actor_by_ap();

  auto  actor = (l && r)? (l->get_ap() < r->get_ap())? r:l
               :(l     )? l
               :(r     )? r
               :nullptr;

  constexpr int  ap_max = 80;

    if(actor && (actor->get_ap() >= ap_max))
    {
      force*       own = actor->get_force();
      force*  opponent = (*own == battle_side::left)? &ctx->m_right_force
                       :                              &ctx->m_left_force;

      ww::row*  buf[8] = {0};

      actor->add_ap(-ap_max);

      static gbstd::uniform_rand  rand;

      auto  n = opponent->get_rows_by_position(front_pos,buf,8);

        if(n >= 1)
        {
          rand.reset(0,n-1);

          auto  target = buf[rand()];

          ctx->m_attack_context.reset(*actor,*target);

          ctx->m_act_context.m_row = actor;

          actor->get_process().push({{  set_white,&ctx->m_act_context,true},
                                 {unset_white,&ctx->m_act_context,true},
                                 {move_to_advance,actor,true},
                                 {move_to_advance,actor,true},
                                 {move_to_back,actor,true},
                                 {move_to_back,actor,true},
                               });

          exec.replace({{wait_for_all,ctx,true},
                        {judge_hit,ctx},
                        {wait_for_all,ctx,true},
                      });
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
    for(auto&  row: *f)
    {
        if(row.get_variable().get_hp())
        {
          row.add_y_position(-2);
        }
    }


  ++exec;
}


void
move_to_down(gbstd::execution&  exec, force*  f) noexcept
{
    for(auto&  row: *f)
    {
        if(row.get_variable().get_hp())
        {
          row.add_y_position(2);
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


  exec.replace({{wait_for_all,ctx,true},
                {start_battle,ctx,true},
              });

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
wait_for_all(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  ctx->step_players();

  ctx->pump_text();

    if(!ctx->m_typewriter)
    {
      ctx->update_number_of_busy_players();

        if(!ctx->m_number_of_busy_players)
        {
          ++exec;
        }
    }
}


void
battle_context::
wait_for_message(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  ctx->pump_text();

    if(!ctx->m_typewriter)
    {
      ++exec;
    }
}


void
battle_context::
start_time(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  ctx->m_time_add_amount = ctx->m_time_standard.get();

  ++exec;
}


void
battle_context::
stop_time(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  ctx->m_time_add_amount = 0;

  ++exec;
}


void
battle_context::
wait_for_players(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  ctx->step_players();

  ctx->update_number_of_busy_players();

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
      ++exec;
    }
}




void
battle_context::
start_battle(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
    if(!gbstd::g_input.test_p())
    {
      exec.replace({{wait_for_message,ctx,true},
                    {wait_for_press_p_key,ctx,true},
                    {start_time,ctx,true},
                    {wait_for_players,ctx,true},
                    {judge,ctx,true},
                  });

      ctx->push_message("enterキーをおすと　かいし　します\n",gbstd::colors::white);
 
      ctx->pump_text_all();
    }
}


void
battle_context::
update_number_of_busy_players() noexcept
{
  m_number_of_busy_players = 0;

    for(auto&  row: m_left_force)
    {
      auto&  proc = row.get_process();

        if(proc.is_busy() || !row.is_motion_finished())
        {
          ++m_number_of_busy_players;
        }
    }


    for(auto&  row: m_right_force)
    {
      auto&  proc = row.get_process();

        if(proc.is_busy() || !row.is_motion_finished())
        {
          ++m_number_of_busy_players;
        }
    }
}


void
battle_context::
step_players() noexcept
{
  m_left_force.step();
  m_right_force.step();
}


void
battle_context::
pump_text() noexcept
{
    if(m_typewriter)
    {
      m_typewriter.pump();

      int  n = 4;

        if(m_typewriter && gbstd::g_input.test_p() && n)
        {
          m_typewriter.pump();

          --n;
        }
    }
}


void
battle_context::
pump_text_all() noexcept
{
    while(m_typewriter)
    {
      m_typewriter.pump();
    }
}




}




