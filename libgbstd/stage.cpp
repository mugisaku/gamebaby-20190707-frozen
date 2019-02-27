#include"libgbstd/stage.hpp"




namespace gbstd{




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

  m_time            = 0;
  m_time_add_amount = 0;
}


void
task_list::
push(void  (*callback)(uint32_t&,void*), uint32_t  delay, void*  data) noexcept
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
step(uint32_t&  delay, gbstd::process*  proc) noexcept
{
  proc->step();
}
}


void
task_list::
push(uint32_t  delay, gbstd::process&  proc) noexcept
{
  push(reinterpret_cast<void(*)(uint32_t&,void*)>(step),delay,&proc);
}


void
task_list::
process() noexcept
{
  m_time += m_time_add_amount;

  node*     last = nullptr;
  node*  current = m_top;

    while(current)
    {
      uint32_t  next_time = current->m_next_time;

        for(;;)
        {
            if(next_time > m_time)
            {
              current->m_next_time = next_time;

              last = current                       ;
                     current = current->m_next_node;

              break;
            }


          auto&  delay = current->m_delay;;

          current->m_callback(delay,current->m_data);

            if(!delay)
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
std::vector<task_list>  g_major_task_list_stack;
std::vector<task_list>  g_minor_task_list_stack;

std::vector<std::vector<painter>>  g_major_painter_list_stack;
std::vector<std::vector<painter>>  g_minor_painter_list_stack;


template<typename  T, typename  U>
U&
go_next(T&  stack, U&&  element) noexcept
{
  stack.emplace_back(std::move(element));

  element.clear();

  return element;
}


template<typename  T, typename  U>
void
go_back(T&  stack, U&  element) noexcept
{
    if(stack.size())
    {
      element = std::move(stack.back());

      stack.pop_back();
    }
}


uint32_t  g_default_time_add_amount=80;


}


task_list  g_major_task_list;
task_list  g_minor_task_list;

std::vector<painter>  g_major_painter_list;
std::vector<painter>  g_minor_painter_list;


void             set_default_time_add_amount(uint32_t  a) noexcept{       g_default_time_add_amount = a;}
const uint32_t&  get_default_time_add_amount(           ) noexcept{return g_default_time_add_amount    ;}


task_list&
go_next_major_task_list() noexcept
{
  auto&  e = go_next(g_major_task_list_stack,std::move(g_major_task_list));

  g_major_task_list.set_time_add_amount(g_default_time_add_amount);

  return e;
}


task_list&
go_next_minor_task_list() noexcept
{
  auto&  e = go_next(g_minor_task_list_stack,std::move(g_minor_task_list));

  g_minor_task_list.set_time_add_amount(g_default_time_add_amount);

  return e;
}


void  go_back_major_task_list() noexcept{go_back(g_major_task_list_stack,g_major_task_list);}
void  go_back_minor_task_list() noexcept{go_back(g_minor_task_list_stack,g_minor_task_list);}


std::vector<painter>&  go_next_major_painter_list() noexcept{return go_next(g_major_painter_list_stack,std::move(g_major_painter_list));}
std::vector<painter>&  go_next_minor_painter_list() noexcept{return go_next(g_minor_painter_list_stack,std::move(g_minor_painter_list));}

void  go_back_major_painter_list() noexcept{go_back(g_major_painter_list_stack,g_major_painter_list);}
void  go_back_minor_painter_list() noexcept{go_back(g_minor_painter_list_stack,g_minor_painter_list);}


void
process_task_lists() noexcept
{
  g_major_task_list.process();
  g_minor_task_list.process();
}


void
render_painter_lists(const canvas&  cv) noexcept
{
    for(auto&  p: g_major_painter_list)
    {
      p(cv);
    }


    for(auto&  p: g_minor_painter_list)
    {
      p(cv);
    }
}


}




