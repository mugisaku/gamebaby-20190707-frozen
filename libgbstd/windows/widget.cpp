#include"libgbstd/window.hpp"



namespace gbstd{




widget::
widget(int  w, int  h) noexcept:
style_box(w,h)
{
     show();
  display();
}




bool
widget::
test_by_point(point  pt) const noexcept
{
  auto  pos = get_content_position();

  return((pt.x >=  pos.x                      ) &&
         (pt.y >=  pos.y                      ) &&
         (pt.x <  (pos.x+get_content_width() )) &&
         (pt.y <  (pos.y+get_content_height())));
}


widget*
widget::
find_by_point(point  pt) noexcept
{
    if(test_by_point(pt) && is_displayed())
    {
        if(is_autonomous() || m_children.empty())
        {
          return this;
        }

      else
        if(m_solo_widget)
        {
            if(m_solo_widget->is_displayed())
            {
              auto  w = m_solo_widget->find_by_point(pt);

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
                  auto  w = child->find_by_point(pt);

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
//      printf("widget request_redraw error: %s::%s windowがない\n",get_class_name(),m_name.data());

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
reform(point  parent_content_position) noexcept
{
  set_base_position(parent_content_position+m_offset);

    if(m_children.empty())
    {
      return;
    }


  int  w = 0;
  int  h = 0;

  const widget*  previous = nullptr;

    for(auto&  child: m_children)
    {
        if(child->is_displayed())
        {
          auto  pos = get_content_position();

            if(previous)
            {
                if(child->m_follow_style == follow_style::right)
                {
                  pos = previous->get_base_position();

                  pos.x += previous->get_width();
                }

              else
                if(child->m_follow_style == follow_style::bottom)
                {
                  pos = previous->get_base_position();

                  pos.y += previous->get_height();
                }
            }


          child->reform(pos);


          auto  child_pos = child->get_content_position()-get_content_position();

          w = std::max(w,child_pos.x+child->get_width() );
          h = std::max(h,child_pos.y+child->get_height());


          previous = child.get();
        }
    }


  set_content_width( w);
  set_content_height(h);
}


void
widget::
redraw(const image&  img) noexcept
{
    if(!is_hidden() && is_displayed())
    {
      auto  rect = get_content_rectangle();

      canvas  cv(img,rect);

      render(cv);

        for(auto&  child: m_children)
        {
          child->redraw(img);
        }
    }


  unset_flag(flags::redraw_queued);
}




void
widget::
do_on_mouse_enter() noexcept
{
}


void
widget::
do_on_mouse_leave() noexcept
{
}


void
widget::
do_on_mouse_act(point  mouse_pos) noexcept
{
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
      child->set_offset(point(x,y));

      m_children.emplace_back(child);
    }
}


void
widget::
append_column_child(widget*  child) noexcept
{
    if(child)
    {
      child->set_offset(point(0,0));

      child->m_follow_style = follow_style::bottom;

      m_children.emplace_back(child);
    }
}


void
widget::
append_row_child(widget*  child) noexcept
{
    if(child)
    {
      child->set_offset(point(0,0));

      child->m_follow_style = follow_style::right;

      m_children.emplace_back(child);
    }
}


void
widget::
print(int  indent) const noexcept
{
  printf("%p %s(%s) - flags:%d w:%4d h:%4d rel_pt{%4d,%4d} abs_pt{%4d,%4d}",this,m_name.data(),get_class_name(),
    m_state,
    get_width(),
    get_height(),
    m_offset.x,
    m_offset.y,
    get_base_position().x,
    get_base_position().y
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




