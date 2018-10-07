#include"libgbstd/windows/component.hpp"



namespace gbstd{




struct
checkbox::
shared_data
{
  uint32_t  m_reference_count=0;

  void  (*m_callback)(checkbox_event  evt)=nullptr;

  checkbox*  m_first=nullptr;
  checkbox*  m_last =nullptr;

  checkbox*  m_last_changed=nullptr;

};


void
checkbox::
iconshow_callback(iconshow_event  evt) noexcept
{
    if(evt.is_click())
    {
      auto&  chkbox = *evt->get_userdata<checkbox>();

      auto  cb = chkbox.m_data->m_callback;

        if(cb)
        {
            if(chkbox)
            {
              evt->set_index(0);

              cb({chkbox,checkbox_event::kind::unset});
            }

          else
            {
              evt->set_index(1);

              cb({chkbox,checkbox_event::kind::set});
            }
        }
    }
}


void
checkbox::
iconshow_radio_callback(iconshow_event  evt) noexcept
{
    if(evt.is_click())
    {
      auto&  chkbox = *evt->get_userdata<checkbox>();

      auto  cb = chkbox.m_data->m_callback;

        if(cb && !chkbox)
        {
          auto  dat = chkbox.m_data;

            if(dat->m_last_changed)
            {
              dat->m_last_changed->m_iconshow->set_index(0);

              cb({*dat->m_last_changed,checkbox_event::kind::unset});
            }


          evt->set_index(1);

          cb({chkbox,checkbox_event::kind::set});

          dat->m_last_changed = &chkbox;
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




void
checkbox::
revise_to_radio() noexcept
{
  m_iconshow->assign({&icons::radio_unchecked,&icons::radio_checked},iconshow_radio_callback);
}




}




