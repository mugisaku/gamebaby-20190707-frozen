#ifndef gbstd_sound_hpp_is_included
#define gbstd_sound_hpp_is_included


#include"libgbstd/utility.hpp"


namespace gbstd{




using f32_t = float;
using sample_t = f32_t;

constexpr f32_t  g_number_of_samples_per_second = 48000;//1秒あたりのサンプル数
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




enum class
sound_kind
{
  null,
  square_wave,
  triangle_wave,
  sawtooth_wave,
  noise,
  short_noise,

};


class
sound_instruction
{
  uint32_t  m_length;

  sample_t  m_start_volume;
  sample_t  m_end_volume;

  f32_t  m_start_frequency;
  f32_t  m_end_frequency;

public:
  sound_instruction() noexcept{}

  sound_instruction&  set_length(uint32_t  l) noexcept{  m_length    = l;  return *this;}      

  sound_instruction&  set_start_volume(sample_t  v) noexcept{  m_start_volume    = v;  return *this;}      
  sound_instruction&  set_end_volume(  sample_t  v) noexcept{  m_end_volume      = v;  return *this;}      
  sound_instruction&  set_start_frequency(f32_t  f) noexcept{  m_start_frequency = f;  return *this;}      
  sound_instruction&  set_end_frequency(  f32_t  f) noexcept{  m_end_frequency   = f;  return *this;}      

  uint32_t  get_length() const noexcept{return m_length;}
  f32_t  get_start_frequency() const noexcept{return m_start_frequency;}
  f32_t  get_end_frequency() const noexcept{return m_end_frequency;}
  sample_t  get_start_volume() const noexcept{return m_start_volume;}
  sample_t  get_end_volume() const noexcept{return m_end_volume;}

};




//vm is Volume    Modulation音量変調
//fm is Frequency Modulation周波数変調
class
sound_device
{
  bool  m_downward_flag=false;

  uint32_t  m_length=0;

  f32_t  m_fm_current  =0;
  f32_t  m_fm_increment=0;

  f32_t  m_vm_current  =0;
  f32_t  m_vm_increment=0;

  f32_t  m_number_of_remain_samples=0;

  f32_t  m_number_of_upward_samples  =0;
  f32_t  m_number_of_downward_samples=0;

protected:
  virtual f32_t  get_sample() noexcept{return is_downward()? -get_volume():get_volume();}

  void  set_number_of_upward_samples(  f32_t  v) noexcept{m_number_of_upward_samples   = v;}
  void  set_number_of_downward_samples(f32_t  v) noexcept{m_number_of_downward_samples = v;}

  virtual void  update() noexcept=0;

  bool  is_downward() const noexcept{return m_downward_flag;}

  f32_t  get_volume()    const noexcept{return m_vm_current;}
  f32_t  get_frequency() const noexcept{return m_fm_current;}

public:
  sound_device(                               ) noexcept{}
  sound_device(const sound_instruction&  instr) noexcept;

  uint32_t  get_length() const noexcept{return m_length;}
  uint32_t  get_number_of_samples() const noexcept{return get_number_of_samples_by_time(m_length);}

  void  mix(f32_t*  ptr) noexcept;

};


class
square_wave_device: public sound_device
{
  void  update() noexcept override;

public:
  using sound_device::sound_device;

};


class
triangle_wave_device: public sound_device
{
protected:
  f32_t  m_sample_current;
  f32_t  m_sample_increment;

  f32_t  get_sample() noexcept override;

  void  update() noexcept override;

public:
  using sound_device::sound_device;

};


class
sawtooth_wave_device: public triangle_wave_device
{
  f32_t  get_sample() noexcept override;

  void  update() noexcept override;

public:
  using triangle_wave_device::triangle_wave_device;

};


class
noise_device: public sound_device
{
protected:
  uint16_t  m_seed=0xFFFF;

  virtual void  update_seed() noexcept;

  void  update() noexcept override;

  f32_t  get_sample() noexcept override{return static_cast<int16_t>(m_seed)/32767.0*get_volume();}

public:
  using sound_device::sound_device;

};


class
short_noise_device: public noise_device
{
  void  update_seed() noexcept override;

public:
  using noise_device::noise_device;

};



}




#endif




