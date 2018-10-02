#include"libgbstd/window.hpp"



namespace gbstd{




widget::
widget(int  w, int  h) noexcept:
m_width(w), m_height(h)
{
     show();
  display();
}




bool
widget::
test_by_absolute_point(point  pt) const noexcept
{
  return((pt.x >= m_absolute_position.x) &&
         (pt.y >= m_absolute_position.y) &&
         (pt.x <  (m_absolute_position.x+m_width )) &&
         (pt.y <  (m_absolute_position.y+m_height)));
}


widget*
widget::
find_by_absolute_point(point  pt) noexcept
{
    if(test_by_absolute_point(pt) && is_displayed())
    {
        if(m_children.empty())
        {
          return this;
        }

      else
        if(m_solo_widget)
        {
            if(m_solo_widget->is_displayed())
            {
              auto  w = m_solo_widget->find_by_absolute_point(pt);

                if(w)
                {
                  return w;
                }
            }
        }

      else
        {
            for(auto&  child: m_children)
            {
                if(child->is_displayed())
                {
                  auto  w = child->find_by_absolute_point(pt);

                    if(w)
                    {
                      return w;
                    }
                }
            }
        }
    }


  return nullptr;
}




void
widget::
request_redraw() noexcept
{
    if(!m_window)
    {
      printf("widget::need_to_redraw error: %s::%s rootがない\n",get_class_name(),m_name.data());

      return;
    }


    if(!test_flag(flags::redraw_queued))
    {
      m_window->push_redraw(*this);

      set_flag(flags::redraw_queued);
    }
}


void
widget::
request_reform() noexcept
{
    if(m_window)
    {
      m_window->receive_reform_request();
    }
}




void
widget::
set_solo_widget_by_name(const char*  name) noexcept
{
  std::string  name_s(name);

    for(auto&  child: m_children)
    {
        if(child->get_name() == name_s)
        {
          m_solo_widget = child.get();

          request_reform();

          return;
        }
    }


  printf("container change_current_by_name error: %sというウィジェットは無い",name_s.data());
}


void
widget::
reform(point  parent_absolute_position) noexcept
{
  m_absolute_position = parent_absolute_position+m_relative_position;

  auto&  w = m_width ;
  auto&  h = m_height;

    for(auto&  child: m_children)
    {
        if(child->is_displayed())
        {
          child->reform(m_absolute_position);


          auto&  child_relpos = child->m_relative_position;

          w = std::max(w,child_relpos.x+child->m_width );
          h = std::max(h,child_relpos.y+child->m_height);
        }
    }
}


void
widget::
redraw(const image&  img) noexcept
{
    if(!is_hidden() && is_displayed())
    {
      canvas  cv(img,m_absolute_position.x,m_absolute_position.y,m_width,m_height);

      render(cv);

        for(auto&  child: m_children)
        {
          child->redraw(img);
        }
    }


  unset_flag(flags::redraw_queued);
}




/*
void
widget::
render_background(image_cursor  cur) const noexcept
{
  int  w = m_width;
  int  h = m_height;

  auto&  bgst = *m_background_style;

    if(bgst.is_single_color())
    {
      cur.fill_rectangle(bgst.get_first_color(),0,0,w,h);
    }

  else
    if(bgst.is_stripe())
    {
      cur.draw_stripe_rectangle(bgst.get_first_color(),bgst.get_second_color(),bgst.get_interval(),0,0,w,h);
    }
}
*/




void
widget::
set_window(window*  w) noexcept
{
  m_window = w;

    for(auto&  child: m_children)
    {
      child->set_window(w);
    }
}


void
widget::
append_child(widget*  child, int  x, int  y) noexcept
{
    if(child)
    {
      child->set_relative_position(point(x,y));

      m_children.emplace_back(child);
    }
}




void
widget::
print(int  indent) const noexcept
{
  printf("%p %s(%s) - flags:%d w:%4d h:%4d rel_pt{%4d,%4d} abs_pt{%4d,%4d}",this,m_name.data(),get_class_name(),
    m_state,
    m_width,
    m_height,
    m_relative_position.x,
    m_relative_position.y,
    m_absolute_position.x,
    m_absolute_position.y
  );


  printf_with_indent(indent+2,"{\n");

  auto   it = m_children.cbegin();
  auto  end = m_children.cend();

    if(it != end)
    {
      (*it)->print(indent+2);

        while(++it != end)
        {
          printf_with_indent(indent+2,",\n");

          (*it)->print(indent+2);
        }
    }


  printf_with_indent(indent,"\n}");
}


}




