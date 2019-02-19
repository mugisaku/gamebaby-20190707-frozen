#ifndef stage_hpp_was_included
#define stage_hpp_was_included


#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/image.hpp"
#include<vector>




namespace gbstd{




class
task_list
{
  struct node{
    node*     m_next_node;
    uint32_t  m_next_time;

    uint32_t  m_delay;

    void  (*m_callback)(uint32_t&,void*);

    void*  m_data;

  };

  node*  m_top=nullptr;

  int  m_number_of_nodes=0;

  uint32_t  m_time           =0;
  uint32_t  m_time_add_amount=0;

public:
  task_list(uint32_t  tmaa=0) noexcept: m_time_add_amount(tmaa){}
  task_list(const task_list&   rhs) noexcept=delete;
  task_list(      task_list&&  rhs) noexcept{assign(std::move(rhs));}
 ~task_list(){clear();}

  task_list&  operator=(const task_list&   rhs) noexcept=delete;
  task_list&  operator=(      task_list&&  rhs) noexcept{return assign(std::move(rhs));}

  task_list&  assign(task_list&&  rhs) noexcept;

  const uint32_t&  get_time()            const noexcept{return m_time           ;}
  const uint32_t&  get_time_add_amount() const noexcept{return m_time_add_amount;}

  void  set_time_add_amount(uint32_t  tm) noexcept{m_time_add_amount = tm;}

  void  clear() noexcept;

  void  push(void  (*callback)(uint32_t&  delay, void*  data), uint32_t  delay, void*  data) noexcept;

  void  push(uint32_t  delay, process&  proc) noexcept;

  template<typename  T>
  void  push(void  (*callback)(uint32_t&  delay, T*  data), uint32_t  delay, T*  data) noexcept
  {
    push(static_cast<void(*)(uint32_t&,void*)>(callback),delay,data);
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

};




class
painter
{
  const int*  m_data;

  void  (*m_callback)(const int&  data, const canvas&  cv);

public:
  painter() noexcept: m_data(nullptr), m_callback(nullptr){}

  template<typename  T>
  painter(const T&  obj, void  (*callback)(const T&, const canvas&)) noexcept:
  m_data(reinterpret_cast<const int*>(&obj)),
  m_callback(reinterpret_cast<void(*)(const int&,const canvas&)>(callback)){}

  void  operator()(const canvas&  cv) const noexcept{m_callback(*m_data,cv);}

};


extern task_list  g_major_task_list;
extern task_list  g_minor_task_list;

extern std::vector<painter>  g_major_painter_list;
extern std::vector<painter>  g_minor_painter_list;


void             set_default_time_add_amount(uint32_t  a) noexcept;
const uint32_t&  get_default_time_add_amount(           ) noexcept;

void  go_next_major_task_list() noexcept;
void  go_back_major_task_list() noexcept;

void  go_next_minor_task_list() noexcept;
void  go_back_minor_task_list() noexcept;


void  go_next_major_painter_list() noexcept;
void  go_back_major_painter_list() noexcept;

void  go_next_minor_painter_list() noexcept;
void  go_back_minor_painter_list() noexcept;

void    process_task_lists() noexcept;
void  render_painter_lists(const canvas&  cv) noexcept;


}




#endif




