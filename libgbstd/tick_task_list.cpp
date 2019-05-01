#include"libgbstd/task.hpp"




namespace gbstd{




struct flags{
  static constexpr int  remove = 1;
  static constexpr int    skip = 2;
};




struct
tick_task
{
  std::string  m_name;

  tick_task_list::callback  m_callback;

  void*  m_data;

  status_value<int>  m_status;

  clock_watch  m_clock_watch;

  uint32_t  m_interval;
  uint32_t  m_next_time;

  uint32_t  m_skip_count;

  weak_reference_counter  m_counter;

  tick_task*  m_next;

  tick_task() noexcept{}

};


tick_task*
tick_task_list::
m_dead_node_top=nullptr;


void
tick_task_list::
unrefer(tick_task*  ptr) noexcept
{
    if(ptr)
    {
      ptr->m_counter.set_value(0).reset();

      ptr->m_next = m_dead_node_top      ;
                    m_dead_node_top = ptr;
    }
}




void
tick_task_list::
clear() noexcept
{
  auto  ptr = m_top_pointer          ;
              m_top_pointer = nullptr;

    while(ptr)
    {
      auto  next = ptr->m_next;

      unrefer(ptr)      ;
              ptr = next;

      --m_number_of_tasks;
    }
}


tick_task_control
tick_task_list::
push(clock_watch  w, uint32_t  intval, callback  cb, void*  data, std::string_view  name) noexcept
{
    if(!m_dead_node_top)
    {
      m_dead_node_top = new tick_task;

      m_dead_node_top->m_next = nullptr;
    }


  auto  node = m_dead_node_top                          ;
               m_dead_node_top = m_dead_node_top->m_next;

  node->m_name     = name;
  node->m_callback = cb;
  node->m_data     = data;
  node->m_skip_count = 0;
  node->m_next_time = w.get_time();
  node->m_clock_watch = std::move(w);
  node->m_interval = intval;
  node->m_status.clear();
  node->m_counter = weak_reference_counter();
  node->m_counter.set_value(1);

  node->m_next = m_top_pointer       ;
                 m_top_pointer = node;

  ++m_number_of_tasks;

  return tick_task_control(*node);
}




void
tick_task_list::
process_child(tick_task_list&  ls) noexcept
{
  ls.process();
}


tick_task_control
tick_task_list::
push(clock_watch  w, uint32_t  intval, tick_task_list&  ls, std::string_view  name) noexcept
{
  m_control = push(w,intval,process_child,ls,name);

  return m_control;
}


void
tick_task_list::
remove() noexcept
{
  auto  ptr = m_top_pointer;

  tick_task*  last = nullptr;

    while(ptr)
    {
      auto  next = ptr->m_next;

        if(ptr->m_status.test(flags::remove))
        {
            if(last)
            {
              last->m_next = next;
            }


          unrefer(ptr);

            if(ptr == m_top_pointer)
            {
              m_top_pointer = next;
            }


          --m_number_of_tasks;
        }


      last = ptr       ;
             ptr = next;
    }
}


void
tick_task_list::
process() noexcept
{
  auto  ptr = m_top_pointer;

  tick_task*  last = nullptr;

    while(ptr)
    {
      auto  next = ptr->m_next;

        if(ptr->m_status.test(flags::remove))
        {
            if(last)
            {
              last->m_next = next;
            }


          unrefer(ptr);

            if(ptr == m_top_pointer)
            {
              m_top_pointer = next;
            }


          --m_number_of_tasks;
        }

      else
        if(ptr->m_status.test(flags::skip))
        {
          ++ptr->m_skip_count;
        }

      else
        {
          auto  now = ptr->m_clock_watch.get_time();

            while(now >= ptr->m_next_time)
            {
              ptr->m_callback(*static_cast<dummy*>(ptr->m_data));

                if(!ptr->m_interval)
                {
                  break;
                }


              ptr->m_next_time += ptr->m_interval;
            }
        }


      last = ptr       ;
             ptr = next;
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
tick_task_list::
print_dead() noexcept
{
  printf("dead tick: %d, ",count(m_dead_node_top));
}


void
tick_task_list::
clear_dead() noexcept
{
  auto  ptr = m_dead_node_top          ;
              m_dead_node_top = nullptr;

    while(ptr)
    {
      auto  next = ptr->m_next;

      delete ptr       ;
             ptr = next;
    }
}




tick_task_control::
tick_task_control(tick_task&  tsk) noexcept:
m_pointer(&tsk),
m_counter(tsk.m_counter)
{
}




tick_task_control::
operator bool() const noexcept
{
  return m_pointer && m_counter;
}


tick_task_control&
tick_task_control::
assign(const tick_task_control&   rhs) noexcept
{
    if(this != &rhs)
    {
      m_pointer = rhs.m_pointer;
      m_counter = rhs.m_counter;
    }


  return *this;
}


tick_task_control&
tick_task_control::
assign(tick_task_control&&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_pointer = rhs.m_pointer          ;
                  rhs.m_pointer = nullptr;

      m_counter = std::move(rhs.m_counter);
    }


  return *this;
}


tick_task_control&    tick_task_control::set_remove_flag() noexcept{  m_pointer->m_status.set(  flags::remove);  return *this;}
tick_task_control&  tick_task_control::unset_remove_flag() noexcept{  m_pointer->m_status.unset(flags::remove);  return *this;}

tick_task_control&    tick_task_control::set_skip_flag() noexcept{  m_pointer->m_status.set(  flags::skip);  return *this;}
tick_task_control&  tick_task_control::unset_skip_flag() noexcept{  m_pointer->m_status.unset(flags::skip);  return *this;}


bool  tick_task_control::test_remove_flag() const noexcept{return m_pointer->m_status.test(flags::remove     );}
bool  tick_task_control::test_skip_flag()   const noexcept{return m_pointer->m_status.test(flags::skip       );}




}




