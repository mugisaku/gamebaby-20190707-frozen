#include"libgbstd/task.hpp"




namespace gbstd{




struct flags{
  static constexpr int        remove =  1;
  static constexpr int     skip_draw =  2;
  static constexpr int     skip_tick =  4;
  static constexpr int   blink_major =  8;
  static constexpr int   blink_minor = 16;
  static constexpr int         child = 32;
  static constexpr int         timer = 64;
};




struct
task
{
  std::string  m_name;

  void  (*m_collect_callback)(dummy*);
  void  (*m_draw_callback)(task_control,const canvas&,dummy&);
  void  (*m_tick_callback)(task_control,              dummy&);

  void*  m_data;

  status_value<int>  m_status;

  clock_watch  m_clock_watch;

  uint32_t  m_interval;
  uint32_t  m_next_time;

  uint32_t  m_timer;


  uint32_t  m_blink_show_value;
  uint32_t  m_blink_hide_value;
  uint32_t  m_blink_counter=0;

  task*  m_next;

  task() noexcept{}

};


task*
task_list::
m_dead_node_top=nullptr;


void
task_list::
unrefer(task*  ptr) noexcept
{
    if(ptr)
    {
        if(ptr->m_collect_callback)
        {
          ptr->m_collect_callback(static_cast<dummy*>(ptr->m_data));
        }


      ptr->m_next = m_dead_node_top      ;
                    m_dead_node_top = ptr;
    }
}




void
task_list::
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


task_control
task_list::
internal_push(void*  data, bool  is_child, std::string_view  name) noexcept
{
    if(!m_dead_node_top)
    {
      m_dead_node_top = new task;

      m_dead_node_top->m_next = nullptr;
    }


  auto  node = m_dead_node_top                          ;
               m_dead_node_top = m_dead_node_top->m_next;

  node->m_name     = name;
  node->m_data     = data;
  node->m_blink_show_value = 0;
  node->m_blink_hide_value = 0;
  node->m_blink_counter = 0;
  node->m_clock_watch = clock_master::get_zero_clock_watch();
  node->m_interval = 0;
  node->m_next_time = 0;
  node->m_collect_callback = nullptr;
  node->m_draw_callback = nullptr;
  node->m_tick_callback = nullptr;
  node->m_status.clear();

  node->m_next = m_top_pointer       ;
                 m_top_pointer = node;

  ++m_number_of_tasks;

    if(is_child)
    {
      node->m_status.set(flags::child);
    }


  return node;
}




void
task_list::
enforce_remove() noexcept
{
  auto  ptr = m_top_pointer;

  task*  last = nullptr;

    while(ptr)
    {
        if(ptr->m_status.test(flags::remove))
        {
          ptr = erase(ptr,last);
        }

      else
        {
          last = ptr              ;
                 ptr = ptr->m_next;
        }
    }
}


task*
task_list::
erase(task*  ptr, task*  prev_ptr) noexcept
{
  auto  next = ptr->m_next;

    if(prev_ptr)
    {
      prev_ptr->m_next = next;
    }


  unrefer(ptr);

    if(ptr == m_top_pointer)
    {
      m_top_pointer = next;
    }


  --m_number_of_tasks;

  return next;
}


void
task_list::
tick_object(task&  tsk) noexcept
{
  auto  now = tsk.m_clock_watch.get_time();

    while((now >= tsk.m_next_time) && tsk.m_tick_callback &&
          !tsk.m_status.test(flags::remove) &&
          !tsk.m_status.test(flags::skip_tick))
    {
      tsk.m_tick_callback(tsk,*static_cast<dummy*>(tsk.m_data));

        if(!tsk.m_interval)
        {
          break;
        }


      tsk.m_next_time += tsk.m_interval;

        if(tsk.m_timer >= tsk.m_interval)
        {
          tsk.m_timer -= tsk.m_interval;
        }
    }
}


void
task_list::
draw_object(const canvas&  cv, task&  tsk) noexcept
{
    if(!tsk.m_status.test(flags::skip_draw) && tsk.m_status.test(flags::blink_major))
    {
        if(tsk.m_blink_counter)
        {
          --tsk.m_blink_counter;
        }

      else
        {
          tsk.m_blink_counter = tsk.m_status.test(flags::blink_minor)? tsk.m_blink_show_value
                               :                                       tsk.m_blink_hide_value;

          tsk.m_status.reverse(flags::blink_minor);
        }
    }


    if(tsk.m_draw_callback && !tsk.m_status.test(flags::blink_minor))
    {
      tsk.m_draw_callback(tsk,cv,*reinterpret_cast<dummy*>(tsk.m_data));
    }
}


void
task_list::
process(const canvas*  cv, bool  tick) noexcept
{
  auto  ptr = m_top_pointer;

  task*  last = nullptr;

    while(ptr)
    {
        if(ptr->m_status.test(flags::remove))
        {
          ptr = erase(ptr,last);
        }

      else
        {
            if(ptr->m_status.test(flags::child))
            {
              static_cast<task_list*>(ptr->m_data)->process(ptr->m_status.test(flags::skip_draw)? nullptr:cv,
                                                            (tick && !ptr->m_status.test(flags::skip_tick)));
            }

          else
            {
                if(tick){tick_object(    *ptr);}
                if(  cv){draw_object(*cv,*ptr);}
            }


          last = ptr              ;
                 ptr = ptr->m_next;
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
task_list::
print_dead() noexcept
{
  printf("dead tick: %d, ",count(m_dead_node_top));
}


void
task_list::
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




task_control    task_control::set_remove_flag() noexcept{  m_pointer->m_status.set(  flags::remove);  return *this;}
task_control  task_control::unset_remove_flag() noexcept{  m_pointer->m_status.unset(flags::remove);  return *this;}

task_control    task_control::set_skip_draw_flag() noexcept{  m_pointer->m_status.set(  flags::skip_draw);  return *this;}
task_control  task_control::unset_skip_draw_flag() noexcept{  m_pointer->m_status.unset(flags::skip_draw);  return *this;}

task_control    task_control::set_skip_tick_flag() noexcept{  m_pointer->m_status.set(  flags::skip_tick);  return *this;}
task_control  task_control::unset_skip_tick_flag() noexcept{  m_pointer->m_status.unset(flags::skip_tick);  return *this;}

task_control  task_control::set_blink_flag() noexcept{  m_pointer->m_status.set(  flags::blink_major);  return *this;}

task_control
task_control::
unset_blink_flag() noexcept
{
  m_pointer->m_status.unset(flags::blink_major);
  m_pointer->m_status.unset(flags::blink_minor);

  return *this;
}


task_control
task_control::
set_blinking_rate(int  show, int  hide) noexcept
{
  m_pointer->m_blink_show_value = show;
  m_pointer->m_blink_hide_value = hide;

  return *this;
}


bool  task_control::test_remove_flag()    const noexcept{return m_pointer->m_status.test(flags::remove     );}
bool  task_control::test_skip_draw_flag() const noexcept{return m_pointer->m_status.test(flags::skip_draw  );}
bool  task_control::test_skip_tick_flag() const noexcept{return m_pointer->m_status.test(flags::skip_tick  );}
bool  task_control::test_blink_flag()     const noexcept{return m_pointer->m_status.test(flags::blink_major);}
bool  task_control::test_child_flag()     const noexcept{return m_pointer->m_status.test(flags::child      );}
bool  task_control::test_timer_flag()     const noexcept{return m_pointer->m_status.test(flags::timer      );}


const std::string&  task_control::get_name() const noexcept{return m_pointer->m_name;}
const void*  task_control::get_data() const noexcept{return m_pointer->m_data;}


uint32_t
task_control::
get_interval() const noexcept
{
  return m_pointer->m_interval;
}


task_control
task_control::
set_interval(uint32_t  t) noexcept
{
  m_pointer->m_interval = t;

  return *this;
}


bool
task_control::
test_timer() const noexcept
{
  return !m_pointer->m_timer;
}


task_control
task_control::
set_timer(uint32_t  t) noexcept
{
  m_pointer->m_timer = t;

  m_pointer->m_status.set(flags::timer);

  return *this;
}


task_control
task_control::
unset_timer() noexcept
{
  m_pointer->m_status.unset(flags::timer);

  return *this;
}


task_control
task_control::
set_collect(void  (*cb)(dummy*)) noexcept
{
  m_pointer->m_collect_callback = cb;

  return *this;
}


task_control
task_control::
set_draw(void  (*cb)(task_control,const canvas&,dummy&)) noexcept
{
  m_pointer->m_draw_callback = cb;

  return *this;
}


task_control
task_control::
set_tick(clock_watch  w, uint32_t  intval, void  (*cb)(task_control,dummy&)) noexcept
{
  m_pointer->m_clock_watch   =      w;
  m_pointer->m_interval      = intval;
  m_pointer->m_tick_callback =     cb;

  m_pointer->m_next_time = w.get_time()+intval;

  return *this;
}




}




