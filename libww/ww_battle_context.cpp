#include"libww/ww.hpp"




namespace ww{


void
battle_context::
reset(const force_initializer&  l,
      const force_initializer&  r) noexcept
{
   m_left_force.reset(side::left ,l);
  m_right_force.reset(side::right,r);

  m_base_process.assign("",80,gbstd::execution_frame("judge",judge,this));
}


void
battle_context::
pump_queue(std::list<row*>&  q) noexcept
{
    while(q.size())
    {
      auto  a = q.front();

        if(a->is_surviving())
        {
          m_actor_queue.emplace_back(a);
        }


      q.pop_front();
    }
}


row*
battle_context::
pump(std::list<row*>&  q) noexcept
{
    while(q.size())
    {
      auto  a = q.front();

      q.pop_front();

        if(a->is_surviving())
        {
          return (a);
        }
    }


  return nullptr;
}


void
battle_context::
pump_queue() noexcept
{
  auto&  l = m_left_force;
  auto&  r = m_right_force;

  l.increase_ap();
  r.increase_ap();

    while(l.m_actor_queue.size() && r.m_actor_queue.size())
    {
      auto  la = pump(l.m_actor_queue);
      auto  ra = pump(r.m_actor_queue);

        if(la && ra)
        {
            if(la->m_ap > ra->m_ap)
            {
              m_actor_queue.emplace_back(la);
              m_actor_queue.emplace_back(ra);
            }

          else
            {
              m_actor_queue.emplace_back(ra);
              m_actor_queue.emplace_back(la);
            }
        }

      else
        if(la)
        {
          m_actor_queue.emplace_back(la);
        }

      else
        if(ra)
        {
          m_actor_queue.emplace_back(ra);
        }
    }


  pump_queue(l.m_actor_queue);
  pump_queue(r.m_actor_queue);
}


void
battle_context::
render(const gbstd::canvas&  cv) const noexcept
{
   m_left_force.render({cv,                             0,0,g_frame_w*3,g_frame_h*8});
  m_right_force.render({cv,cv.get_width()-1-(g_frame_w*3),0,g_frame_w*3,g_frame_h*8});
}




}




