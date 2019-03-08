#ifndef stage_hpp_was_included
#define stage_hpp_was_included


#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/image.hpp"
#include<vector>




namespace gbstd{




void             set_default_time_add_amount(uint32_t  a) noexcept;
const uint32_t&  get_default_time_add_amount(           ) noexcept;



class
task_control
{
protected:
  uint32_t  m_time;

  uint32_t*  m_delay;

public:
  task_control(uint32_t  tm=0) noexcept: m_time(tm){}

  uint32_t  get_time() const noexcept{return m_time;}

  uint32_t  get_delay() const noexcept{return *m_delay;}

  void  reset_delay(uint32_t  ms) const noexcept{*m_delay = ms;}

  void  kill() noexcept{m_delay = nullptr;}

};


class
task_list
{
  struct node{
    node*     m_next_node;
    uint32_t  m_next_time;

    uint32_t  m_delay;

    void  (*m_callback)(task_control&,void*);

    void*  m_data;

  };

  node*  m_top=nullptr;

  int  m_number_of_nodes=0;

  uint32_t  m_time           =0;
  uint32_t  m_time_add_amount=0;

  static bool  m_interruption_flag;

public:
  task_list(uint32_t  tmaa=get_default_time_add_amount()) noexcept: m_time_add_amount(tmaa){}
  task_list(const task_list&   rhs) noexcept=delete;
  task_list(      task_list&&  rhs) noexcept{assign(std::move(rhs));}
 ~task_list(){clear();}

  task_list&  operator=(const task_list&   rhs) noexcept=delete;
  task_list&  operator=(      task_list&&  rhs) noexcept{return assign(std::move(rhs));}

  task_list&  assign(task_list&&  rhs) noexcept;

  int  get_number_of_nodes() const noexcept{return m_number_of_nodes;}

  const uint32_t&  get_time()            const noexcept{return m_time           ;}
  const uint32_t&  get_time_add_amount() const noexcept{return m_time_add_amount;}

  void  set_time_add_amount(uint32_t  tm) noexcept{m_time_add_amount = tm;}

  void  clear() noexcept;

  void  push(void  (*callback)(task_control&  ctrl, void*  data), uint32_t  delay, void*  data) noexcept;

  void  push(uint32_t  delay, process&  proc) noexcept;

  template<typename  T>
  void  push(void  (*callback)(task_control&  ctrl, T*  data), uint32_t  delay, T*  data) noexcept
  {
    push(reinterpret_cast<void(*)(task_control&,void*)>(callback),delay,data);
  }

  void  process() noexcept;

  class iterator{
    node*  m_data;

  public:
    constexpr iterator(node*  nd=nullptr) noexcept: m_data(nd){}

    constexpr operator bool() const noexcept{return  m_data;}
    constexpr node&  operator *() const noexcept{return *m_data;}
    constexpr node*  operator->() const noexcept{return  m_data;}

    iterator&  operator++() noexcept;

  };

  iterator  begin() const noexcept{return iterator(m_top);}
  iterator    end() const noexcept{return iterator(     );}

  static void  interrupt() noexcept{m_interruption_flag = true;}

};




class
painter
{
  const int*  m_data;

  void  (*m_callback)(const int&  data, const canvas&  cv);

  int*  m_counter;

public:
  painter() noexcept: m_data(nullptr), m_callback(nullptr), m_counter(nullptr){}

  template<typename  T>
  painter(const T&  obj, void  (*callback)(const T&, const canvas&), int*  counter=nullptr) noexcept:
  m_data(reinterpret_cast<const int*>(&obj)),
  m_callback(reinterpret_cast<void(*)(const int&,const canvas&)>(callback)),
  m_counter(counter){}

  void  operator()(const canvas&  cv) const noexcept
  {
    m_callback(*m_data,cv);

      if(m_counter)
      {
        ++*m_counter;
      }
  }

};


using painter_list = std::vector<painter>;


task_list&  get_major_task_list() noexcept;
task_list&  get_minor_task_list() noexcept;

void  push_major_task_list(task_list&  ls) noexcept;
void  push_minor_task_list(task_list&  ls) noexcept;

void  pop_major_task_list() noexcept;
void  pop_minor_task_list() noexcept;

painter_list*  get_major_painter_list() noexcept;
painter_list*  get_minor_painter_list() noexcept;

void  set_major_painter_list(painter_list*  ls) noexcept;
void  set_minor_painter_list(painter_list*  ls) noexcept;

void    process_task_lists() noexcept;
void  render_painter_lists(const canvas&  cv) noexcept;


}




#endif




