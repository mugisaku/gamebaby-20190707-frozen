#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/parser.hpp"
#include"libgbstd/file_op.hpp"
#include"libonch/onch.hpp"
#include"sdl.hpp"
#include<list>
#include<vector>




#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


gbstd::canvas
g_screen_canvas;


namespace{


void
main_loop() noexcept
{
  constexpr int  delay = 80;

  static uint32_t  next;

  sdl::update_control();

    if(gbstd::g_time >= next)
    {
      g_screen_canvas.fill(color());

      sdl::update_screen(g_screen_canvas);

      next = gbstd::g_time+delay;
    }
}


void
output(const onch_element&  e, const onch_space&  sp) noexcept
{
  auto  bin = e.generate_wave(sp);

  std::vector<int16_t>  wavbin(bin.size());

  auto  src     =    bin.data();
  auto  src_end =    bin.data()+bin.size();
  auto  dst     = wavbin.data();

    while(src != src_end)
    {
      auto  v = (*src++)*32767.0;

      *dst++ = static_cast<int16_t>(v);
    }


  gbstd::wave_format  fmt;

  fmt.set_sampling_rate(gbstd::g_number_of_samples_per_second);
  fmt.set_number_of_bits_per_sample(16);
  fmt.set_number_of_channels(1);

  fmt.update();

  gbstd::wave  wav(wavbin.data(),2*wavbin.size(),fmt);

  wav.save_to_file("../__output.wav");
}


}




int
main(int  argc, char**  argv)
{
#ifdef __EMSCRIPTEN__
  set_description("<pre>"
                  "</pre>");

  show_github_link();
#endif

/*
  sdl::init(200,100,1.0);
  sdl::init_sound();

  g_screen_canvas = sdl::make_screen_canvas();
*/

  onch_parameter  pr = {
    {100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,},
    {0.01,0.02,0.04,0.08,0.10,0.15,0.20,0.25,0.30,0.35,0.40,0.45,0.50,0.55,0.60,0.65},
    {20.0,40.0,80.0,160.0,320.0,400.0,600.0,800.0,1000.0,1200.0,1400.0,1600.0,1800.0,2000.0,2400.0,2800.0}
  };


  onch_space  sp(pr);

  sp.load_from_file("../music.txt");

  auto  def = sp.find("main");

    if(def && def->get_element())
    {
      auto&  e = def->get_element();

      e.print();

      output(e,sp);
    }


/*
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }


  sdl::quit_sound();
  sdl::quit();
#endif
*/


  return 0;
}




