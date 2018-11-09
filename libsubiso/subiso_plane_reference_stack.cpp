#include"libsubiso/subiso.hpp"




namespace subiso{




bool  is_valid(const space_view::node*  ptr){return ptr? *ptr->m_box:false;}


int
plane_reference_stack::
element::
get_flags() const noexcept
{
  auto&  box = *m_node->m_box;

  int  flags = 0;

    if(m_plane->m_kind == plane::kind::top)
    {
        if(m_node->m_up_node)
        {
            if(!is_valid(m_node->m_right_node) || is_valid(m_node->m_up_right_node)){flags |=  right_flag;}
            if(!is_valid(m_node->m_left_node)  || is_valid(m_node->m_up_left_node) ){flags |=   left_flag;}
            if(!is_valid(m_node->m_back_node)  || is_valid(m_node->m_up_back_node) ){flags |=    top_flag;}
            if(!is_valid(m_node->m_front_node)                                     ){flags |= bottom_flag;}
        }
    }

  else
    {
        if(!is_valid(m_node->m_right_node) || is_valid(m_node->m_front_right_node)){flags |=  right_flag;}
        if(!is_valid(m_node->m_left_node)  || is_valid(m_node->m_front_left_node) ){flags |=   left_flag;}
        if(!is_valid(m_node->m_down_node)  || is_valid(m_node->m_down_front_node) ){flags |= bottom_flag;}
        if(!is_valid(m_node->m_up_node)                                           ){flags |=    top_flag;}
    }


  return flags;
}




void
plane_reference_stack::
assign(const element*  els, int  n) noexcept
{
  delete[] m_top;

    if(n)
    {
                  m_top = new element[n];
      m_current = m_top                 ;

      m_bottom = m_top+n-1;

        for(int  i = 0;  i < n;  ++i)
        {
          m_top[i] = els[i];
        }
    }

  else
    {
      m_top = nullptr;
    }
}


void
plane_reference_stack::
seek() noexcept
{
  m_current = m_top;

    while(m_current->get_node()->m_box->is_null())
    {
        if(m_current == m_bottom)
        {
          break;
        }


      ++m_current;
    }
}


void
plane_reference_stack::
render(const gbstd::canvas&  cv) noexcept
{
  seek();

  auto&  el = get_current();

  auto  pl = el.get_plane();

    if(pl)
    {
      pl->render(el.get_flags(),cv);
    }
}




}




