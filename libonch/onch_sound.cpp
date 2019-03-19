#include"libonch/onch.hpp"
#include"libgbstd/sound.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




void
onch_sound::
output(f32_t*  it, f32_t*  end) const noexcept
{
  gbstd::sound_event  evt(m_frequency.get_value(),m_volume.get_value(),m_length.get_value());

  auto  num_samples = gbstd::get_number_of_samples_by_time(get_length());

    switch(m_kind)
    {
  case(sound_kind::null):
      break;
  case(sound_kind::square_wave):
      {
        gbstd::square_wave_device  sq;

        sq.input(evt);

        sq.generate_for_number_of_samples(num_samples,it);
      }
      break;
  case(sound_kind::noise):
      break;
  case(sound_kind::short_noise):
      break;
    }
}




void
onch_sound::
print() const noexcept
{
    switch(m_kind)
    {
  case(sound_kind::null       ): printf("null");break;
  case(sound_kind::square_wave): printf("sqaure wave");break;
  case(sound_kind::noise      ): printf("noise");break;
  case(sound_kind::short_noise): printf("short noise");break;
    }


  printf("{frequency: %f,",m_frequency.get_value());
  printf("volume: %f,",m_volume.get_value());
  printf("length: %ums}",m_length.get_value());
}




}




