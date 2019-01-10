#include"libww/ww.hpp"




namespace ww{




int
force::
get_rows_by_position(battle_position  pos, row**  buf, int  n) noexcept
{
  int  c = 0;

    for(auto&  row: m_rows)
    {
        if(!n)
        {
          break;
        }


        if(row.is_surviving() && (row.m_variable.get_position() == pos))
        {
          *buf++ = &row;

          --n;
          ++c;
        }
    }


  return c;
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

  int  y = g_frame_h;

    for(int  i = 0;  i < m_number_of_rows;  ++i)
    {
      auto&  row = m_rows[i];

      row.reset(*this,(i < init.m_company_list.size())? init.m_company_list[i]:nullptr);


      auto  pos = row.m_original->get_position();

      row.m_base_pos.y = y             ;
                         y += g_frame_h;

      row.m_current_pos.y = row.m_base_pos.y;
    }
}


void
force::
update_total_hp() noexcept
{
  m_total_hp = 0;

    for(auto&  row: m_rows)
    {
        if(row.is_surviving())
        {
          m_total_hp += row.m_variable.get_hp();
        }
    }
}


void
force::
ready() noexcept
{
    for(int  i = 0;  i < m_number_of_rows;  ++i)
    {
      m_rows[i].ready();
    }
}


row*
force::
get_actor_by_ap() noexcept
{
  row*  highest = nullptr;

    for(auto&  row: m_rows)
    {
        if(row.is_surviving())
        {
            if(!highest || (highest->m_ap < row.m_ap))
            {
              highest = &row;
            }
        }
    }


  return highest;
}


void
force::
distribute_ap(int  v) noexcept
{
    for(auto&  row: m_rows)
    {
        if(row.is_surviving())
        {
          row.m_ap += v;
        }
    }
}


void
force::
step() noexcept
{
    for(auto&  row: m_rows)
    {
      row.step();
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




