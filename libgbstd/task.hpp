#ifndef task_hpp_was_included
#define task_hpp_was_included


#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/image.hpp"
#include<list>




namespace gbstd{




struct
task_tag
{
  uint16_t  major_number;
  uint16_t  minor_number;

  constexpr task_tag(uint16_t  maj=0, uint16_t  min=0) noexcept:
  major_number(maj), minor_number(min){}

};


class
draw_task_entry
{
  task_tag  m_tag;

  void  (*m_callback)(const canvas&  cv, dummy&  data);

  dummy*  m_data;

public:
  constexpr draw_task_entry() noexcept:
  m_callback(nullptr), m_data(nullptr){}

  constexpr draw_task_entry(task_tag  tag, void  (*cb)(const canvas&,dummy&)) noexcept:
  m_tag(tag), m_callback(cb), m_data(nullptr){}

  template<typename  T>
  constexpr draw_task_entry(task_tag  tag, void  (*cb)(const canvas&,T&), T&  data) noexcept:
  m_tag(tag),
  m_callback(reinterpret_cast<void(*)(const canvas&,dummy&)>(cb)),
  m_data(reinterpret_cast<dummy*>(&data)){}

  constexpr const task_tag&  get_tag() const noexcept{return m_tag;}

  void  operator()(const canvas&  cv) const noexcept{m_callback(cv,*m_data);}

};


class
draw_task_list
{
  struct reference_counter;
  struct node;

  static struct reference_counter*  m_dead_counter_top;
  static struct node*               m_dead_node_top;

  std::list<node*>  m_container;

  static void  unrefer(node*  ptr) noexcept;
  static void  unrefer(reference_counter*  ptr) noexcept;

public:
  draw_task_list() noexcept{}
 ~draw_task_list(){clear();}

  class control{
    node*                  m_node=nullptr;
    reference_counter*  m_counter=nullptr;

  public:
    constexpr control() noexcept{}
    control(node&  nd) noexcept;
    control(const control&   rhs) noexcept{assign(rhs);}
    control(      control&&  rhs) noexcept{assign(std::move(rhs));}
   ~control();

    control&  operator=(const control&   rhs) noexcept{return assign(rhs);}
    control&  operator=(      control&&  rhs) noexcept{return assign(std::move(rhs));}

    operator bool() const noexcept;

    control&  assign(const control&   rhs) noexcept;
    control&  assign(      control&&  rhs) noexcept;

    control&    set_remove_flag() noexcept;
    control&  unset_remove_flag() noexcept;

    control&    set_skip_flag() noexcept;
    control&  unset_skip_flag() noexcept;

    control&    set_blink_flag() noexcept;
    control&  unset_blink_flag() noexcept;

    control&  set_blinking_rate(int  show, int  hide) noexcept;

    bool  test_remove_flag() const noexcept;
    bool  test_skip_flag()   const noexcept;
    bool  test_blink_flag()   const noexcept;

  };


  void  clear() noexcept;

  control  push(draw_task_entry  ent) noexcept;

  void  remove() noexcept;

  void  process(const canvas&  cv) noexcept;

  void  print() const noexcept;

};


}




#endif




