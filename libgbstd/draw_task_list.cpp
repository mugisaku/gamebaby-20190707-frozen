#include"libgbstd/task.hpp"




namespace gbstd{




struct flags{
  static constexpr int  remove = 1;
  static constexpr int    skip = 2;
};


using observer = draw_task_list::observer;


struct
draw_task_list::
node
{
  draw_task_entry  m_entry;

  status_value<int>  m_status;

  uint32_t  m_skip_count;

  reference_counter*  m_counter;

  node*  m_next;

};


struct
draw_task_list::
reference_counter
{
  int  m_value;
  int  m_weak_value;

  reference_counter*  m_next;

};


draw_task_list::node*
draw_task_list::
m_dead_node_top=nullptr;


draw_task_list::reference_counter*
draw_task_list::
m_dead_counter_top=nullptr;


void
draw_task_list::
unrefer(reference_counter*  ptr) noexcept
{
  auto&  n = ptr->m_weak_value;

    if(!--n)
    {
      ptr->m_next = m_dead_counter_top      ;
                    m_dead_counter_top = ptr;
    }
}


draw_task_list::
~draw_task_list()
{
    for(auto  ptr: m_container)
    {
      ptr->m_counter->m_value = 0;

      unrefer(ptr->m_counter);

      ptr->m_next = m_dead_node_top      ;
                    m_dead_node_top = ptr;
    }
}




draw_task_list::observer
draw_task_list::
push(draw_task_entry  ent) noexcept
{
    if(!m_dead_node_top)
    {
      m_dead_node_top = new node;

      m_dead_node_top->m_next = nullptr;
    }


    if(!m_dead_counter_top)
    {
      m_dead_counter_top = new reference_counter;

      m_dead_counter_top->m_next = nullptr;
    }


  auto  node = m_dead_node_top                          ;
               m_dead_node_top = m_dead_node_top->m_next;

  auto  counter = m_dead_counter_top                             ;
                  m_dead_counter_top = m_dead_counter_top->m_next;

  counter->m_value      = 1;
  counter->m_weak_value = 1;

  node->m_entry   = ent;
  node->m_skip_count = 0;
  node->m_status.clear();
  node->m_counter = counter;

  return observer(*node);
}


void
draw_task_list::
process(const canvas&  cv) noexcept
{
  auto  it = m_container.begin();

    while(it != m_container.end())
    {
      auto  nd = *it;

        if(nd->m_status.test(flags::remove))
        {
          nd->m_next = m_dead_node_top     ;
                       m_dead_node_top = nd;

          it = m_container.erase(it);
        }

      else
        if(nd->m_status.test(flags::skip))
        {
          ++nd->m_skip_count;

          nd->m_entry(cv);

          ++it;
        }

      else
        {
          ++it;
        }
    }
}




observer::
observer(node&  nd) noexcept:
m_node(&nd),
m_counter(nd.m_counter)
{
  ++m_counter->m_weak_value;
}


observer::
~observer()
{
  unrefer(m_counter);
}




observer::
operator bool() const noexcept
{
  return m_counter->m_value;
}


observer&    observer::set_remove_flag() noexcept{  m_node->m_status.set(  flags::remove);  return *this;}
observer&  observer::unset_remove_flag() noexcept{  m_node->m_status.unset(flags::remove);  return *this;}

observer&    observer::set_skip_flag() noexcept{  m_node->m_status.set(  flags::skip);  return *this;}
observer&  observer::unset_skip_flag() noexcept{  m_node->m_status.unset(flags::skip);  return *this;}

bool  observer::test_remove_flag() const noexcept{return m_node->m_status.test(flags::remove);}
bool  observer::test_skip_flag()   const noexcept{return m_node->m_status.test(flags::skip  );}




}




