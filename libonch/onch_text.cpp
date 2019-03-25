#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




uint32_t
onch_text::
get_length() const noexcept
{
  uint32_t  l = 0;

    for(auto  w: m_words)
    {
      l += w.get_length();
    }


  return l;
}




void
onch_text::
output(sound_kind  k, onch_output_context&  ctx) const noexcept
{
    for(auto  w: m_words)
    {
      w.output(k,ctx);
    }
}


void
onch_text::
print() const noexcept
{
    for(auto  w: m_words)
    {
      w.print();

      printf(" ");
    }
}




}




