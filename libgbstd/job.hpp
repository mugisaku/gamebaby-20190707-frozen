#ifndef gbstd_job_HPP
#define gbstd_job_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include<utility>
#include<new>
#include<string>
#include<cstdint>


namespace gbstd{


class  job;
class  job_list;
class  process;


using process_callback = void(*)(process&  proc, void*  dat);


class
job_list
{
  struct element;

  element*  m_first=nullptr;

  int  m_number_of_jobs=0;

public:
  job_list() noexcept{};
  job_list(const job_list& ) noexcept=delete;
  job_list(      job_list&&) noexcept=delete;
 ~job_list() noexcept{clear();}

  job_list&  operator=(const job_list& ) noexcept=delete;
  job_list&  operator=(      job_list&&) noexcept=delete;


  operator bool() const noexcept{return m_first;}

  void  clear() noexcept;

  int  get_number_of_jobs() const noexcept{return m_number_of_jobs;}

  void  add(job&&  job) noexcept;

  template<typename...  ARGS>void  add(ARGS...  args) noexcept{add(job(args...));}


  void  step() noexcept;
  void  step_all() noexcept;

  class iterator{
    element*  m_data;
  public:
    constexpr iterator(element*  e=nullptr) noexcept: m_data(e){}

    constexpr operator bool() const noexcept{return m_data;}

    job&  operator*() const noexcept;

    iterator&  operator++()    noexcept;
    iterator   operator++(int) noexcept;
  };

  iterator  begin() const noexcept{return iterator(m_first);}
  iterator    end() const noexcept{return iterator(       );}

};




class
process
{
  const process*  m_parent=nullptr;

  uint32_t  m_id=0;

  std::string  m_name;

  job_list  m_foreground_job_list;
  job_list  m_background_job_list;

  uint32_t  m_next_time=0;
  uint32_t  m_interval=0;

  uint32_t  m_pc=0;

  void*  m_data=nullptr;

  process_callback  m_callback=nullptr;

  enum class state{
    stopping,
    running,
    waiting,
    exited,
  } m_state=state::running;

  void  execute() noexcept;

public:
  process() noexcept{}
  process(const char*  name, process_callback  cb, uint32_t  intval, void*  dat=nullptr) noexcept{assign(name,cb,intval,dat);}
  process(const process&   rhs) noexcept=delete;
  process(      process&&  rhs) noexcept{assign(std::move(rhs));}
 ~process(){clear();}

  process&  operator=(const process&   rhs) noexcept=delete;
  process&  operator=(      process&&  rhs) noexcept{return assign(std::move(rhs));}

  process&  assign(process&&  rhs) noexcept;
  process&  assign(const char*  name, process_callback  cb, uint32_t  intval, void*  dat=nullptr) noexcept;

  template<typename  T>
  process(const char*  name, void  (*cb)(process&,T*), uint32_t  intval, T*  dat) noexcept
  {
    assign(name,reinterpret_cast<process_callback>(cb),intval,dat);
  }

  template<typename  T>
  process&
  assign(const char*  name, void  (*cb)(process&,T*), uint32_t  intval, T*  dat) noexcept
  {
    return assign(name,reinterpret_cast<process_callback>(cb),intval,dat);
  }


  bool  is_running()  const noexcept{return m_state == state::running;}
  bool  is_stopping() const noexcept{return m_state == state::stopping;}
  bool  is_waiting()  const noexcept{return m_state == state::waiting;}
  bool  is_exited()   const noexcept{return m_state == state::exited;}

  void  clear() noexcept;
  void  reset() noexcept;

  const std::string&  get_name() const noexcept{return m_name;}

  void   run() noexcept{m_state = state::running;}
  void  stop() noexcept{m_state = state::stopping;}
  void  wait() noexcept{m_state = state::waiting;}
  void  exit() noexcept{m_state = state::exited;}

        job_list&  get_foreground_job_list()       noexcept{return m_foreground_job_list;}
  const job_list&  get_foreground_job_list() const noexcept{return m_foreground_job_list;}

        job_list&  get_background_job_list()       noexcept{return m_background_job_list;}
  const job_list&  get_background_job_list() const noexcept{return m_background_job_list;}

  uint32_t  get_id() const noexcept{return m_id;}

  uint32_t  get_pc(           ) const noexcept{return m_pc     ;}
  void      set_pc(uint32_t  v)       noexcept{       m_pc  = v;}
  void      add_pc(int       v)       noexcept{       m_pc += v;}

  operator uint32_t() const noexcept{return m_pc;}

  process&  operator =(uint32_t  v) noexcept{  m_pc  = v;  return *this;}
  process&  operator+=(int       v) noexcept{  m_pc += v;  return *this;}

  uint32_t  get_interval(           ) const noexcept{return m_interval     ;}
  void      set_interval(uint32_t  v)       noexcept{       m_interval  = v;}
  void      add_interval(int       v)       noexcept{       m_interval += v;}

  void  step() noexcept;

};




class
job
{
  process  m_process;

public:
  job() noexcept{}
  job(const job&   rhs) noexcept=delete;
  job(      job&&  rhs) noexcept{assign(std::move(rhs));}
 ~job(){clear();}

  job&  operator=(const job&   rhs) noexcept=delete;
  job&  operator=(      job&&  rhs) noexcept{return assign(std::move(rhs));}

  job&  assign(job&&  rhs) noexcept;

  template<typename...  ARGS>
  job(ARGS...  args) noexcept
  {
    m_process.assign(args...);
  }

  template<typename...  ARGS>
  job&
  assign(ARGS...  args) noexcept
  {
    clear();

    m_process.assign(args...);

    return *this;
  }

  operator bool() const noexcept{return m_process;}

  bool  is_busy()     const noexcept;
  bool  is_finished() const noexcept;

  void  clear() noexcept;

  process&  get_process() noexcept{return m_process;}

  void  step() noexcept;

};




}




#endif




