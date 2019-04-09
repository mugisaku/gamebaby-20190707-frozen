#include"sdl.hpp"
#include<SDL.h>
#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include"libgbstd/sound.hpp"
#include"libgbstd/file_op.hpp"


namespace sdl{


namespace{


bool  g_recording_flag;

std::vector<int16_t>  g_recording_buffer;


SDL_AudioSpec  g_spec;

SDL_AudioDeviceID  g_devid;

void
callback(void*  data, uint8_t*  buf, int  len) noexcept
{
  SDL_memset(buf,g_spec.silence,len);

  auto  dst = reinterpret_cast<float*>(buf);

  int  n = len/sizeof(*dst);
}


void
print() noexcept
{
  printf("freq %d\n",g_spec.freq);
  printf("ch %d\n",g_spec.channels);
  printf("smpl %d\n",g_spec.samples);

  const char*  s = "unknown";

    switch(g_spec.format)
    {
  case(AUDIO_U8    ): s = "U8";break;
  case(AUDIO_S8    ): s = "S8";break;
  case(AUDIO_S16LSB): s = "S16LSB";break;
  case(AUDIO_S16MSB): s = "S16MSB";break;
  case(AUDIO_U16LSB): s = "U16LSB";break;
  case(AUDIO_S32LSB): s = "S32LSB";break;
  case(AUDIO_S32MSB): s = "S32MSB";break;
  case(AUDIO_F32LSB): s = "F32LSB";break;
  case(AUDIO_F32MSB): s = "F32MSB";break;
    }


  printf("format %s\n",s);
}


}


void
init_sound() noexcept
{
    if(SDL_WasInit(SDL_INIT_AUDIO))
    {
      return;
    }


  SDL_InitSubSystem(SDL_INIT_AUDIO);

  SDL_AudioSpec  spec;

  spec.freq     = 24000;
  spec.channels = 1;
  spec.format   = AUDIO_F32;
  spec.samples  = 4096;
  spec.callback = callback;

  g_devid = SDL_OpenAudioDevice(nullptr,0,&spec,&g_spec,0);

    if(g_devid == 0)
    {
      printf("%s\n",SDL_GetError());

      report;
    }
}


void
quit_sound() noexcept
{
    if(SDL_WasInit(SDL_INIT_AUDIO))
    {
      SDL_CloseAudioDevice(g_devid);

      SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}




void
start_sound_recording() noexcept
{
  g_recording_flag = true;
}


void
clear_sound_recording() noexcept
{
  g_recording_buffer.clear();
}


void
stop_sound_recording() noexcept
{
  g_recording_flag = false;
}


bool
test_sound_recording() noexcept
{
  return g_recording_flag;
}


std::vector<uint8_t>
get_sound_wave_binary() noexcept
{
  gbstd::wave_format  fmt;

  fmt.set_number_of_channels(1);
  fmt.set_sampling_rate(24000);
  fmt.set_number_of_bits_per_sample(16);

  fmt.update();

  gbstd::wave  wav;

  bool  playing = (SDL_GetAudioDeviceStatus(g_devid) == SDL_AUDIO_PLAYING);

    if(playing)
    {
      stop_sound_playing();
    }


  wav.assign(g_recording_buffer.data(),g_recording_buffer.size(),fmt);

    if(playing)
    {
      start_sound_playing();
    }


  return wav.to_binary();
}




void
start_sound_playing() noexcept
{
  SDL_PauseAudioDevice(g_devid,0);
}


void
stop_sound_playing() noexcept
{
  SDL_PauseAudioDevice(g_devid,1);
}




}




