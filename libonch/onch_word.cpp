#include"libonch/onch.hpp"
#include"libgbstd/sound.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




onch_word&
onch_word::
set_l(int  lspec, int  l) noexcept
{
  m_data |= (( lspec<<3)|l )<<m_l_shift_amount;

  return *this;
}


onch_word&
onch_word::
set_b(int  bfspec, int  bf) noexcept
{
  m_data |= ((bfspec<<3)|bf)<<m_bf_shift_amount;

  return *this;
}


onch_word&
onch_word::
set_v(int  v0spec, int  v0, int  v1spec, int  v1) noexcept
{
  m_data |= ((v0spec<<3)|v0)<<m_v0_shift_amount;
  m_data |= ((v1spec<<3)|v1)<<m_v1_shift_amount;

  return *this;
}


onch_word&
onch_word::
set_f(int  f0spec, int  f0, int  f1spec, int  f1) noexcept
{
  m_data |= ((f0spec<<3)|f0)<<m_f0_shift_amount;
  m_data |= ((f1spec<<3)|f1)<<m_f1_shift_amount;

  return *this;
}




uint32_t
onch_word::
get_output_length(onch_output_context&  ctx) const noexcept
{
  return is_play()? ctx.get_play_length(get_l_spec(),get_l_value())
        :is_rest()? ctx.get_rest_length(get_l_spec(),get_l_value())
        :0;
}


template<typename  T>
void
do_mix(uint32_t  rate, const sound_instruction&  instr, f32_t*  ptr) noexcept
{
  T  tmp(rate);

  tmp.mix(instr,ptr);
}


void
do_mix(gbstd::sound_kind  k, uint32_t  rate, const sound_instruction&  instr, f32_t*  ptr) noexcept
{
    switch(k)
    {
  case(sound_kind::sine_wave):
      do_mix<sine_wave_device>(rate,instr,ptr);
      break;
  case(sound_kind::square_wave):
      do_mix<square_wave_device>(rate,instr,ptr);
      break;
  case(sound_kind::triangle_wave):
      do_mix<triangle_wave_device>(rate,instr,ptr);
      break;
  case(sound_kind::sawtooth_wave):
      do_mix<sawtooth_wave_device>(rate,instr,ptr);
      break;
  case(sound_kind::noise):
      do_mix<gbstd::noise_device>(rate,instr,ptr);
      break;
  case(sound_kind::short_noise):
      do_mix<gbstd::short_noise_device>(rate,instr,ptr);
      break;
    }
}


void
onch_word::
output(sound_kind  k, onch_output_context&  ctx) const noexcept
{
  auto  l = get_output_length(ctx);

  auto  num_samples = gbstd::sound_device::get_number_of_samples(ctx.m_sampling_rate,l);

  auto  v0 = ctx.get_volume(get_v0_spec(),get_v0_value());
  auto  v1 = ctx.get_volume(get_v1_spec(),get_v1_value());
  auto  f0 = ctx.get_frequency(get_f0_spec(),get_f0_value());
  auto  f1 = ctx.get_frequency(get_f1_spec(),get_f1_value());
  auto  bf = ctx.get_vibrato_frequency(get_bf_spec(),get_bf_value());

    if(is_play() && num_samples)
    {
      std::vector<f32_t>  buf(num_samples);

      gbstd::sound_instruction  instr;

      instr.set_length(l)
           .set_start_volume(v0)
           .set_end_volume(v1)
           .set_start_frequency(f0)
           .set_end_frequency(f1);

      do_mix(k,ctx.m_sampling_rate,instr,buf.data());

        if(bf == 0)
        {
            for(int  i = 0;  i < num_samples;  ++i)
            {
              ctx.m_it[i] += buf[i];
            }
        }

      else
        {
          std::vector<f32_t>  subbuf(num_samples);

          instr.set_length(l)
               .set_start_volume(0.4)
               .set_end_volume(  0.4)
               .set_start_frequency(bf)
               .set_end_frequency(  bf);

          do_mix(gbstd::sound_kind::sine_wave,ctx.m_sampling_rate,instr,subbuf.data());

            for(int  i = 0;  i < num_samples;  ++i)
            {
              auto  rate = (1.0+subbuf[i]);

              ctx.m_it[i] += buf[i]*rate;
            }
        }
    }


  ctx.m_it += num_samples;
}


void
onch_word::
print() const noexcept
{
  printf("word{");

    if(is_rest())
    {
      printf("r");

        if(get_l_spec() == specs::index){printf("%d",1+get_l_value());}
      else                              {printf("?");}
    }

  else
    {
        if(is_play())
        {
          printf("p");
        }


        if(get_l_spec() == specs::index){printf("%d",1+get_l_value());}
      else                              {printf("?");}

      auto  v0spe = get_v0_spec();
      auto  v0val = get_v0_value();
      auto  v1spe = get_v1_spec();
      auto  v1val = get_v1_value();

      auto  f0spe = get_f0_spec();
      auto  f0val = get_f0_value();
      auto  f1spe = get_f1_spec();
      auto  f1val = get_f1_value();

      auto  bfspe = get_bf_spec();
      auto  bfval = get_bf_value();

      printf("-v");

           if(v0spe == specs::zero){printf("0");}
      else if(v0spe == specs::index){printf("%d",1+v0val);}
      else if(v0spe == specs::no_spec){printf("?");}

           if(v1spe == specs::zero){printf("0");}
      else if(v1spe == specs::index){printf("%d",1+v1val);}
      else if(v1spe == specs::no_spec){printf("?");}


      printf("-f");

           if(f0spe == specs::zero){printf("0");}
      else if(f0spe == specs::index){printf("%d",1+f0val);}
      else if(f0spe == specs::no_spec){printf("?");}

           if(f1spe == specs::zero){printf("0");}
      else if(f1spe == specs::index){printf("%d",1+f1val);}
      else if(f1spe == specs::no_spec){printf("?");}


      printf("-b");

           if(bfspe == specs::zero){printf("0");}
      else if(bfspe == specs::index){printf("%d",1+bfval);}
      else if(bfspe == specs::no_spec){printf("?");}
    }


  printf("}");
}


}




