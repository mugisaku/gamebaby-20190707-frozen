#ifndef clock_hpp_was_included
#define clock_hpp_was_included


#include"libgbstd/utility.hpp"
#include"libgbstd/weak_reference_counter.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/image.hpp"
#include<list>




namespace gbstd{




class clock;


class
clock_control
{
  clock*  m_clock=nullptr;

public:
  constexpr clock_control() noexcept{}
  constexpr clock_control(clock&  clk) noexcept: m_clock(&clk){}

  clock&  operator *() const noexcept{return *m_clock;}
  clock*  operator->() const noexcept{return  m_clock;}

};


class
clock_watch
{
  const clock*  m_clock=nullptr;

public:
  constexpr clock_watch() noexcept{}
  constexpr clock_watch(const clock&  clk) noexcept: m_clock(&clk){}

  const std::string&   get_name() const noexcept;
  const uint32_t&     get_value() const noexcept;

};


class
clock
{
  std::string  m_name;

  uint32_t  m_value=0;
  uint32_t  m_buffer=0;

  int  m_permil=0;

  bool  m_stopped_flag=false;

public:
  clock() noexcept{}

  clock(const std::string&  name, int  permil=0, bool  immediately=false) noexcept:
  m_name(name), m_permil(permil), m_stopped_flag(!immediately){}

  const std::string&  get_name() const noexcept{return m_name;}

  const uint32_t&  get_value() const noexcept{return m_value;}
  const int&  get_permil() const noexcept{return m_permil;}

  clock&  set_value(     uint32_t  v) noexcept{  m_value      = v;  return *this;}
  clock&  set_permil(int  v) noexcept{  m_permil = v;  return *this;}

  void  reset() noexcept;

  void    start() noexcept{            m_stopped_flag = false;}
  void  restart() noexcept{  reset();  m_stopped_flag = false;}
  void     stop() noexcept{            m_stopped_flag =  true;}

  bool  is_stopped() const noexcept{return m_stopped_flag;}

  void  step(uint32_t  t) noexcept;

  void  print() const noexcept;

};


class
clock_master
{
  std::vector<clock>  m_clock_list;

  uint32_t  m_last_time=0;

public:
  clock_master(){}

  clock*  find(const std::string&  name) noexcept;
  clock&  operator[](const std::string&  name) noexcept;

  void  operator+=(clock  t) noexcept{m_clock_list.emplace_back(t);}

  void  step() noexcept;

  void  print() const noexcept;

};


}




#endif




