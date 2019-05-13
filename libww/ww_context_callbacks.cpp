#include"libww/ww_context.hpp"




namespace ww{




void
context::
start(gbstd::execution&  exec, context&  ctx) noexcept
{
  exec.push({
//    {display_logo,ctx,gbstd::interruptions::on},
    {loop_battle,ctx,gbstd::interruptions::on},
  });
}


/*
void
context::
display_logo(gbstd::execution&  exec, context&  ctx) noexcept
{
  ++exec;
}
*/


void
context::
loop_battle(gbstd::execution&  exec, context&  ctx) noexcept
{
  exec.push({{start_battle,ctx,gbstd::interruptions::on}});
}


void
context::
start_battle(gbstd::execution&  exec, context&  ctx) noexcept
{
  ctx.m_left_party.clear();
  ctx.m_right_party.clear();

  ctx.m_left_party.push( {{"ひだりぐんA",800,},{"ひだりぐんB",800,},{"ひだりぐんC",800,},});
  ctx.m_right_party.push({{"みぎぐんA",800,},{"みぎぐんB",800,},{"みぎぐんC",800,},});

  ctx.s_battle.entry_party( ctx.m_left_party,battles::sides::left );
  ctx.s_battle.entry_party(ctx.m_right_party,battles::sides::right);


  ++exec;

  exec.push({
    {initialize_battle,ctx,gbstd::interruptions::on},
    {       run_battle,ctx,gbstd::interruptions::on},
    {  finalize_battle,ctx,gbstd::interruptions::on},
  });
}


void
context::
initialize_battle(gbstd::execution&  exec, context&  ctx) noexcept
{
  ++exec;
}


battle_result
context::
judge_battle() const noexcept
{
  int  l = 0;
  int  r = 0;

    for(auto&  c: s_battle.m_character_table)
    {
        if(c.can_continue_battle())
        {
            if(c.m_field.m_side.is_left()){++l;}
          else                            {++r;}
        }
    }


  return (l && r)? battle_result::continuing
        :(l     )? battle_result::left_won
        :(     r)? battle_result::right_won
        :          battle_result::draw;
}




bool
context::
judge_hit(const battles::character&  actor, const battles::character&  target) const noexcept
{
  static gbstd::uniform_rand  hit_rand(1,10);

  return (hit_rand() <= 8);
}


int
context::
calculate_damage(const battles::character&  actor, const battles::character&  target) const noexcept
{
  static gbstd::uniform_rand  damage_rand(80,1000);

  return damage_rand();
}


void
context::
distribute_ap(int  v) noexcept
{
    for(auto&  c: s_battle.m_character_table)
    {
        if(c.can_continue_battle())
        {
          c.m_ap += v;
        }
    }
}


battles::character*
context::
get_battle_character_by_ap() noexcept
{
  battles::character*  highest = nullptr;

    for(auto&  c: s_battle.m_character_table)
    {
        if(!highest || (c.m_ap > highest->m_ap))
        {
          highest = &c;
        }
    }


  return highest;
}


void
context::
fight(gbstd::execution&  exec) noexcept
{
  struct tester{
    battles::side  m_side;

    bool  operator()(const battles::character&  c) const noexcept
    {
      return (c.m_field.m_side == m_side);
    }

  };


//  auto&  q = s_battle.m_action_queue;

//  q.clear();

  s_battle.m_action_index = 0;

START:
  auto  actor = get_battle_character_by_ap();

  constexpr int  ap_max = 80;

    if(actor && (actor->m_ap >= ap_max))
    {
      actor->m_ap -= ap_max;

      static gbstd::uniform_rand  rand;

      auto  opposite_side = actor->m_field.m_side.get_opposite();

      auto&  table = s_battle.filter(tester{opposite_side});

      auto  n = table.size();

        if(n >= 1)
        {
          rand.reset(0,n-1);

          auto&  target = table[rand()];

//          auto  target_i = target.m_index;

/*
          q.emplace_back(battle_action_kind::attack,target_i);

            if(judge_hit(*actor,target))
            {
              auto  damage = calculate_damage(*actor,target);

              q.emplace_back(battle_action_kind::receive,0,target_i);
            }

          else
            {
              q.emplace_back(battle_action_kind::avoid,0,target_i);
            }
*/
        }


      exec.push({
        {wait_until_end_movie,*this,gbstd::interruptions::on},
      });
    }

  else
    {
      constexpr int  v = 33;

      distribute_ap( v);

      goto START;
    }
}


void
context::
run_battle(gbstd::execution&  exec, context&  ctx) noexcept
{
  struct tester{
    battles::side  m_side;

    bool  operator()(const battles::character&  c) const noexcept
    {
      return (c.m_field.m_side == m_side);
    }

  };


  auto  res = ctx.judge_battle();

    if(res == battle_result::continuing)
    {
      ctx.fight(exec);

      return;
    }


    if(res == battle_result::left_won)
    {
      auto&  table = ctx.s_battle.filter(tester{battles::sides::left});

        for(auto&  c: table)
        {
        }
    }

  else
    if(res == battle_result::right_won)
    {
      auto&  table = ctx.s_battle.filter(tester{battles::sides::right});

        for(auto&  c: table)
        {
        }
    }

  else
    {
    }


  ++exec;
}


void
context::
wait_until_end_movie(gbstd::execution&  exec, context&  ctx) noexcept
{
    if(ctx.s_battle.m_action_index >= ctx.s_battle.m_action_queue.size())
    {
      ++exec;

      return;
    }


  auto&  a = ctx.s_battle.m_action_queue[ctx.s_battle.m_action_index++];

    switch(a.get_kind())
    {
  case(battle_action_kind::null):
      break;
  case(battle_action_kind::attack):
      break;
  case(battle_action_kind::avoid):
      break;
  case(battle_action_kind::receive):
      break;
  default:;
    }
}


void
context::
finalize_battle(gbstd::execution&  exec, context&  ctx) noexcept
{

  ++exec;
}




}




