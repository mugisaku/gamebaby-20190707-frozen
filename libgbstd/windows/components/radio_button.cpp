#include"libgbstd/windows/component.hpp"



namespace gbstd{




radio_button::
radio_button(const char16_t*  text, void  (*cb)(checkbox_event)) noexcept:
checkbox(text,cb)
{
  revise_to_radio();
}


radio_button::
radio_button(const radio_button&  member, const char16_t*  text) noexcept:
checkbox(member,text)
{
  revise_to_radio();
}




}




