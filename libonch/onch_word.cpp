#include"libonch/onch.hpp"
#include"libgbstd/sound.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




uint32_t
onch_word::
get_length() const noexcept
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


  return length_table[get_length_index()];
}


void
onch_word::
output(sound_kind  k, onch_output_context&  ctx) const noexcept
{
  constexpr double  volume_max = 0.1;

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


  auto  length = get_length();

  auto  num_samples = gbstd::get_number_of_samples_by_time(length);

  auto  v = volume_max/8*(1+get_volume_index());
  auto  f = frequency_table[get_frequency_index()];

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


  ctx.m_it += num_samples;
}


void
onch_word::
print() const noexcept
{
  printf("l%dv%df%d",1+get_length_index(),
                     1+get_volume_index(),
                     1+get_frequency_index());
}


}




