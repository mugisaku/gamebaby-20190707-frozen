#include"libgbstd/menu.hpp"




namespace gbstd{
namespace menus{




stack&
stack::
ready(clock_watch  w, uint32_t  intval, gbstd::task_list&  ls) noexcept
{
    if(m_container.empty())
    {
      m_control = ls.push(*this);

      m_control.set_draw<stack>().set_tick<stack>(w,intval);
    }


  return *this;
}



stack&
stack::
internal_open(int  opening_value, view&  v, void*  data, callback<dummy>  cb) noexcept
{
    if(!v.test_busy_flag())
    {
      v.set_busy_flag();

      m_container.emplace_back();

      auto&  e = m_container.back();

      e.m_opening_value = opening_value;
      e.m_view          = &v;
      e.m_data          = data;
      e.m_callback      = cb;
    }


  return *this;
}


stack&
stack::
close_top(int  closing_value) noexcept
{
    if(m_container.size())
    {
      result  res(m_container.back().m_opening_value,closing_value);

      m_container.back().m_view->unset_busy_flag();

      m_container.pop_back();

        if(m_container.size())
        {
          auto&  e = m_container.back();

          e.m_callback(*this,&res,*e.m_view,*static_cast<dummy*>(e.m_data));
        }
    }

  else
    if(m_control)
    {
      m_control.set_remove_flag().clear();
    }


  return *this;
}


void
stack::
draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, stack&  stk) noexcept
{
    for(auto&  e: stk.m_container)
    {
      e.m_view->draw(cv);
    }
}


void
stack::
tick(gbstd::task_control  ctrl, stack&  stk) noexcept
{
    if(stk.m_container.size())
    {
      auto&  e = stk.m_container.back();

      e.m_callback(stk,nullptr,*e.m_view,*static_cast<dummy*>(e.m_data));

        if(stk.m_container.empty())
        {
          stk.m_control.set_remove_flag().clear();
        }
    }
}




}}




