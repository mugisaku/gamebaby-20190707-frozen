#ifndef clock_hpp_was_included
#define clock_hpp_was_included


#include"libgbstd/control.hpp"
#include"libgbstd/utility.hpp"
#include<string_view>
#include<list>




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


}




#endif




