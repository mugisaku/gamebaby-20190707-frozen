#include"libgbstd/task.hpp"




namespace gbstd{




struct flags{
  static constexpr int  remove = 1;
  static constexpr int    skip = 2;
  static constexpr int   blink_major = 4;
  static constexpr int   blink_minor = 8;
};


struct
draw_task
{
  std::string  m_name;

  draw_task_list::callback  m_callback;

  void*  m_data;

  status_value<int>  m_status;

  uint32_t  m_skip_count;

  uint32_t  m_blink_show_value;
  uint32_t  m_blink_hide_value;
  uint32_t  m_blink_counter=0;

  weak_reference_counter  m_counter;

  draw_task*  m_next;

  draw_task() noexcept{}

};


draw_task*
draw_task_list::
m_dead_node_top=nullptr;


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
unrefer(draw_task*  ptr) noexcept
{
    if(ptr)
    {
      ptr->m_counter.set_value(0).reset();

      ptr->m_next = m_dead_node_top      ;
                    m_dead_node_top = ptr;
    }
}




void
draw_task_list::
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


draw_task_control
draw_task_list::
push(callback  cb, void*  data, std::string_view  name) noexcept
{
    if(!m_dead_node_top)
    {
      m_dead_node_top = new draw_task;

      m_dead_node_top->m_next = nullptr;
    }


  auto  node = m_dead_node_top                          ;
               m_dead_node_top = m_dead_node_top->m_next;

  node->m_name     = name;
  node->m_callback =   cb;
  node->m_data     = data;

  node->m_skip_count = 0;
  node->m_blink_show_value = 0;
  node->m_blink_hide_value = 0;
  node->m_blink_counter = 0;
  node->m_status.clear();
  node->m_counter = weak_reference_counter();
  node->m_counter.set_value(1);

  node->m_next = m_top_pointer       ;
                 m_top_pointer = node;

  ++m_number_of_tasks;

  return draw_task_control(*node);
}




void
draw_task_list::
process_child(const canvas&  cv, draw_task_list&  ls) noexcept
{
  ls.process(cv);
}


draw_task_control
draw_task_list::
push(draw_task_list&  ls, std::string_view  name) noexcept
{
  m_control = push(process_child,ls,name);

  return m_control;
}


void
draw_task_list::
remove() noexcept
{
  auto  ptr = m_top_pointer;

  draw_task*  last = nullptr;

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
draw_task_list::
process(const canvas&  cv) noexcept
{
  auto  ptr = m_top_pointer;

  draw_task*  last = nullptr;

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
            if(ptr->m_status.test(flags::blink_major))
            {
                if(ptr->m_blink_counter)
                {
                  --ptr->m_blink_counter;
                }

              else
                {
                  ptr->m_blink_counter = ptr->m_status.test(flags::blink_minor)? ptr->m_blink_show_value
                                        :                                        ptr->m_blink_hide_value;

                  ptr->m_status.reverse(flags::blink_minor);
                }
            }


            if(!ptr->m_status.test(flags::blink_minor))
            {
              ptr->m_callback(cv,*reinterpret_cast<dummy*>(ptr->m_data));
            }
        }


      last = ptr       ;
             ptr = next;
    }
}


void
draw_task_list::
print_dead() noexcept
{
  printf("dead draw %d, ",count(m_dead_node_top));
}


void
draw_task_list::
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




draw_task_control::
draw_task_control(draw_task&  tsk) noexcept:
m_pointer(&tsk),
m_counter(tsk.m_counter)
{
}




draw_task_control::
operator bool() const noexcept
{
  return m_pointer && m_counter;
}


draw_task_control&
draw_task_control::
assign(const draw_task_control&   rhs) noexcept
{
    if(this != &rhs)
    {
      m_pointer = rhs.m_pointer;
      m_counter = rhs.m_counter;
    }


  return *this;
}


draw_task_control&
draw_task_control::
assign(draw_task_control&&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_pointer = rhs.m_pointer          ;
                  rhs.m_pointer = nullptr;

      m_counter = std::move(rhs.m_counter);
    }


  return *this;
}


const std::string&  draw_task_control::get_name() const noexcept{return m_pointer->m_name;}


draw_task_control&    draw_task_control::set_remove_flag() noexcept{  m_pointer->m_status.set(  flags::remove);  return *this;}
draw_task_control&  draw_task_control::unset_remove_flag() noexcept{  m_pointer->m_status.unset(flags::remove);  return *this;}

draw_task_control&    draw_task_control::set_skip_flag() noexcept{  m_pointer->m_status.set(  flags::skip);  return *this;}
draw_task_control&  draw_task_control::unset_skip_flag() noexcept{  m_pointer->m_status.unset(flags::skip);  return *this;}

draw_task_control&  draw_task_control::set_blink_flag() noexcept{  m_pointer->m_status.set(  flags::blink_major);  return *this;}

draw_task_control&
draw_task_control::
unset_blink_flag() noexcept
{
  m_pointer->m_status.unset(flags::blink_major);
  m_pointer->m_status.unset(flags::blink_minor);

  return *this;
}


draw_task_control&
draw_task_control::
set_blinking_rate(int  show, int  hide) noexcept
{
  m_pointer->m_blink_show_value = show;
  m_pointer->m_blink_hide_value = hide;

  return *this;
}


bool  draw_task_control::test_remove_flag() const noexcept{return m_pointer->m_status.test(flags::remove     );}
bool  draw_task_control::test_skip_flag()   const noexcept{return m_pointer->m_status.test(flags::skip       );}
bool  draw_task_control::test_blink_flag()  const noexcept{return m_pointer->m_status.test(flags::blink_major);}




}




