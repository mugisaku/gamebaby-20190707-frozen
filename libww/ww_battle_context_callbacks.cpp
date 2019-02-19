#include"libww/ww_battle_context.hpp"




namespace ww{




void
enable_blink(gbstd::execution&  exec, ww::company*  company) noexcept
{
  company->get_blink_context().enable();

  ++exec;
}


void
disable_blink(gbstd::execution&  exec, ww::company*  company) noexcept
{
  company->get_blink_context().disable();

  ++exec;
}


void
move_to_advance(gbstd::execution&  exec, ww::company*  company) noexcept
{
  company->move_to_advance(4);

  ++exec;
}


void
move_to_back(gbstd::execution&  exec, ww::company*  company) noexcept
{
  company->move_to_back(4);

  ++exec;
}


void
set_white(gbstd::execution&  exec, act_context*  ctx) noexcept
{
  ctx->m_company->set_white_flag();

  ++exec;
}


void
unset_white(gbstd::execution&  exec, act_context*  ctx) noexcept
{
  ctx->m_company->unset_white_flag();

  ++exec;
}


void
battle_context::
judge_hit(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  auto&   actor = *ctx->m_attack_context.m_actor;
  auto&  target = *ctx->m_attack_context.m_target;

  static gbstd::uniform_rand     hit_rand( 1,  10);
  static gbstd::uniform_rand  damage_rand(80,1000);

  gbstd::string_form  sf;

    if(hit_rand() <= 8)
    {
      int  damage_amount = damage_rand();

      ctx->m_notifiers.emplace_back(target.get_current_position(),sf("%4d",damage_amount));

      target.get_variable().add_hp(-damage_amount);

      target.reset_hp_bar();

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


  auto  actor = ctx->get_company_by_ap();

  constexpr int  ap_max = 80;

    if(actor && (actor->get_ap() >= ap_max))
    {
      actor->add_ap(-ap_max);

      static gbstd::uniform_rand  rand;

      company*  buf[12];

      auto  opposite_side = actor->get_tag().get_opposite();

      auto  n = ctx->get_companies_by_side(opposite_side,std::begin(buf),std::end(buf));

//      auto  n = opponent->get_companys_by_position(front_pos,buf,8);

        if(n >= 1)
        {
          rand.reset(0,n-1);

          auto  target = buf[rand()];

          ctx->m_attack_context.reset(*actor,*target);

          ctx->m_act_context.m_company = actor;

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

      ctx->distribute_ap( v);

      goto START;
    }
}


void
battle_context::
branch_by_judge(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  auto  res = ctx->judge();

    if(res == battle_context::result::continuing)
    {
      exec.push({{fight,ctx,true}});

      return;
    }


/*
  exec.replace({{wait_for_all,ctx,true},
                {start_battle,ctx,true},
               });
*/
    if(res == battle_context::result::left_won)
    {
      ctx->push_message("ひだりぐんの　しょうり\n",gbstd::colors::white);
    }

  else
    if(res == battle_context::result::right_won)
    {
      ctx->push_message("みぎぐんの　しょうり\n",gbstd::colors::white);
    }

  else
    {
      ctx->push_message("りょうぐん　はいぼく\n",gbstd::colors::white);
    }


  ++exec;
}




void
battle_context::
wait_for_all(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  ctx->pump_text();

    if(!ctx->m_typewriter)
    {
        if(!ctx->m_number_of_playing_companies)
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
//  ctx->m_time_add_amount.set(80);

  ++exec;
}


void
battle_context::
stop_time(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
//  ctx->m_time_add_amount.set(0);

  ++exec;
}


void
battle_context::
wait_for_players(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
    if(!ctx->m_number_of_playing_companies)
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




}




