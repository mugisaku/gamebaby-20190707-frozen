#ifndef task_hpp_was_included
#define task_hpp_was_included


#include"libgbstd/utility.hpp"
#include"libgbstd/image.hpp"
#include"libgbstd/clock.hpp"
#include<list>
#include<string_view>




namespace gbstd{




class
task
{
  std::string  m_name;

  void  (*m_draw_callback)(const canvas&,dummy&);
  void  (*m_tick_callback)(              dummy&);

  void*  m_data;

  status_value<int>  m_status;

  struct flags{
    static constexpr int          live =  1;
    static constexpr int         sleep =  2;
    static constexpr int         alarm =  4;
    static constexpr int          show =  8;
    static constexpr int        freeze = 16;
    static constexpr int         blink = 32;
    static constexpr int     blink_bit = 64;
  };


  clock_watch  m_clock_watch;

  uint32_t  m_interval;
  uint32_t  m_next_time;

  uint32_t  m_blink_show_value;
  uint32_t  m_blink_hide_value;
  uint32_t  m_blink_counter=0;

public:
  task() noexcept{}
  task(std::string_view  name, void*  ptr) noexcept: m_name(name), m_data(ptr){}

  const std::string&  get_name() const noexcept{return m_name;}

  const void*  get_data() const noexcept{return m_data;}

  task&  live() noexcept{  m_status.set(  flags::live);  return *this;}
  task&   die() noexcept{  m_status.unset(flags::live);  return *this;}

  task&  sleep(uint32_t  t) noexcept;

  task&  sleep() noexcept{  m_status.set(  flags::sleep);  return *this;}
  task&  getup() noexcept{  m_status.unset(flags::sleep);  return *this;}

  task&  show() noexcept{  m_status.set(  flags::show);  return *this;}
  task&  hide() noexcept{  m_status.unset(flags::show);  return *this;}

  task&    freeze() noexcept{  m_status.set(  flags::freeze);  return *this;}
  task&  unfreeze() noexcept{  m_status.unset(flags::freeze);  return *this;}

  task&    blink() noexcept{  m_status.set(  flags::blink);  return *this;}
  task&  unblink() noexcept{  m_status.unset(flags::blink);  return *this;}

  task&  set_blinking_rate(int  show, int  hide) noexcept;

  bool  is_living()   const noexcept{return m_status.test(flags::live);}
  bool  is_sleeping() const noexcept{return m_status.test(flags::sleep);}
  bool  is_blinking() const noexcept{return m_status.test(flags::blink);}
  bool  is_freezing() const noexcept{return m_status.test(flags::freeze);}
  bool  is_showing()  const noexcept{return m_status.test(flags::show);}

  uint32_t  get_interval(           ) const noexcept{return m_interval                   ;}
  task&     set_interval(uint32_t  t)       noexcept{       m_interval = t;  return *this;}

  task&  set_clock_watch(clock_watch  w) noexcept{  m_clock_watch = w;  return *this;}

  task&  set_data(void*  ptr) noexcept{  m_data = ptr;  return *this;}


  task&  set_draw(void  (*cb)(const canvas&,dummy&)) noexcept{  m_draw_callback = cb;  return *this;}

  template<typename  T>
  task&  set_draw(void  (*cb)(const canvas&,T&)) noexcept
  {
    return set_draw(reinterpret_cast<void(*)(const canvas&,dummy&)>(cb));
  }

  template<typename  T>
  task&  set_draw() noexcept
  {
    return set_draw(T::draw);
  }

  task&  set_tick(void  (*cb)(dummy&)) noexcept{  m_tick_callback = cb;  return *this;}

  template<typename  T>
  task&  set_tick(void  (*cb)(T&)) noexcept
  {
    return set_tick(reinterpret_cast<void(*)(dummy&)>(cb));
  }

  template<typename  T>
  task&  set_tick() noexcept
  {
    return set_tick(T::tick);
  }

  task&  operator()(                 ) noexcept;
  task&  operator()(const canvas&  cv) noexcept;

};


class
task_list
{
  std::list<task*>  m_list;

public:
  task_list() noexcept{}
 ~task_list(){clear();}

  void  clear() noexcept{m_list.clear();}

  task_list&  add(task&  tsk) noexcept;

  task_list&  remove_by_name(std::string_view  name) noexcept;

  void  enforce_remove() noexcept;

  void  process(const canvas*  cv) noexcept;

  static void  print_dead() noexcept;
  static void  clear_dead() noexcept;

};




}




#endif




