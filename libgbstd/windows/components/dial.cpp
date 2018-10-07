#include"libgbstd/windows/component.hpp"



namespace gbstd{




int
dial_event::
get_new_value() const noexcept
{
  return (*this)->get_current();
}




constexpr const char*  format = "%2d/%2d";


dial::
dial(int  min, int  max, void  (*callback)(dial_event  evt)) noexcept:
m_current(min),
m_min(min),
m_max(max),
m_callback(callback)
{
  m_down_show = new iconshow({&icons::left ,&icons::sunken_left });
  m_up_show   = new iconshow({&icons::right,&icons::sunken_right});

  m_down_button = new button(m_down_show,down);
  m_up_button   = new button(  m_up_show,  up);

  m_down_button->set_userdata(this);
  m_up_button->set_userdata(  this);


  string_form  fs;

  m_label = new label(fs(format,min,max));

  append_row_child({m_down_button,m_up_button,m_label});
}




void
dial::
set_current(int  v) noexcept
{
  m_current = std::min(m_max,std::max(m_min,v));

  update_label();
}


void
dial::
update_label() noexcept
{
  string_form  fs;

  m_label->set_text(fs(format,m_current,m_max));
}


void
dial::
up(button_event  evt) noexcept
{
  auto&  dial = *evt->get_userdata<gbstd::dial>();

  dial.m_up_show->set_index(evt.is_press()? 1:0);

    if(evt.is_release())
    {
        if(dial.m_current < dial.m_max)
        {
          auto  old_value = dial.m_current++;

            if(dial.m_callback)
            {
              dial.m_callback({dial,old_value});
            }


          dial.update_label();
        }
    }
}


void
dial::
down(button_event  evt) noexcept
{
  auto&  dial = *evt->get_userdata<gbstd::dial>();

  dial.m_down_show->set_index(evt.is_press()? 1:0);

    if(evt.is_release())
    {
        if(dial.m_current > dial.m_min)
        {
          auto  old_value = dial.m_current--;

            if(dial.m_callback)
            {
              dial.m_callback({dial,old_value});
            }


          dial.update_label();
        }
    }
}


}




