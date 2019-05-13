#include"libww/ww_field.hpp"
#include"libww/ww_character.hpp"




namespace ww{
namespace battles{




field::
field() noexcept
{
}




void
field::
draw(const gbstd::canvas&  cv, field&  f) noexcept
{
  auto&  name = f.m_character->m_head.m_name;

  cv.draw_string({gbstd::colors::white},name.data(),f.m_name_point.x,f.m_name_point.y);

  f.m_hp_bar.draw(cv,f.m_hp_bar);
}


void
field::
tick(field&  f) noexcept
{
  constexpr gbstd::fixed_t  length_max = 200;

  auto&  c = *f.m_character;

  auto  hp     = c.m_product_body.m_hp    ;
  auto  hp_max = c.m_product_body.m_hp_max;

    if((f.m_last_hp     != hp    ) ||
       (f.m_last_hp_max != hp_max))
    {
      f.m_last_hp     = hp    ;
      f.m_last_hp_max = hp_max;

      auto  n = length_max/hp_max*hp;

      f.m_hp_bar.set_desired_length(n).update_increment();
    }


  auto  l = f.m_hp_bar.get_length();

  gbstd::color  color = (l >= length_max/2)? gbstd::colors::white
                       :(l >= length_max/4)? gbstd::colors::yellow
                       :                     gbstd::colors::red;

  f.m_hp_bar.set_color(color);

  f.m_hp_bar.tick(f.m_hp_bar);
}




}}




