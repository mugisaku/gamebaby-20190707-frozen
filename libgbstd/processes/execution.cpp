#include"libgbstd/process.hpp"




namespace gbstd{




namespace{
constexpr uint32_t  g_offset_of_frame_nameptr = 0;
constexpr uint32_t  g_offset_of_previous_pc   = 1;
constexpr uint32_t  g_offset_of_previous_bp   = 2;
constexpr uint32_t  g_offset_of_entry_array   = 3;
constexpr uint32_t  g_size_of_entry           = 3;
}



execution&
execution::
operator++() noexcept
{
    if(m_pc_barrier)
    {
      printf("execution error: pc is barriered\n");
    }

  else
    {
      m_pc += g_size_of_entry;
    }


  return *this;
}


execution&
execution::
reset() noexcept
{
  m_pc = 0;
  m_lc = 0;
  m_sp = 0;
  m_bp = 0;

  m_memory[g_offset_of_previous_pc] = 0;
  m_memory[g_offset_of_previous_bp] = 0;

  m_verbose_flag = false;
  m_pc_barrier   = false;

  return *this;
}


execution&
execution::
push(std::initializer_list<execution_entry>  ls, const char*  name) noexcept
{
  auto  prev_pc = m_pc;
  auto  prev_bp = m_bp;

  m_bp = m_sp                           ;
         m_sp += g_offset_of_entry_array;


  m_pc = m_sp;

  m_memory[m_bp+g_offset_of_frame_nameptr] = name? reinterpret_cast<uintptr_t>(name):0;
  m_memory[m_bp+g_offset_of_previous_pc]   = prev_pc;
  m_memory[m_bp+g_offset_of_previous_bp]   = prev_bp;

    for(auto  ent: ls)
    {
      m_memory[m_sp++] = ent.m_nameptr;
      m_memory[m_sp++] = ent.m_callback;
      m_memory[m_sp++] = ent.m_data;
    }


  ++m_lc;

    if(test_verbose_flag())
    {
      print();

      printf("\n*frame:\"%s\" pushed\n\n\n",name? name:"-noname-");
    }


  m_pc_barrier = true;


  return *this;
}


execution&
execution::
replace(std::initializer_list<execution_entry>  ls, const char*  name) noexcept
{
  m_memory[m_bp+g_offset_of_frame_nameptr] = name? reinterpret_cast<uintptr_t>(name):0;

         m_pc = m_bp+g_offset_of_entry_array;
  m_sp = m_pc                               ;

    for(auto  ent: ls)
    {
      m_memory[m_sp++] = ent.m_nameptr;
      m_memory[m_sp++] = ent.m_callback;
      m_memory[m_sp++] = ent.m_data;
    }


  m_pc_barrier = true;


  return *this;
}


execution&
execution::
pop() noexcept
{
    if(m_lc)
    {
      auto  name = reinterpret_cast<const char*>(m_memory[m_bp+g_offset_of_frame_nameptr]);

        if(test_verbose_flag())
        {
          print();

          printf("\n*frame:\"%s\" popped\n\n\n",name? name:"-noname-");
        }


      m_pc = m_memory[m_bp+g_offset_of_previous_pc];

      m_sp = m_bp                                         ;
             m_bp = m_memory[m_bp+g_offset_of_previous_bp];

      --m_lc;
    }


  return *this;
}




execution&
execution::
remove_clock_by_name(std::string_view  name) noexcept
{
  auto  it = m_clock_list.begin();

    while(it != m_clock_list.end())
    {
        if((*it)->get_name() == name)
        {
          it = m_clock_list.erase(it);
        }

      else
        {
          ++it;
        }
    }


  return *this;
}


execution&
execution::
remove_task_by_name(std::string_view  name) noexcept
{
  auto  it = m_task_list.begin();

    while(it != m_task_list.end())
    {
        if((*it)->get_name() == name)
        {
          it = m_task_list.erase(it);
        }

      else
        {
          ++it;
        }
    }


  return *this;
}


execution&
execution::
remove_dead_task() noexcept
{
  auto  it = m_task_list.begin();

    while(it != m_task_list.end())
    {
        if(!(*it)->is_living())
        {
          it = m_task_list.erase(it);
        }

      else
        {
          ++it;
        }
    }


  return *this;
}




void
execution::
print() const noexcept
{
  printf("{pc:%3u, bp:%3u, sp: %3u",m_pc,m_bp,m_sp);

    for(auto  ptr: m_clock_list)
    {
      printf("{\"%s\": %8u}",ptr->get_name().data(),ptr->get_time());
    }
}




}




