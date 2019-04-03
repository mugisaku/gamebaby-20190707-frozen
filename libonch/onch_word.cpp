#include"libonch/onch.hpp"
#include"libgbstd/sound.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




onch_word&
onch_word::
set_l_index(int  i) noexcept
{
  unset_l_index();

  m_data |= ((8|i)<<8);

  return *this;
}


onch_word&
onch_word::
set_v_index(int  i) noexcept
{
  unset_v_index();

  m_data |= ((8|i)<<4);

  return *this;
}


onch_word&
onch_word::
set_f_index(int  i) noexcept
{
  unset_f_index();

  m_data |= ((8|i));

  return *this;
}




onch_word&
onch_word::
unset_l_index() noexcept
{
  m_data &= ~0x0F00;

  return *this;
}


onch_word&
onch_word::
unset_v_index() noexcept
{
  m_data &= ~0x00F0;

  return *this;
}


onch_word&
onch_word::
unset_f_index() noexcept
{
  m_data &= ~0x000F;

  return *this;
}




uint32_t
onch_word::
get_length(onch_output_context&  ctx) const noexcept
{
  constexpr uint32_t  length_table[] = {
      40,
      80,
     160,
     320,
     640,
    1280,
    2560,
    5120,
  };


  return length_table[ctx.get_l_index(*this)];
}


void
onch_word::
output(sound_kind  k, onch_output_context&  ctx) const noexcept
{
  constexpr double  volume_max = 0.8;

  constexpr double  frequency_table[] = {
      80.0,
     120.0,
     180.0,
     270.0,
     405.0,
     607.5,
     911.25,
    1366.875,
  };


  auto  length = get_length(ctx);

  auto  num_samples = gbstd::get_number_of_samples_by_time(length);

    if(!test_rest_flag())
    {
      auto  v = volume_max/8*(1+ctx.get_v_index(*this));
      auto  f = frequency_table[ctx.get_f_index(*this)];

      gbstd::sound_event  evt(f,v,length);

        switch(k)
        {
      case(sound_kind::square_wave):
          {
            gbstd::square_wave_device  sq;

            sq.input(evt);

            sq.generate_for_number_of_samples(num_samples,ctx.m_it);
          }
          break;
      case(sound_kind::noise):
          {
            gbstd::noise_device  ns;

            ns.input(evt);

            ns.generate_for_number_of_samples(num_samples,ctx.m_it);
          }
          break;
      case(sound_kind::short_noise):
          {
            gbstd::short_noise_device  sns;

            sns.input(evt);

            sns.generate_for_number_of_samples(num_samples,ctx.m_it);
          }
          break;
        }
    }


  ctx.m_it += num_samples;
}


void
onch_word::
print() const noexcept
{
    if(test_rest_flag())
    {
      printf("r");

        if(test_l_index()){printf("%d",1+get_l_index());}
      else                {printf("?");}
    }

  else
    {
      printf("p");

        if(test_l_index()){printf("%d",1+get_l_index());}
      else                {printf("?");}

      printf("v");

        if(test_v_index()){printf("%d",1+get_v_index());}
      else                {printf("?");}

      printf("f");

        if(test_f_index()){printf("%d",1+get_f_index());}
      else                {printf("?");}
    }
}


}




