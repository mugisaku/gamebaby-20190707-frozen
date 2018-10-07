#include"libge/ge.hpp"




class
sample: public gbstd::widget
{
  static constexpr int  size = 32;

public:
  sample(color_maker&  cm) noexcept: gbstd::widget(size,size){set_userdata(&cm);}

  void  render(const gbstd::canvas&  cv) noexcept override
  {
    cv.fill_rectangle(get_userdata<color_maker>()->get_color(),0,0,size,size);
  }

};




color_maker::
color_maker(void  (*callback)(color_maker&,gbstd::color)) noexcept:
m_callback(callback),
m_color(0,0,0)
{
  m_r_dial = new gbstd::dial(0,7,update_color_internal);
  m_g_dial = new gbstd::dial(0,7,update_color_internal);
  m_b_dial = new gbstd::dial(0,7,update_color_internal);

  m_r_dial->set_userdata(this);
  m_g_dial->set_userdata(this);
  m_b_dial->set_userdata(this);

  m_sample = new sample(*this);


  auto  r_label = new gbstd::label(u"[R]");
  auto  g_label = new gbstd::label(u"[G]");
  auto  b_label = new gbstd::label(u"[B]");

  append_column_child({gbstd::make_row({r_label,m_r_dial}),
                       gbstd::make_row({g_label,m_g_dial}),
                       gbstd::make_row({b_label,m_b_dial}),m_sample});
}




void
color_maker::
set_color(gbstd::color  color) noexcept
{
    if(m_color != color)
    {
      m_r_dial->set_current(color.get_r7());
      m_g_dial->set_current(color.get_g7());
      m_b_dial->set_current(color.get_b7());

      update_color();
    }
}


void
color_maker::
update_color_internal(gbstd::dial_event  evt) noexcept
{
  auto&  cm = *evt->get_userdata<color_maker>();

  cm.update_color();
}


void
color_maker::
update_color() noexcept
{
  m_color = gbstd::color(m_r_dial->get_current(),
                         m_g_dial->get_current(),
                         m_b_dial->get_current());

    if(m_callback)
    {
      m_callback(*this,m_color);
    }


  m_sample->request_redraw();
}




