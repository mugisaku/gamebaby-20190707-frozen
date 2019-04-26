#ifndef weak_reference_counter_hpp_was_included
#define weak_reference_counter_hpp_was_included


#include"libgbstd/utility.hpp"




namespace gbstd{




class
weak_reference_counter
{
  struct data;

  data*  m_data=nullptr;

  static data*  m_dead_top;

  void  unrefer() noexcept;

public:
  weak_reference_counter() noexcept;
  weak_reference_counter(const weak_reference_counter&   rhs) noexcept{assign(rhs);}
  weak_reference_counter(      weak_reference_counter&&  rhs) noexcept{assign(std::move(rhs));}
 ~weak_reference_counter(){unrefer();}

  weak_reference_counter&  operator=(const weak_reference_counter&   rhs) noexcept{return assign(rhs);}
  weak_reference_counter&  operator=(      weak_reference_counter&&  rhs) noexcept{return assign(std::move(rhs));}

  weak_reference_counter&  assign(const weak_reference_counter&   rhs) noexcept;
  weak_reference_counter&  assign(      weak_reference_counter&&  rhs) noexcept;

  weak_reference_counter&  set_value(int  v)       noexcept;
  const int&               get_value(      ) const noexcept;

  void  reset() noexcept;

  operator bool() const noexcept;

  static void  print_dead() noexcept;

};


}




#endif




