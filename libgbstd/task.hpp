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




class draw_task;
class tick_task;


class
draw_task_control
{
  draw_task*  m_pointer=nullptr;

  weak_reference_counter  m_counter;

public:
  draw_task_control() noexcept{}
  draw_task_control(draw_task&  tsk) noexcept;
  draw_task_control(const draw_task_control&   rhs) noexcept{assign(rhs);}
  draw_task_control(      draw_task_control&&  rhs) noexcept{assign(std::move(rhs));}

  draw_task_control&  operator=(const draw_task_control&   rhs) noexcept{return assign(rhs);}
  draw_task_control&  operator=(      draw_task_control&&  rhs) noexcept{return assign(std::move(rhs));}

  operator bool() const noexcept;

  draw_task_control&  assign(const draw_task_control&   rhs) noexcept;
  draw_task_control&  assign(      draw_task_control&&  rhs) noexcept;

  const std::string&  get_name() const noexcept;

  draw_task_control&    set_remove_flag() noexcept;
  draw_task_control&  unset_remove_flag() noexcept;

  draw_task_control&    set_skip_flag() noexcept;
  draw_task_control&  unset_skip_flag() noexcept;

  draw_task_control&    set_blink_flag() noexcept;
  draw_task_control&  unset_blink_flag() noexcept;

  draw_task_control&  set_blinking_rate(int  show, int  hide) noexcept;

  bool  test_remove_flag() const noexcept;
  bool  test_skip_flag()   const noexcept;
  bool  test_blink_flag()   const noexcept;

};


class
draw_task_list
{
  static draw_task*  m_dead_node_top;

  static void  unrefer(draw_task*  ptr) noexcept;

  static void  process_child(const canvas&  cv, draw_task_list&  ls) noexcept;


  draw_task*  m_top_pointer=nullptr;

  int  m_number_of_tasks=0;

  draw_task_control  m_control;

public:
  draw_task_list() noexcept{}
 ~draw_task_list(){clear();}

  void  clear() noexcept;

  int  get_number_of_tasks() const noexcept{return m_number_of_tasks;}

  draw_task_control&  get_control() noexcept{return m_control;}

  using callback = void  (*)(const canvas&,dummy&);

  draw_task_control  push(callback  cb, void*  data, std::string_view  name="") noexcept;
  draw_task_control  push(callback  cb, std::string_view  name="") noexcept{return push(cb,nullptr,name);}

  template<typename  T>
  draw_task_control  push(void  (*cb)(const canvas&,T&), T&  data, std::string_view  name="") noexcept
  {
    return push(reinterpret_cast<callback>(cb),reinterpret_cast<void*>(&data),name);
  }

  template<typename  T>
  draw_task_control  push(T&  data, std::string_view  name="") noexcept
  {
    return push(reinterpret_cast<callback>(T::draw),reinterpret_cast<void*>(&data),name);
  }


  draw_task_control  push(draw_task_list&  ls, std::string_view  name="") noexcept;

  void  remove() noexcept;

  void  process(const canvas&  cv) noexcept;

  static void  print_dead() noexcept;
  static void  clear_dead() noexcept;

};




class
tick_task_control
{
  tick_task*              m_pointer=nullptr;
  weak_reference_counter  m_counter;

public:
  tick_task_control() noexcept{}
  tick_task_control(tick_task&  tsk) noexcept;
  tick_task_control(const tick_task_control&   rhs) noexcept{assign(rhs);}
  tick_task_control(      tick_task_control&&  rhs) noexcept{assign(std::move(rhs));}

  tick_task_control&  operator=(const tick_task_control&   rhs) noexcept{return assign(rhs);}
  tick_task_control&  operator=(      tick_task_control&&  rhs) noexcept{return assign(std::move(rhs));}

  operator bool() const noexcept;

  tick_task_control&  assign(const tick_task_control&   rhs) noexcept;
  tick_task_control&  assign(      tick_task_control&&  rhs) noexcept;

  tick_task_control&    set_remove_flag() noexcept;
  tick_task_control&  unset_remove_flag() noexcept;

  tick_task_control&    set_skip_flag() noexcept;
  tick_task_control&  unset_skip_flag() noexcept;

  bool  test_remove_flag() const noexcept;
  bool  test_skip_flag()   const noexcept;

  const std::string&  get_name() const noexcept;

};


class
tick_task_list
{
  static tick_task*  m_dead_node_top;

  tick_task*  m_top_pointer=nullptr;

  int  m_number_of_tasks=0;

  tick_task_control  m_control;

  static void  unrefer(tick_task*  ptr) noexcept;

  static void  process_child(tick_task_list&  ls) noexcept;

public:
  tick_task_list() noexcept{}
 ~tick_task_list(){clear();}

  void  clear() noexcept;

  int  get_number_of_tasks() const noexcept{return m_number_of_tasks;}

  tick_task_control&  get_control() noexcept{return m_control;}

  using callback = void  (*)(dummy&);

  tick_task_control  push(clock_watch  w, uint32_t  intval, callback  cb, void*  data, std::string_view  name="") noexcept;
  tick_task_control  push(clock_watch  w, uint32_t  intval, callback  cb, std::string_view  name="") noexcept
  {return push(w,intval,cb,nullptr,name);}

  template<typename  T>
  tick_task_control  push(clock_watch  w, uint32_t  intval, void  (*cb)(T&), T&  data, std::string_view  name="") noexcept
  {
    return push(w,intval,reinterpret_cast<callback>(cb),reinterpret_cast<void*>(&data),name);
  }

  template<typename  T>
  tick_task_control  push(clock_watch  w, uint32_t  intval, T&  data, std::string_view  name="") noexcept
  {
    return push(w,intval,reinterpret_cast<callback>(T::tick),reinterpret_cast<void*>(&data),name);
  }

  tick_task_control  push(clock_watch  w, uint32_t  intval, tick_task_list&  ls, std::string_view  name="") noexcept;

  void  remove() noexcept;

  void  process() noexcept;

  static void  print_dead() noexcept;
  static void  clear_dead() noexcept;

};




}




#endif




