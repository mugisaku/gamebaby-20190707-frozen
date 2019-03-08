#include"libww/ww_battle_context.hpp"
#include"sdl.hpp"




namespace ww{




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
      sdl::beep(40);

      int  damage_amount = damage_rand();

      ctx->m_notifiers.emplace_back(target.get_offset(),target.get_current_position(),sf("%4d",damage_amount));

      target.add_damage_motion();

      target.get_variable().add_hp(-damage_amount);

      target.reset_hp_bar();

        if(target.get_variable().get_hp() <= 0)
        {
          target.get_variable().set_hp(0);
          target.set_ap(0);
        }
    }

  else
    {
      ctx->m_notifiers.emplace_back(target.get_offset(),target.get_current_position(),"ミス!");
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
  struct tester{
    battle_side  m_side;

    bool  operator()(const company&  c) const noexcept
    {
      return c.is_surviving() && (c.get_tag() == m_side);
    }

  };


START:
/*
    if(gbstd::g_input.test_n())
    {
      exec.push({{wait_for_all,ctx,true},
                 {hide_message,ctx},
                 {open_command_window,ctx,true},
                 {show_message,ctx},
               });

      return;
    }
*/


  auto  actor = ctx->get_company_by_ap();

  constexpr int  ap_max = 80;

    if(actor && (actor->get_ap() >= ap_max))
    {
      actor->add_ap(-ap_max);

      static gbstd::uniform_rand  rand;

      auto  opposite_side = actor->get_tag().get_opposite();

      auto&  table = ctx->reset_filtering_table();

      auto  n = table.filter(tester{opposite_side});

        if(n >= 1)
        {
          rand.reset(0,n-1);

          auto&  target = table[rand()];

          ctx->m_attack_context.reset(*actor,target);

          ctx->m_act_context.m_company = actor;

          actor->add_attack_motion();

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
  struct tester{
    battle_side  m_side;

    bool  operator()(const company&  c) const noexcept
    {
      return c.is_surviving() && (c.get_tag() == m_side);
    }

  };


  auto  res = ctx->judge();

    if(res == battle_context::result::continuing)
    {
      exec.push({{fight,ctx,true}});

      return;
    }


  exec.replace({
                {wait_for_players,ctx,true},
                {exit,ctx,true},
               });

    if(res == battle_context::result::left_won)
    {
      auto&  table = ctx->reset_filtering_table();

      table.filter(tester{battle_sides::left});

        for(auto&  c: table)
        {
          c->add_jump_motion();
          c->add_jump_motion();
        }


      ctx->push_message("ひだりぐんの　しょうり\n",gbstd::colors::white);
    }

  else
    if(res == battle_context::result::right_won)
    {
      auto&  table = ctx->reset_filtering_table();

      table.filter(tester{battle_sides::right});

        for(auto&  c: table)
        {
          c->add_jump_motion();
          c->add_jump_motion();
        }


      ctx->push_message("みぎぐんの　しょうり\n",gbstd::colors::white);
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


void
battle_context::
exit(gbstd::execution&  exec, battle_context*  ctx) noexcept
{
  ctx->cleanup();

  ++exec;
}




}




