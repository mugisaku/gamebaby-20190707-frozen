#include"libgbstd/windows/component.hpp"



namespace gbstd{




struct
checkbox::
shared_data
{
  uint32_t  m_reference_count=0;

  void*  m_userdata=nullptr;

  void  (*m_callback)(checkbox_event  evt)=nullptr;

  checkbox*  m_first=nullptr;
  checkbox*  m_last =nullptr;

  checkbox*  m_last_changed=nullptr;

  bool  m_radio_mark=false;

};


void
checkbox::
iconshow_callback(iconshow_event  evt) noexcept
{
    if(evt.is_click())
    {
      auto&  chkbox = *evt->get_userdata<checkbox>();

        if(chkbox.is_checked()){chkbox.uncheck();}
      else                     {chkbox.check();}
    }
}


void
checkbox::
iconshow_radio_callback(iconshow_event  evt) noexcept
{
    if(evt.is_click())
    {
      auto&  chkbox = *evt->get_userdata<checkbox>();

        if(!chkbox.is_checked())
        {
          chkbox.check();
        }
    }
}




checkbox::
checkbox(const char16_t*  text, void  (*cb)(checkbox_event)) noexcept:
m_label(new label(text)),
m_data(new shared_data),
m_iconshow(new iconshow({&icons::unchecked,&icons::checked},iconshow_callback)),
m_entry_number(0)
{
  m_iconshow->set_userdata(this);

  m_data->m_callback = cb;

  m_data->m_first = this;
  m_data->m_last  = this;

  append_row_child({m_iconshow,m_label});

  ++m_data->m_reference_count;
}


checkbox::
checkbox(const checkbox&  member, const char16_t*  text) noexcept:
m_label(new label(text)),
m_data(member.m_data),
m_iconshow(new iconshow({&icons::unchecked,&icons::checked},iconshow_callback)),
m_entry_number(member.m_entry_number+1)
{
  m_iconshow->set_userdata(this);

  m_data->m_last->m_next = this;
  m_data->m_last         = this;

  append_row_child({m_iconshow,m_label});

  ++m_data->m_reference_count;
}


checkbox::
~checkbox()
{
    if(m_data)
    {
      auto&  refcount = m_data->m_reference_count;

        if(!--refcount)
        {
          delete m_data;
        }
    }
}




void*
checkbox::
get_common_userdata_internal() const noexcept
{
  return m_data->m_userdata;
}


void
checkbox::
set_common_userdata(void*  ptr) const noexcept
{
  m_data->m_userdata = ptr;
}




void
checkbox::
check() noexcept
{
    if(is_checked())
    {
      return;
    }


  auto  cb = m_data->m_callback;

    if(m_data->m_radio_mark)
    {
        if(m_data->m_last_changed)
        {
          m_data->m_last_changed->m_iconshow->set_index(0);

            if(cb)
            {
              cb({*m_data->m_last_changed,checkbox_event::kind::unset});
            }
        }


      m_iconshow->set_index(1);

        if(cb)
        {
          cb({*this,checkbox_event::kind::set});
        }


      m_data->m_last_changed = this;
    }

  else
    {
      m_iconshow->set_index(1);

        if(cb)
        {
          cb({*this,checkbox_event::kind::set});
        }
    }
}


void
checkbox::
uncheck() noexcept
{
  auto  cb = m_data->m_callback;

    if(!m_data->m_radio_mark && is_checked())
    {
      m_iconshow->set_index(0);

        if(cb)
        {
          cb({*this,checkbox_event::kind::unset});
        }
    }
}




void
checkbox::
revise_to_radio() noexcept
{
  m_iconshow->assign({&icons::radio_unchecked,&icons::radio_checked},iconshow_radio_callback);

  m_data->m_radio_mark = true;
}




}




