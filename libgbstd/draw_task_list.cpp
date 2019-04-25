#include"libgbstd/task.hpp"




namespace gbstd{




struct flags{
  static constexpr int  remove = 1;
  static constexpr int    skip = 2;
  static constexpr int   blink_major = 4;
  static constexpr int   blink_minor = 8;
};


using control = draw_task_list::control;


struct
draw_task_list::
node
{
  draw_task_entry  m_entry;

  status_value<int>  m_status;

  uint32_t  m_skip_count;

  uint32_t  m_blink_show_value;
  uint32_t  m_blink_hide_value;
  uint32_t  m_blink_counter=0;

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
unrefer(node*  ptr) noexcept
{
    if(ptr)
    {
      ptr->m_counter->m_value = 0;

      unrefer(ptr->m_counter);

      ptr->m_next = m_dead_node_top      ;
                    m_dead_node_top = ptr;
    }
}


void
draw_task_list::
unrefer(reference_counter*  ptr) noexcept
{
    if(ptr)
    {
      auto&  n = ptr->m_weak_value;

        if(!--n)
        {
          ptr->m_next = m_dead_counter_top      ;
                        m_dead_counter_top = ptr;
        }
    }
}




void
draw_task_list::
clear() noexcept
{
    for(auto  ptr: m_container)
    {
      unrefer(ptr);
    }
}


draw_task_list::control
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
  node->m_blink_show_value = 0;
  node->m_blink_hide_value = 0;
  node->m_blink_counter = 0;
  node->m_status.clear();
  node->m_counter = counter;

  m_container.emplace_back(node);

  return control(*node);
}


void
draw_task_list::
remove() noexcept
{
  auto  it = m_container.begin();

    while(it != m_container.end())
    {
      auto  nd = *it;

        if(nd->m_status.test(flags::remove))
        {
          unrefer(nd);

          it = m_container.erase(it);
        }

      else
        {
          ++it;
        }
    }
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
          unrefer(nd);

          it = m_container.erase(it);
        }

      else
        if(nd->m_status.test(flags::skip))
        {
          ++nd->m_skip_count;

          ++it;
        }

      else
        {
            if(nd->m_status.test(flags::blink_major))
            {
                if(nd->m_blink_counter)
                {
                  --nd->m_blink_counter;
                }

              else
                {
                  nd->m_blink_counter = nd->m_status.test(flags::blink_minor)? nd->m_blink_show_value
                                       :                                       nd->m_blink_hide_value;

                  nd->m_status.reverse(flags::blink_minor);
                }
            }


            if(!nd->m_status.test(flags::blink_minor))
            {
              nd->m_entry(cv);
            }


          ++it;
        }
    }
}


template<typename  T>
int
count(T*  ptr) noexcept
{
  int  n = 0;

    while(ptr)
    {
      ++n;

      ptr = ptr->m_next;
    }


  return n;
}


void
draw_task_list::
print() const noexcept
{
  printf("dead top %d, dead counter %d\n",
    count(m_dead_node_top),
    count(m_dead_counter_top));

}




control::
control(node&  nd) noexcept:
m_node(&nd),
m_counter(nd.m_counter)
{
  ++m_counter->m_weak_value;
}


control::
~control()
{
  unrefer(m_counter);
}




control::
operator bool() const noexcept
{
  return m_node && m_counter->m_value;
}


control&
control::
assign(const control&   rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer(m_counter);

      m_node    = rhs.m_node;
      m_counter = rhs.m_counter;

      ++m_counter->m_weak_value;
    }


  return *this;
}


control&
control::
assign(control&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer(m_counter);

      m_node = rhs.m_node          ;
               rhs.m_node = nullptr;

      m_counter = rhs.m_counter          ;
                  rhs.m_counter = nullptr;
    }


  return *this;
}


control&    control::set_remove_flag() noexcept{  m_node->m_status.set(  flags::remove);  return *this;}
control&  control::unset_remove_flag() noexcept{  m_node->m_status.unset(flags::remove);  return *this;}

control&    control::set_skip_flag() noexcept{  m_node->m_status.set(  flags::skip);  return *this;}
control&  control::unset_skip_flag() noexcept{  m_node->m_status.unset(flags::skip);  return *this;}

control&  control::set_blink_flag() noexcept{  m_node->m_status.set(  flags::blink_major);  return *this;}

control&
control::
unset_blink_flag() noexcept
{
  m_node->m_status.unset(flags::blink_major);
  m_node->m_status.unset(flags::blink_minor);

  return *this;
}


control&
control::
set_blinking_rate(int  show, int  hide) noexcept
{
  m_node->m_blink_show_value = show;
  m_node->m_blink_hide_value = hide;

  return *this;
}


bool  control::test_remove_flag() const noexcept{return m_node->m_status.test(flags::remove     );}
bool  control::test_skip_flag()   const noexcept{return m_node->m_status.test(flags::skip       );}
bool  control::test_blink_flag()  const noexcept{return m_node->m_status.test(flags::blink_major);}




}




