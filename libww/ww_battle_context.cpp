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
  gbstd::push_major_task_list(m_task_list);

  gbstd::set_major_painter_list(&m_painter_list);

  auto&  tskls = m_task_list;
  auto&  pails = m_painter_list;

  tskls.clear();
  pails.clear();

  pails.emplace_back(*this,render);

    for(auto&  c: m_company_list)
    {
      c.startup();
    }


  m_text.fill(gbstd::character());

  m_typewriter.set_cursor_position({0,0});

  m_command_request = true;

  m_result = result::continuing;

  tskls.push(80,m_process);
  tskls.push(drive,80,this);
  pails.emplace_back(*this,render);

  m_process.assign({{wait_for_players,this,true},
                    {branch_by_judge,this,true},
                  });
}


void
battle_context::
cleanup() noexcept
{
    for(auto&  c: m_company_list)
    {
      c.cleanup();
    }


  gbstd::pop_major_task_list();
}




void
battle_context::
push_entry(entry&  ent, battle_side  side, gbstd::color  color) noexcept
{
  gbstd::point  offset(m_field_width/2,0);

  m_company_list.emplace_back(m_number_of_playing_companies);

  auto&  c = m_company_list.back();


  int&  i = side.is_left()? m_number_of_left_companies
           :                m_number_of_right_companies;

  c.set_tag(side,i++,color);

  c.set_offset({offset.x,offset.y});

  c.set_entry(ent);

  ++m_number_of_total_companies;
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


filtering_table<company,16>&
battle_context::
reset_filtering_table() noexcept
{
  return m_filtering_table.assign(m_company_list.begin(),m_company_list.end());
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
drive(gbstd::task_control&  ctrl, battle_context*  ctx) noexcept
{
  auto  it = ctx->m_notifiers.begin();

    while(it != ctx->m_notifiers.end())
    {
        if(it->is_halted())
        {
          it = ctx->m_notifiers.erase(it);
        }

      else
        {
          it->step();

          ++it;
        }
    }
}


void
battle_context::
render(const battle_context&  ctx, const gbstd::canvas&  cv) noexcept
{
    if(ctx.m_display_flags&display_flags::show_text)
    {
      static constexpr gbstd::point  console_pos = gbstd::point(80,160);

      cv.draw_typewriter(ctx.m_typewriter,console_pos);
    }


    for(auto&  nt: ctx.m_notifiers)
    {
      nt.render(cv);
    }
}




}




