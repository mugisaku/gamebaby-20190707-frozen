#include"libgbstd/process.hpp"




namespace gbstd{




namespace{
constexpr uint32_t  g_offset_of_frame_nameptr = 0;
constexpr uint32_t  g_offset_of_previous_pc   = 1;
constexpr uint32_t  g_offset_of_previous_bp   = 2;
constexpr uint32_t  g_offset_of_entry_array   = 3;
constexpr uint32_t  g_size_of_entry           = 4;
}



execution&
execution::
operator++() noexcept
{
  m_pc += g_size_of_entry;

  return *this;
}


void
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
      m_memory[m_sp++] = ent.m_stop_sign;
    }


  ++m_lc;

    if(test_verbose_flag())
    {
      print();

      printf("\n*frame:\"%s\" pushed\n\n\n",name? name:"-noname-");
    }
}


void
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
      m_memory[m_sp++] = ent.m_stop_sign;
    }
}


void
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
}


void
execution::
print() const noexcept
{
  printf("[exec:\"%s\"] {pc:%3u}, {bp:%3u}, {sp: %3u}",get_name(),m_pc,m_bp,m_sp);
}




process&
process::
assign(std::initializer_list<execution_entry>  ls) noexcept
{
  m_pc = 0;
  m_lc = 0;
  m_bp = 0;
  m_sp = 0;

  m_memory[g_offset_of_previous_pc] = 0;
  m_memory[g_offset_of_previous_bp] = 0;

  push(ls);

  return *this;
}




void
process::
step() noexcept
{
  int  counter = 8;

    while(*this)
    {
        if(m_pc < m_sp)
        {
          auto&  ent = *reinterpret_cast<const execution_entry*>(&m_memory[m_pc]);

          auto    cb = ent.get_callback();
          auto  data = ent.get_data();

            if(test_verbose_flag())
            {
              print();
              printf("\n*calback:\"%s\" invoked\n\n",ent.get_name());
            }


          cb(*this,data);


          auto  stop_sign(ent.m_stop_sign);

            if(!--counter || stop_sign)
            {
              return;
            }
        }

      else
        {
          pop();
        }
    }
}




}




