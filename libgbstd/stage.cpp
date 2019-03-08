#include"libgbstd/stage.hpp"




namespace gbstd{




class
control: public task_control
{
public:
  using task_control::task_control;

  void  set_delay(uint32_t&  delay) noexcept{m_delay = &delay;}

  bool  is_alive() const noexcept{return m_delay;}

};


bool
task_list::
m_interruption_flag;


task_list&
task_list::
assign(task_list&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_top            ,rhs.m_top            );
      std::swap(m_number_of_nodes,rhs.m_number_of_nodes);
      std::swap(m_time           ,rhs.m_time           );
      std::swap(m_time_add_amount,rhs.m_time_add_amount);
    }


  return *this;
}


void
task_list::
clear() noexcept
{
    while(m_top)
    {
      auto  next = m_top->m_next_node;

      delete m_top       ;
             m_top = next;
    }


  m_number_of_nodes = 0;

  m_time            =                             0;
  m_time_add_amount = get_default_time_add_amount();
}


void
task_list::
push(void  (*callback)(task_control&,void*), uint32_t  delay, void*  data) noexcept
{
  auto  nd = new node;

  nd->m_next_time = m_time+delay;

  nd->m_delay = delay;

  nd->m_callback = callback;

  nd->m_data = data;

  nd->m_next_node = m_top     ;
                    m_top = nd;

  ++m_number_of_nodes;
}


namespace{
void
step(task_control&  ctrl, gbstd::process*  proc) noexcept
{
  proc->step();
}
}


void
task_list::
push(uint32_t  delay, gbstd::process&  proc) noexcept
{
  push(reinterpret_cast<void(*)(task_control&,void*)>(step),delay,&proc);
}


void
task_list::
process() noexcept
{
  m_interruption_flag = false;

  m_time += m_time_add_amount;

  node*     last = nullptr;
  node*  current = m_top;

  control  ctrl(m_time);

    while(current)
    {
      uint32_t  next_time = current->m_next_time;

        for(;;)
        {
            if(m_interruption_flag)
            {
              goto QUIT;
            }


            if(next_time > m_time)
            {
              current->m_next_time = next_time;

              last = current                       ;
                     current = current->m_next_node;

              break;
            }


          auto&  delay = current->m_delay;;

          ctrl.set_delay(delay);

          current->m_callback(ctrl,current->m_data);

            if(!delay || !ctrl.is_alive())
            {
              auto  next = current->m_next_node;

              delete current       ;
                     current = next;

              --m_number_of_nodes;

                if(last)
                {
                  last->m_next_node = current;
                }


              break;
            }


          next_time += delay;
        }
    }


QUIT:
    if(!m_number_of_nodes)
    {
      m_top = nullptr;
    }
}


task_list::iterator&
task_list::iterator::
operator++() noexcept
{
  m_data = m_data->m_next_node;

  return *this;
}




namespace{
task_list*  g_major_task_list[16];
task_list*  g_minor_task_list[16];

task_list**  g_major_task_list_ptr = g_major_task_list;
task_list**  g_minor_task_list_ptr = g_minor_task_list;

painter_list*  g_major_painter_list;
painter_list*  g_minor_painter_list;


uint32_t  g_default_time_add_amount=160;


}


void             set_default_time_add_amount(uint32_t  a) noexcept{       g_default_time_add_amount = a;}
const uint32_t&  get_default_time_add_amount(           ) noexcept{return g_default_time_add_amount    ;}


task_list&  get_major_task_list() noexcept{return **g_major_task_list_ptr;}
task_list&  get_minor_task_list() noexcept{return **g_minor_task_list_ptr;}


void
push_major_task_list(task_list&  ls) noexcept
{
  *++g_major_task_list_ptr = &ls;

  task_list::interrupt();
}


void
push_minor_task_list(task_list&  ls) noexcept
{
  *++g_minor_task_list_ptr = &ls;

  task_list::interrupt();
}


void  pop_major_task_list() noexcept{--g_major_task_list_ptr;}
void  pop_minor_task_list() noexcept{--g_minor_task_list_ptr;}

painter_list*  get_major_painter_list() noexcept{return g_major_painter_list;}
painter_list*  get_minor_painter_list() noexcept{return g_minor_painter_list;}

void  set_major_painter_list(painter_list*  ls) noexcept{g_major_painter_list = ls;}
void  set_minor_painter_list(painter_list*  ls) noexcept{g_minor_painter_list = ls;}


void
process_task_lists() noexcept
{
    if(*g_major_task_list_ptr)
    {
      (*g_major_task_list_ptr)->process();
    }


    if(*g_minor_task_list_ptr)
    {
      (*g_minor_task_list_ptr)->process();
    }
}


void
render_painter_lists(const canvas&  cv) noexcept
{
    if(g_major_painter_list)
    {
        for(auto&  p: *g_major_painter_list)
        {
          p(cv);
        }
    }


    if(g_minor_painter_list)
    {
        for(auto&  p: *g_minor_painter_list)
        {
          p(cv);
        }
    }
}


}




