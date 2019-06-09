#include"mkfont.hpp"




std::vector<character>
g_characters =
{
#include"characters.txt"
};


std::vector<combined>
g_combineds =
{
#include"combineds.txt"
};


std::reference_wrapper<character>  g_current_character = std::ref(g_characters[0]);
std::reference_wrapper<combined>   g_current_combined  = std::ref(g_combineds[0]);


character&
get_character_by_unicode(char16_t  unicode) noexcept
{
  static character*  table[0x10000];

  static character  null;

  auto  ptr = table[unicode];

    if(!ptr)
    {
        for(auto&  c: g_characters)
        {
            if(c.m_unicode == unicode)
            {
              table[unicode] = &c;

              return c;
            }
        }


      table[unicode] = &null;

      return null;
    }


  return *ptr;
}


void
character::
render(const canvas&  cv, int  pixel_size) const noexcept
{
  auto  src = m_data;

    for(int  y = 0;  y < m_size;  ++y)
    {
      auto  code = *src++;

        for(int  x = 0;  x < m_size;  ++x)
        {
          auto&  white = colors::white;
          auto&  black = colors::black;

          cv.fill_rectangle((code&0x80)? white:black,pixel_size*x,
                                                     pixel_size*y,
                                                     pixel_size,
                                                     pixel_size);

          code <<= 1;
        }
    }
}




namespace{
void
up_chr(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      evt->get_userdata<editor>().m_chrsel.up();
    }
}


void
down_chr(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      evt->get_userdata<editor>().m_chrsel.down();
    }
}


void
up_cmb(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      evt->get_userdata<editor>().m_cmbsel.up();
    }
}


void
down_cmb(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      evt->get_userdata<editor>().m_cmbsel.down();
    }
}


void
save(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto  f = fopen("characters.txt","wb");

        if(f)
        {
          fprintf(f,"//characters\n");

            for(auto&  c: g_characters)
            {
              fprintf(f,"{0x%04X,{",c.m_unicode);

                for(int  i = 0;  i < character::m_size;  ++i)
                {
                  fprintf(f,"0x%02X,",c.m_data[i]);
                }


              fprintf(f,"}},\n");
            }


          fclose(f);
        }
    }
}


void
save_as_combined(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto  f = fopen("font_data.txt","wb");

        if(f)
        {
            for(auto&  c: g_combineds)
            {
              auto&  u = get_character_by_unicode(c.m_upper);
              auto&  l = get_character_by_unicode(c.m_lower);

              fprintf(f,"{0x%04X,{",c.m_unicode);

                for(int  y = 0;  y < character::m_size;  ++y)
                {
                  fprintf(f,"0x%02X,",u.m_data[y]);
                }


                for(int  y = 0;  y < character::m_size;  ++y)
                {
                  fprintf(f,"0x%02X,",l.m_data[y]);
                }


              fprintf(f,"}},\n");
            }


          fclose(f);
        }
    }
}
}


editor::
editor(widgets::operating_node&  root) noexcept:
m_bitmap(*this),
m_chrsel(*this),
m_cmbsel(*this),
m_character_label(root.create_label().set_string(u"U+0000")),
m_character_up_button(root.create_button().set_content(root.create_label().set_string(u"↑"))),
m_character_down_button(root.create_button().set_content(root.create_label().set_string(u"↓"))),
m_combined_up_button(root.create_button().set_content(root.create_label().set_string(u"↑"))),
m_combined_down_button(root.create_button().set_content(root.create_label().set_string(u"↓"))),
m_save_button(root.create_button().set_content(root.create_label().set_string(u"Save"))),
m_save_as_combined_button(root.create_button().set_content(root.create_label().set_string(u"Save as Combined")))
{
  m_character_up_button.set_callback(    up_chr).set_userdata(this);
  m_character_down_button.set_callback(down_chr).set_userdata(this);
  m_combined_up_button.set_callback(    up_cmb).set_userdata(this);
  m_combined_down_button.set_callback(down_cmb).set_userdata(this);
  m_save_button.set_callback(            save).set_userdata(this);
  m_save_as_combined_button.set_callback(save_as_combined).set_userdata(this);
}




