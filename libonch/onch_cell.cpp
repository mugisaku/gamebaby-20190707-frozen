#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




uint32_t
onch_cell::
get_length() const noexcept
{
  uint32_t  l = 0;

    for(auto&  txt: m_texts)
    {
      l += txt.get_length();
    }


  return l;
}




void
onch_cell::
output(onch_output_context&  ctx) const noexcept
{
    for(auto&  txt: m_texts)
    {
      txt.output(m_kind,ctx);
    }
}


void
onch_cell::
print() const noexcept
{
    switch(m_kind)
    {
  case(sound_kind::square_wave):
      printf("square{");
      break;
  case(sound_kind::noise):
      printf("noise{");
      break;
  case(sound_kind::short_noise):
      printf("short_noise{");
      break;
    }


    for(auto&  txt: m_texts)
    {
      txt.print();
    }

  printf("}");
}




}




