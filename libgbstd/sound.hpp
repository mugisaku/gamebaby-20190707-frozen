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
  return static_cast<uint32_t>(g_number_of_samples_per_millisecond*ms);
}


class invalid
{const char*  what() const noexcept{return "invalid";}};

class invalid_frequency
{const char*  what() const noexcept{return "invalid frequency";}};

class invalid_volume
{const char*  what() const noexcept{return "invalid volume";}};

class invalid_number_of_samples_per_cycle
{const char*  what() const noexcept{return "invalid number of samples per cycle";}};

class invalid_number_of_steps
{const char*  what() const noexcept{return "invalid number of steps";}};




struct
sound_event
{
  uint32_t  m_length;

  f32_t  m_start_frequency;
  f32_t    m_end_frequency;

  uint32_t  m_number_of_fm_steps;

  sample_t  m_start_volume;
  sample_t    m_end_volume;

  uint32_t  m_number_of_vm_steps;

public:
  sound_event() noexcept{}
  sound_event(f32_t  f, sample_t  v, uint32_t  l) noexcept{assign(f,v,l);}

  sound_event&  assign(f32_t  f, sample_t  v, uint32_t  l) noexcept
  {
    m_length = l;

    m_start_frequency = f;
    m_end_frequency   = f;
    m_start_volume    = v;
    m_end_volume      = v;

    m_number_of_fm_steps = 1;
    m_number_of_vm_steps = 1;

    return *this;
  }

  uint32_t  get_length() const noexcept{return m_length;}

  f32_t  get_start_frequency() const noexcept{return m_start_frequency;}
  f32_t    get_end_frequency() const noexcept{return   m_end_frequency;}

  sample_t  get_start_volume() const noexcept{return m_start_volume;}
  sample_t    get_end_volume() const noexcept{return   m_end_volume;}

  uint32_t  get_number_of_vm_steps() const noexcept{return m_number_of_vm_steps;}
  uint32_t  get_number_of_fm_steps() const noexcept{return m_number_of_fm_steps;}

};




struct
modulation_status
{
  f32_t  m_current=0;
  f32_t  m_increment=0;

  uint32_t  m_initial_number_of_remain_samples=0;
  uint32_t  m_number_of_remain_samples=0;
  uint32_t  m_number_of_steps=0;

  bool  m_changed=false;

  void  clear() noexcept;

  void  step();

  void  change_current(f32_t  v);

  void  set(f32_t  start, f32_t  target, uint32_t  num_steps, uint32_t  ms);

};


//vm is Volume    Modulation音量変調
//fm is Frequency Modulation周波数変調
class
sound_device
{
protected:
  using callback = void(*)(sound_device&  dev, void*  data);

  struct flags{
    static constexpr int  slept = 1;
    static constexpr int  muted = 2;
  };


  status_value<int>  m_status;

  uint32_t  m_number_of_remain_samples_for_timer=0;

  void  process_timer() noexcept;


  modulation_status  m_fm_status;
  modulation_status  m_vm_status;


  std::vector<sound_event>  m_event_queue;

  int  m_event_index;

  void  pump_event() noexcept;

  void  put(sample_t  src, sample_t&  dst);

  virtual void  on_frequency_changed(){}

  void  check_frequency();

public:
  sound_device() noexcept{sleep();}

  void  input(const sound_event&  evt) noexcept;

  virtual void  reset() noexcept;

  bool  is_slept() const noexcept{return m_status.test( flags::slept);}
  void     sleep()       noexcept{m_status.set(  flags::slept);}
  void      wake()       noexcept{m_status.unset(flags::slept);}

  bool  is_muted() const noexcept{return m_status.test( flags::muted);}
  void      mute()       noexcept{m_status.set(  flags::muted);}
  void    unmute()       noexcept{m_status.unset(flags::muted);}

  f32_t  get_frequency() const noexcept{return m_fm_status.m_current;}
  f32_t     get_volume() const noexcept{return m_vm_status.m_current;}

  void  push(std::initializer_list<sound_event>  ls);

};


class
square_wave_device: public sound_device
{
protected:
  uint32_t  m_number_of_samples_per_high=0;
  uint32_t  m_number_of_samples_per_low =0;

  uint32_t  m_number_of_remain_samples=0;

  bool  m_low_phase=false;

  void  on_frequency_changed() override;

public:
  square_wave_device() noexcept{}
  square_wave_device(f32_t  freq, sample_t  vol){assign(freq,vol);}

  square_wave_device&  assign(f32_t  freq, sample_t  vol);

  void  reset() noexcept override;

  void  generate_for_number_of_samples(uint32_t  n, sample_t*  buffer);

};


class
noise_device: public sound_device
{
protected:
  uint16_t  m_seed=0xFFFF;

public:
  noise_device() noexcept{}
  noise_device(sample_t  vol){assign(vol);}

  noise_device&  assign(sample_t  vol);

  void  reset() noexcept;

  void  generate_for_number_of_samples(uint32_t  n, sample_t*  buffer);

};


class
short_noise_device: public noise_device
{
public:
  short_noise_device() noexcept{}
  short_noise_device(sample_t  vol){assign(vol);}

  void  generate_for_number_of_samples(uint32_t  n, sample_t*  buffer);

};



}




#endif




