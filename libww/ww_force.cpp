#include"libww/ww.hpp"




namespace ww{




int
force::
get_rows(battle_position  pos, row**  buf, int  n) noexcept
{
  int  c = 0;

    for(auto&  row: m_rows)
    {
        if(!n)
        {
          break;
        }


        if(row.is_surviving() && (row.m_variable.m_pos == pos))
        {
          *buf++ = &row;

          --n;
          ++c;
        }
    }


  return c;
}


void
force::
increase_ap() noexcept
{
    for(auto&  row: m_rows)
    {
        if(!row.m_entry_flag && row.is_surviving())
        {
          static gbstd::uniform_rand  rand(1,20);

          constexpr int  max = 200;

          row.m_ap += rand();

            if(row.m_ap >= max)
            {
              row.m_ap -= max;

              m_actor_queue.emplace_back(&row);

              row.m_entry_flag = true;
            }
        }
    }
}


bool
force::
can_continue_fight() const noexcept
{
    for(auto&  row: m_rows)
    {
        if(row.is_surviving())
        {
          return true;
        }
    }


  return false;
}


void
force::
reset(ww::side  side, const force_initializer&  init) noexcept
{
  m_side  = side;
  m_color = init.m_color;

  m_actor_queue.clear();

  int  y = g_frame_h;

    for(int  i = 0;  i < m_number_of_rows;  ++i)
    {
      auto&  row = m_rows[i];

      row.reset(*this,(i < init.m_company_list.size())? init.m_company_list[i]:nullptr);

      auto  pos = row.m_variable.m_pos;

      row.m_base_pos.x = g_frame_w*pos;


      row.m_base_pos.y = y             ;
                         y += g_frame_h;

      row.m_current_pos.y = row.m_base_pos.y;
    }
}


void
force::
update() noexcept
{
    for(auto&  row: m_rows)
    {
      row.m_blink_context.step();
    }
}


void
force::
render(const gbstd::canvas&  cv) const noexcept
{
    for(auto&  row: m_rows)
    {
      row.render(cv);
    }
}



}




