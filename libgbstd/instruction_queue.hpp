#ifndef gbstd_instruction_queue_hpp_is_included
#define gbstd_instruction_queue_hpp_is_included


#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include"libgbstd/process.hpp"
#include<vector>


namespace gbstd{




class
instruction
{
public:
  intptr_t  callback;
  intptr_t   opvalue;

  template<typename  T>
  constexpr instruction(void(*cb)(T&)) noexcept:
  callback(reinterpret_cast<intptr_t>(cb)), opvalue(0){}

  template<typename  T>
  constexpr instruction(void(*cb)(T&,int), int  i) noexcept:
  callback(reinterpret_cast<intptr_t>(cb)), opvalue(i){}

  template<typename  T>
  constexpr instruction(void(*cb)(T&,fixed_t), fixed_t  f) noexcept:
  callback(reinterpret_cast<intptr_t>(cb)), opvalue(f.get_raw_integer()){}

};


class
instruction_queue
{
  std::vector<intptr_t>  m_container = {0};

  int  m_rp=0;
  int  m_wp=0;

  void  put(intptr_t  v) noexcept;

  void  push(intptr_t  cb, intptr_t  opval) noexcept;

  void  invoke(void*  data) noexcept;

public:
  operator bool() const noexcept{return m_container[m_rp];}

  void  operator()() noexcept{invoke(nullptr);}

  template<typename  T>
  void  operator()(T&  t) noexcept{invoke(&t);}

  void  clear() noexcept;

  void  push(std::initializer_list<instruction>  ls) noexcept;
  void  push(instruction  instr) noexcept{push({instr});}

  void  pop() noexcept;

};




using real_point = basic_point<fixed_t>;


}




#endif




