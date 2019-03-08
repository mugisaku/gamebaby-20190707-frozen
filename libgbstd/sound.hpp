#ifndef gbstd_sound_hpp_is_included
#define gbstd_sound_hpp_is_included


#include"libgbstd/utility.hpp"


namespace gbstd{




using f32_t = float;
using sample_t = f32_t;

constexpr f32_t  g_number_of_samples_per_second = 24000;//1秒あたりのサンプル数
constexpr f32_t  g_number_of_samples_per_millisecond = g_number_of_samples_per_second/1000;//1ミリ秒あたりのサンプル数

inline
constexpr uint32_t
get_number_of_samples_by_time(uint32_t  ms) noexcept
{
  return g_number_of_samples_per_millisecond*ms;
}


//vm is Volume    Modulation音量変調
//fm is Frequency Modulation周波数変調
class
sound_device
{
protected:
  using callback = void(*)(sound_device&  dev, void*  data);

  f32_t  m_frequency=0;

  f32_t  m_number_of_samples_per_cycle=0;//1周波あたりのサンプル数


  struct flags{
    static constexpr int              slept = 1;
    static constexpr int              muted = 2;
    static constexpr int  frequency_changed = 4;
    static constexpr int     volume_changed = 8;
  };


  status_value<int>  m_status;

  uint32_t  m_number_of_remain_samples_for_timer=0;

  void  process_timer() noexcept;

  void*  m_timer_callback_data=nullptr;

  callback  m_timer_callback=nullptr;


  f32_t  m_fm_increment=0;

  uint32_t  m_number_of_samples_per_fm_step;
  uint32_t  m_number_of_remain_samples_for_fm_step;
  uint32_t  m_number_of_fm_steps;

  void  process_fm() noexcept;

  void*  m_fm_callback_data=nullptr;

  callback  m_fm_callback=nullptr;


  sample_t  m_volume=0;

  sample_t  m_vm_increment=0;

  uint32_t  m_number_of_samples_per_vm_step;
  uint32_t  m_number_of_remain_samples_for_vm_step;
  uint32_t  m_number_of_vm_steps;

  void  process_vm() noexcept;

  void*  m_vm_callback_data=nullptr;

  callback  m_vm_callback=nullptr;


  void  put(sample_t  src, sample_t&  dst) noexcept;

public:
  void  reset() noexcept;

  bool  is_slept() const noexcept{return m_status.test( flags::slept);}
  void     sleep()       noexcept{return m_status.set(  flags::slept);}
  void      wake()       noexcept{return m_status.unset(flags::slept);}

  bool  is_muted() const noexcept{return m_status.test( flags::muted);}
  void      mute()       noexcept{return m_status.set(  flags::muted);}
  void    unmute()       noexcept{return m_status.unset(flags::muted);}

  void  set_timer(uint32_t  ms) noexcept{m_number_of_remain_samples_for_timer = get_number_of_samples_by_time(ms);}

  void  set_absolute_fm(f32_t  target_freq, int  num_steps, uint32_t  ms) noexcept;
  void  set_relative_fm(f32_t         freq, int  num_steps, uint32_t  ms) noexcept;

  void  set_absolute_vm(sample_t  target_vol, int  num_steps, uint32_t  ms) noexcept;
  void  set_relative_vm(sample_t         vol, int  num_steps, uint32_t  ms) noexcept;

  void  set_timer_callback(void(*callback)(sound_device&,void*), void*  data=nullptr) noexcept;
  void     set_fm_callback(void(*callback)(sound_device&,void*), void*  data=nullptr) noexcept;
  void     set_vm_callback(void(*callback)(sound_device&,void*), void*  data=nullptr) noexcept;

  void   set_frequency(f32_t  freq)       noexcept{       m_frequency = freq;}
  f32_t  get_frequency(           ) const noexcept{return m_frequency       ;}

  f32_t  get_number_of_samples_per_cycle() const noexcept{return m_number_of_samples_per_cycle;}

  void      set_volume(sample_t  v)       noexcept{       m_volume  = v;}
  void      add_volume(sample_t  v)       noexcept{       m_volume += v;}
  sample_t  get_volume(           ) const noexcept{return m_volume     ;}

};


class
square_wave_device: public sound_device
{
protected:
  uint32_t  m_number_of_samples_per_high=0;
  uint32_t  m_number_of_samples_per_low=0;

  uint32_t  m_number_of_remain_samples;

  bool  m_low_phase=false;

  void  check_frequency() noexcept;

  void  update_parameters() noexcept;

public:
  void  reset() noexcept;

  void  generate_for_time(uint32_t  milsec, sample_t*  buffer) noexcept;
  void  generate_for_number_of_samples(uint32_t  n, sample_t*  buffer) noexcept;

};


class
noise_device: public sound_device
{
  uint16_t  m_seed=0xFFFF;

  uint16_t  m_result=1;

  bool  m_shortspan_flag=false;

public:
  bool   test_shortspan_flag() const noexcept{return m_shortspan_flag;}
  void    set_shortspan_flag() noexcept{m_shortspan_flag =  true;}
  void  unset_shortspan_flag() noexcept{m_shortspan_flag = false;}

  void  reset() noexcept;

  void  generate_for_time(uint32_t  milsec, sample_t*  buffer) noexcept;
  void  generate_for_number_of_samples(uint32_t  n, sample_t*  buffer) noexcept;

};



}




#endif




