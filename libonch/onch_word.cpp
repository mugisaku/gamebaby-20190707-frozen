#include"libonch/onch.hpp"
#include"libgbstd/sound.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




uint32_t
onch_word::
get_length(const onch_space&  sp) const noexcept
{
  return sp.get_parameter().m_length_table[get_length_index()];
}


void
onch_word::
output(sound_kind  k, onch_output_context&  ctx) const noexcept
{
  auto  length = get_length(*ctx.m_space);

  auto  num_samples = gbstd::get_number_of_samples_by_time(length);

  auto&  pr = ctx.m_space->get_parameter();

  auto  v = pr.m_volume_table[get_volume_index()];
  auto  f = pr.m_frequency_table[get_frequency_index()];


  gbstd::sound_event  evt(length,v,f);

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
  printf("l%dv%df%d",get_length_index(),get_volume_index(),get_frequency_index());
}


}




