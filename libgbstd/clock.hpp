#ifndef clock_hpp_was_included
#define clock_hpp_was_included


#include"libgbstd/utility.hpp"
#include"libgbstd/weak_reference_counter.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/image.hpp"
#include<string_view>




namespace gbstd{




struct clock_entity;


class
clock_control
{
  clock_entity*  m_entity=nullptr;

public:
  clock_control() noexcept{}
  clock_control(clock_entity&  ent) noexcept: m_entity(&ent){}

  const clock_entity&   get_entity() const noexcept{return *m_entity;}

  const uint32_t&   get_time() const noexcept;
  const int&      get_permil() const noexcept;

  bool  is_working() const noexcept;
  bool  is_stopped() const noexcept;

  clock_control&  set_time(uint32_t  v) noexcept;
  clock_control&  set_permil(   int  v) noexcept;

  void  reset() noexcept;

  void    start() noexcept;
  void  restart() noexcept;
  void     stop() noexcept;

};


class
clock_watch
{
  const clock_entity*  m_entity=nullptr;

public:
  clock_watch() noexcept{}
  clock_watch(const clock_entity&  ent) noexcept: m_entity(&ent){}
  clock_watch(const clock_control&  ctrl) noexcept: m_entity(&ctrl.get_entity()){}

  const uint32_t&   get_time() const noexcept;
  const int&      get_permil() const noexcept;

  bool  is_working() const noexcept;
  bool  is_stopped() const noexcept;

};


class
clock_master
{
  clock_entity*  m_top_pointer=nullptr;

  uint32_t  m_last_time=0;

public:
  clock_master() noexcept{}
  clock_master(const clock_master& ) noexcept=delete;
  clock_master(      clock_master&&) noexcept=delete;
 ~clock_master();

  clock_master&  operator=(const clock_master& ) noexcept=delete;
  clock_master&  operator=(      clock_master&&) noexcept=delete;

  clock_control  operator[](std::string_view  name) noexcept;

  clock_control  add(std::string_view  name, int  permil=1000) noexcept;

  void  step() noexcept;

  void  print() const noexcept;

  static clock_watch  get_zero_clock_watch() noexcept;

};


}




#endif




