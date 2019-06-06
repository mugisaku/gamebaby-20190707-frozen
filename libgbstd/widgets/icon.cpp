#include"libgbstd/widget.hpp"




namespace gbstd{
namespace widgets{




icon::
icon(std::initializer_list<int>  ls) noexcept
{
  auto  it     = &m_data[0][0];
  auto  it_end = &m_data[g_icon_size][0];

  auto  src = ls.begin();

    while(it != it_end)
    {
      *it++ = *src++;
    }
}




namespace icons{




const icon
left
{
#include"libgbstd/windows/icons/icon_data__left.txt"
};


const icon
sunken_left
{
#include"libgbstd/windows/icons/icon_data__sunken_left.txt"
};


const icon
right
{
#include"libgbstd/windows/icons/icon_data__right.txt"
};


const icon
sunken_right
{
#include"libgbstd/windows/icons/icon_data__sunken_right.txt"
};


const icon
checked
{
#include"libgbstd/windows/icons/icon_data__checked.txt"
};


const icon
unchecked
{
#include"libgbstd/windows/icons/icon_data__unchecked.txt"
};


const icon
radio_checked
{
#include"libgbstd/windows/icons/icon_data__radio_checked.txt"
};


const icon
radio_unchecked
{
#include"libgbstd/windows/icons/icon_data__radio_unchecked.txt"
};




const icon
plus
{
#include"libgbstd/windows/icons/icon_data__plus.txt"
};


const icon
minus
{
#include"libgbstd/windows/icons/icon_data__minus.txt"
};




const icon
up
{
#include"libgbstd/windows/icons/icon_data__up.txt"
};


const icon
down
{
#include"libgbstd/windows/icons/icon_data__down.txt"
};


}


}}




