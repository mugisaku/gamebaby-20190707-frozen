#ifndef gbstd_process_HPP
#define gbstd_process_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include"libgbstd/image.hpp"
#include<utility>
#include<string_view>
#include<list>
#include<string>
#include<vector>
#include<cstdint>


namespace gbstd{




class
clock
{
  std::string  m_name;

  uint32_t  m_time=0;
  uint32_t  m_fraction=0;

  int  m_permil=1000;

  status_value<int>  m_status;

  bool  m_working_flag=false;

  struct flags{
    static constexpr int  working = 1;
    static constexpr int  pausing = 2;
  };


  friend class clock_master;

public:
  clock() noexcept{}
  clock(std::string_view name, int  permil=1000) noexcept: m_name(name), m_permil(permil){reset();}

  operator bool() const noexcept{return m_status.test(flags::working) && !m_status.test(flags::pausing);}

  const std::string&  get_name() const noexcept{return m_name;}

  const uint32_t&   get_time() const noexcept{return m_time;}
  const int&      get_permil() const noexcept{return m_permil;}

  bool  is_working() const noexcept{return m_status.test(flags::working);}
  bool  is_pausing() const noexcept{return m_status.test(flags::pausing);}

  clock&   turnon() noexcept{  m_status.set(  flags::working);  return *this;}
  clock&  turnoff() noexcept{  m_status.unset(flags::working);  return *this;}

  clock&    pause() noexcept{  m_status.set(  flags::pausing);  return *this;}
  clock&  unpause() noexcept{  m_status.unset(flags::pausing);  return *this;}

  clock&  reset(uint32_t  time=0, uint32_t  fraction=0) noexcept;

  clock&  add(uint32_t  t) noexcept;

  clock&  set_permil(int  v) noexcept{  m_permil = v;  return *this;}

  clock&    start() noexcept{            m_working_flag =  true;  return *this;}
  clock&  restart() noexcept{  reset();  m_working_flag =  true;  return *this;}
  clock&     stop() noexcept{            m_working_flag = false;  return *this;}

};


class
clock_watch
{
  static const clock  m_null;

  const clock*  m_pointer=&m_null;

public:
  clock_watch() noexcept{}
  clock_watch(const clock&  ref) noexcept: m_pointer(&ref){}

  const uint32_t&   get_time() const noexcept{return m_pointer->get_time();}

};


class
clock_master
{
  std::list<clock*>  m_list;

  uint32_t  m_last_time=0;

public:
  clock_master() noexcept{}
  clock_master(const clock_master& ) noexcept=delete;
  clock_master(      clock_master&&) noexcept=delete;
 ~clock_master(){}

  clock_master&  operator=(const clock_master& ) noexcept=delete;
  clock_master&  operator=(      clock_master&&) noexcept=delete;

  clock_master&  add(clock&  cl) noexcept;
  clock_master&  remove(std::string_view  name) noexcept;

  clock_master&  update() noexcept;

  void  print() const noexcept;

};




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

};




class execution;
class process;

using execution_callback = void(*)(execution&,dummy&);


struct
execution_entry
{
  uintptr_t  m_nameptr;
  uintptr_t  m_callback;
  uintptr_t  m_data;

  template<typename  T> constexpr uintptr_t  cast(T*  ptr) noexcept{return reinterpret_cast<uintptr_t>(ptr);}

  constexpr execution_entry() noexcept:
  m_nameptr(0), m_callback(0), m_data(0){}

  template<typename  T>
  constexpr execution_entry(const char*  name, void(*cb)(execution&)) noexcept:
  m_nameptr(cast(name)), m_callback(cast(cb)), m_data(0){}

  template<typename  T>
  constexpr execution_entry(const char*  name, void(*cb)(execution&,T&), T&  data) noexcept:
  m_nameptr(cast(name)), m_callback(cast(cb)), m_data(cast(&data)){}

  template<typename  T>
  constexpr execution_entry(const char*  name, T&  data) noexcept:
  m_nameptr(cast(name)), m_callback(cast(T::execute)), m_data(cast(&data)){}

  constexpr const char*  get_name() const noexcept{return m_nameptr? reinterpret_cast<const char*>(m_nameptr):"";}

  execution_callback  get_callback()  const noexcept{return reinterpret_cast<execution_callback>(m_callback);}
  dummy*                  get_data()      const noexcept{return reinterpret_cast<dummy*>(m_data);}

  constexpr bool  test_interruption() const noexcept{return get_name()[0] == '*';}

};


class
execution
{
protected:
  clock_master  m_clock_master;

  task_list  m_task_list;

  color  m_background_color;

  uint32_t  m_pc=0;//Program Counter
  uint32_t  m_lc=0;//Level Counter
  uint32_t  m_sp=0;//Stack Pointer
  uint32_t  m_bp=0;//Base Pointer

  std::vector<uintptr_t>  m_memory;

  bool  m_verbose_flag=false;
  bool  m_pc_barrier=false;

   execution() noexcept: m_memory(800){}
  ~execution(){}

  execution(const execution& )=default;
  execution(      execution&&)=default;

  execution&  operator=(const execution& )=default;
  execution&  operator=(      execution&&)=default;

  execution&  reset() noexcept;

public:
  operator bool() const noexcept{return m_lc;}

  bool  test_verbose_flag() const noexcept{return m_verbose_flag;}
  execution&    set_verbose_flag() noexcept{  m_verbose_flag =  true;  return *this;}
  execution&  unset_verbose_flag() noexcept{  m_verbose_flag = false;  return *this;}

  execution&  add_clock(clock&  cl) noexcept{  m_clock_master.add(cl);  return *this;}
  execution&   add_task(task&  tsk) noexcept{  m_task_list.add(tsk);    return *this;}

  execution&  remove_clock(std::string_view  name) noexcept{  m_clock_master.remove(name);  return *this;}
  execution&   remove_task(std::string_view  name) noexcept{  m_task_list.remove_by_name(name);  return *this;}

  color       get_background_color(        ) const noexcept{return m_background_color                   ;}
  execution&  set_background_color(color  c)       noexcept{       m_background_color = c;  return *this;}

  const uint32_t&  get_pc() const noexcept{return m_pc;}
  const uint32_t&  get_lc() const noexcept{return m_lc;}
  const uint32_t&  get_sp() const noexcept{return m_sp;}
  const uint32_t&  get_bp() const noexcept{return m_bp;}

  execution&     push(std::initializer_list<execution_entry>  ls, const char*  name=nullptr) noexcept;
  execution&  replace(std::initializer_list<execution_entry>  ls, const char*  name=nullptr) noexcept;

  execution&  pop() noexcept;

  execution&  operator++() noexcept;

  void  print() const noexcept;

};


class
process: public execution
{
  std::string  m_name;

  uint32_t  m_interval=0;
  uint32_t  m_next_time=0;

  canvas  m_canvas;

public:
  process() noexcept{}

  bool  is_busy() const noexcept{return *this;}

  std::string_view  get_name() const noexcept{return m_name.size()? m_name:"NoName";}

  process&  assign(std::string_view  name, execution_entry  start) noexcept;

  process&  set_canvas(const canvas&  cv) noexcept{  m_canvas = cv;  return *this;}
  process&  set_interval(uint32_t  t) noexcept{  m_interval = t;  return *this;}

  bool  step() noexcept;

};




}




#endif




