#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




f32_t
onch_output_context::
get_vibrato_frequency(int  spec, int value) noexcept
{
    if(spec == onch_word::specs::no_spec)
    {
    }

  else
    if(spec == onch_word::specs::zero)
    {
      m_last_vibrato_frequency = 0;
    }

  else
    if(spec == onch_word::specs::index)
    {
      constexpr f32_t  table[] = {
          2,
          4,
          6,
          8,
         10,
         12,
         14,
         16,
      };


      m_last_vibrato_frequency = table[value];
    }


  return m_last_vibrato_frequency;
}




f32_t
onch_output_context::
get_volume(int  spec, int value) noexcept
{
    if(spec == onch_word::specs::no_spec)
    {
    }

  else
    if(spec == onch_word::specs::zero)
    {
      m_last_volume = 0;
    }

  else
    if(spec == onch_word::specs::index)
    {
      constexpr f32_t  volume_max = 0.8;

      m_last_volume = volume_max/8*(1+value);
    }


  return m_last_volume;
}


f32_t
onch_output_context::
get_frequency(int  spec, int value) noexcept
{
    if(spec == onch_word::specs::no_spec)
    {
    }

  else
    if(spec == onch_word::specs::zero)
    {
      m_last_frequency = 0;
    }

  else
    if(spec == onch_word::specs::index)
    {
      constexpr f32_t  frequency_table[] = {
          80.0,
         120.0,
         180.0,
         270.0,
         405.0,
         607.5,
         911.25,
        1366.875,
      };


      m_last_frequency = frequency_table[value];
    }


  return m_last_frequency;
}




}




