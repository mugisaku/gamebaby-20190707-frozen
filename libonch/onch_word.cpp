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
  return ctx.get_length(get_l_spec(),get_l_value());
}


template<typename  T>
void
do_mix(const sound_instruction&  instr, f32_t*  ptr) noexcept
{
  T  tmp(instr);

  tmp.mix(ptr);
}


void
onch_word::
output(sound_kind  k, onch_output_context&  ctx) const noexcept
{
  auto  l = get_output_length(ctx);

    if(!test_rest_flag())
    {
      gbstd::sound_instruction  instr;

      auto  v0 = ctx.get_volume(get_v0_spec(),get_v0_value());
      auto  v1 = ctx.get_volume(get_v1_spec(),get_v1_value());
      auto  f0 = ctx.get_volume(get_f0_spec(),get_f0_value());
      auto  f1 = ctx.get_volume(get_f1_spec(),get_f1_value());

      instr.set_length(l)
           .set_start_volume(v0)
           .set_end_volume(v1)
           .set_start_frequency(f0)
           .set_end_frequency(f1);

        switch(k)
        {
      case(sound_kind::square_wave):
          do_mix<square_wave_device>(instr,ctx.m_it);
          break;
      case(sound_kind::noise):
          do_mix<gbstd::noise_device>(instr,ctx.m_it);
          break;
      case(sound_kind::short_noise):
          do_mix<gbstd::short_noise_device>(instr,ctx.m_it);
          break;
        }
    }


  ctx.m_it += gbstd::get_number_of_samples_by_time(l);
}


void
onch_word::
print() const noexcept
{
    if(test_rest_flag())
    {
      printf("r");

        if(get_l_spec() == specs::index){printf("%d",1+get_l_value());}
      else                              {printf("?");}
    }

  else
    {
      printf("p");

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
    }
}


}




