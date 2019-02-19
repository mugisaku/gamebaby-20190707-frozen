#include"libww/ww_battle_context.hpp"




namespace ww{




battle_context::
battle_context() noexcept
{
  m_text.resize(22,6);

  m_typewriter.assign(m_text);

  m_act_context.m_battle_context    = this;
  m_attack_context.m_battle_context = this;
}




void
battle_context::
set_field_size(int  w, int  h) noexcept
{
  m_field_width  = w;
  m_field_height = h;
}


void
battle_context::
reset_text() noexcept
{
  m_text.fill(gbstd::character());

  m_typewriter.set_cursor_position({0,0});
}


void
battle_context::
startup() noexcept
{
  m_text.fill(gbstd::character());

  m_typewriter.set_cursor_position({0,0});

  m_command_request = true;

  m_result = result::continuing;

  m_process.assign({{wait_for_players,this,true},
                    {branch_by_judge,this,true},
                  });
}


void
battle_context::
cleanup() noexcept
{
}




void
battle_context::
push_entry(entry&  ent, battle_side  side) noexcept
{
  m_company_list.emplace_back(m_number_of_playing_companies);

  auto&  c = m_company_list.back();

  c.reset(ent);


  int&  i = side.is_left()? m_number_of_left_companies
           :                m_number_of_right_companies;

  c.set_tag(side,i++,gbstd::colors::red);
}


void
battle_context::
distribute_ap(int  v) noexcept
{
    for(auto&  c: m_company_list)
    {
        if(c.is_surviving())
        {
          c.add_ap(v);
        }
    }
}


battle_context::result
battle_context::
judge() const noexcept
{
  int  l = 0;
  int  r = 0;

    for(auto&  c: m_company_list)
    {
        if(c.is_surviving())
        {
            if(c.get_tag().is_left()){++l;}
          else                       {++r;}
        }
    }


  return (l && r)? result::continuing
        :(l     )? result::left_won
        :(     r)? result::right_won
        :          result::draw;
}


company*
battle_context::
get_company_by_ap() noexcept
{
  company*  highest = nullptr;

    for(auto&  c: m_company_list)
    {
        if(!highest || (c.get_ap() > highest->get_ap()))
        {
          highest = &c;
        }
    }


  return highest;
}


int
battle_context::
get_companies_by_side(battle_side  side, company**  begin, company**  end) noexcept
{
  int  n = 0;

    for(auto&  c: m_company_list)
    {
        if(c.get_tag() == side)
        {
            if(begin >= end)
            {
              break;
            }


          *begin++ = &c;

          ++n;
        }
    }


  return n;
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




void
battle_context::
step_notifiers() noexcept
{
  auto  it = m_notifiers.begin();

    while(it != m_notifiers.end())
    {
      auto&  n = *it;

        if(!n.is_halted())
        {
          n.step();

          ++it;
        }

      else
        {
          it = m_notifiers.erase(it);
        }
    }
}


void
battle_context::
step() noexcept
{
  step_notifiers();

  m_process.step();
}


void
battle_context::
render(const gbstd::canvas&  cv) const noexcept
{
  gbstd::point  offset(m_field_width/2,0);

    for(auto&  c: m_company_list)
    {
      c.render(offset,cv);
    }


    if(m_display_flags&display_flags::show_text)
    {
      static constexpr gbstd::point  m_console_pos = gbstd::point(80,160);

      cv.draw_typewriter(m_typewriter,m_console_pos);
    }


    for(auto&  n: m_notifiers)
    {
      n.render(offset,cv);
    }
}




}




