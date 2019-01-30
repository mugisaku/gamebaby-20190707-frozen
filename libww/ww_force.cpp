#include"libww/ww.hpp"




namespace ww{




force::
force(battle_side  side, time_wrapper  tm) noexcept:
m_side(side),
m_time(tm)
{
    for(int  i = 0;  i < m_number_of_rows;  ++i)
    {
      auto&  row = m_rows[i];

      row.set_force(*this,counter(m_moving_row_count),i);

      row.set_time(tm);
    }
}


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


        if(row.is_surviving() && (row.get_variable().get_position() == pos))
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
reset() noexcept
{
    for(int  i = 0;  i < m_number_of_rows;  ++i)
    {
      m_rows[i].reset();
    }
}


void
force::
reset(const force_initializer&  init) noexcept
{
  m_color = init.m_color;

    for(int  i = 0;  i < m_number_of_rows;  ++i)
    {
      m_rows[i].reset(*this,(i < init.m_company_list.size())? init.m_company_list[i]:nullptr);
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
          m_total_hp += row.get_variable().get_hp();
        }
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
            if(!highest || (highest->get_ap() < row.get_ap()))
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
          row.add_ap(v);
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
render(gbstd::point  offset, const gbstd::canvas&  cv) const noexcept
{
    for(auto&  row: m_rows)
    {
      row.render(offset,cv);
    }
}





void
force::callbacks::
start_motion(gbstd::execution&  exec, ww::force*  force) noexcept
{
}


void
force::callbacks::
wait_for_motion(gbstd::execution&  exec, ww::force*  force) noexcept
{
}




}




