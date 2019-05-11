#include"libww/ww_field.hpp"
#include"libww/ww_character.hpp"




namespace ww{
namespace battles{




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
  f.m_hp_bar.tick(f.m_hp_bar);
}




}}




