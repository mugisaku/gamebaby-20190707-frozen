#ifndef task_hpp_was_included
#define task_hpp_was_included


#include"libgbstd/utility.hpp"
#include"libgbstd/weak_reference_counter.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/image.hpp"
#include"libgbstd/clock.hpp"
#include<list>
#include<string_view>




namespace gbstd{




class task;


class
task_control
{
  task*  m_pointer=nullptr;

public:
  task_control(task*  ptr=nullptr) noexcept: m_pointer(ptr){}
  task_control(task&  tsk) noexcept: m_pointer(&tsk){}

  operator bool() const noexcept{return m_pointer;}

  const std::string&  get_name() const noexcept;

  const void*  get_data() const noexcept;

  task_control    set_remove_flag() noexcept;
  task_control  unset_remove_flag() noexcept;

  task_control    set_skip_draw_flag() noexcept;
  task_control  unset_skip_draw_flag() noexcept;

  task_control    set_skip_tick_flag() noexcept;
  task_control  unset_skip_tick_flag() noexcept;

  task_control    set_blink_flag() noexcept;
  task_control  unset_blink_flag() noexcept;

  task_control  set_blinking_rate(int  show, int  hide) noexcept;

  bool  test_remove_flag() const noexcept;
  bool  test_skip_draw_flag()   const noexcept;
  bool  test_skip_tick_flag()   const noexcept;
  bool  test_blink_flag()   const noexcept;
  bool  test_child_flag()   const noexcept;
  bool  test_timer_flag()   const noexcept;

  uint32_t      get_interval(           ) const noexcept;
  task_control  set_interval(uint32_t  t)       noexcept;

  bool            test_timer(           ) const noexcept;
  task_control     set_timer(uint32_t  t)       noexcept;
  task_control   unset_timer(           )       noexcept;

  task_control  set_collect(void  (*cb)(dummy*)) noexcept;

  template<typename  T>
  task_control  set_collect(void  (*cb)(T*)) noexcept
  {
    return set_collect(reinterpret_cast<void(*)(dummy*)>(cb));
  }

  template<typename  T>
  task_control  set_collect() noexcept
  {
    return set_collect(T::collect);
  }

  task_control  set_draw(void  (*cb)(task_control,const canvas&,dummy&)) noexcept;

  template<typename  T>
  task_control  set_draw(void  (*cb)(task_control,const canvas&,T&)) noexcept
  {
    return set_draw(reinterpret_cast<void(*)(task_control,const canvas&,dummy&)>(cb));
  }

  template<typename  T>
  task_control  set_draw() noexcept
  {
    return set_draw(T::draw);
  }

  task_control  set_tick(clock_watch  w, uint32_t  intval, void  (*cb)(task_control,dummy&)) noexcept;

  template<typename  T>
  task_control  set_tick(clock_watch  w, uint32_t  intval, void  (*cb)(task_control,T&)) noexcept
  {
    return set_tick(w,intval,reinterpret_cast<void(*)(task_control,dummy&)>(cb));
  }

  template<typename  T>
  task_control  set_tick(clock_watch  w, uint32_t  intval) noexcept
  {
    return set_tick(w,intval,T::tick);
  }

};


class
task_list
{
  static task*  m_dead_node_top;

  task*  m_top_pointer=nullptr;

  int  m_number_of_tasks=0;

  static void  unrefer(task*  ptr) noexcept;

  task*  erase(task*  ptr, task*  prev_ptr) noexcept;

  static void  tick_object(                   task&  tsk) noexcept;
  static void  draw_object(const canvas&  cv, task&  tsk) noexcept;

  task_control  internal_push(void*  data, bool  is_child, std::string_view  name) noexcept;

public:
  task_list() noexcept{}
 ~task_list(){clear();}

  void  clear() noexcept;

  int  get_number_of_tasks() const noexcept{return m_number_of_tasks;}

  task_control  push(task_list&  ls, std::string_view  name="") noexcept{return internal_push( &ls, true,name);}
  task_control  push(                std::string_view  name="") noexcept{return internal_push(nullptr,false,name);}

  template<typename  T>
  task_control  push(T&  data, std::string_view  name="") noexcept{return internal_push(&data,false,name);}

  void  enforce_remove() noexcept;

  void  process(const canvas&  cv, bool  tick=true, bool  draw=true) noexcept;

  static void  print_dead() noexcept;
  static void  clear_dead() noexcept;

};




}




#endif




