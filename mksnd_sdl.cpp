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


namespace{



gbstd::wave
make_wave(const onch_element&  e, const onch_space&  sp) noexcept
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

  return gbstd::wave(wavbin.data(),2*wavbin.size(),fmt);
}


#ifdef __EMSCRIPTEN__
void
main_loop() noexcept
{
  sdl::update_control();

    if(gbstd::g_dropped_file.size())
    {
      gbstd::g_dropped_file.emplace_back(0);

      onch_space  sp;

      auto  p = reinterpret_cast<const char*>(gbstd::g_dropped_file.data());

      sp.load_from_string(p);

      auto  def = sp.find("main");

        if(def && def->get_element())
        {
          auto&  e = def->get_element();

          auto  wav = make_wave(e,sp);

          auto  bin = wav.to_binary();

          gbstd::download(bin.data(),bin.size(),"new.wav");
        }


      gbstd::g_dropped_file.clear();
    }
}
#endif
}




int
main(int  argc, char**  argv)
{
#ifdef __EMSCRIPTEN__
  set_description("<pre>"
                  "</pre>");

  show_github_link();

EM_ASM(
  var  free = document.getElementById('free');

  var  button = document.createElement('button');

  button.innerText = 'compile';
  button.onclick = function()
  {
    var  src = document.getElementById('source');

    var  arr = new Uint8Array(src.value.length);

    var  dst_i = 0;

      for(var  src_i = 0;  src_i < src.value.length;  ++src_i)
      {
        var  c = src.value.charCodeAt(src_i);

          if(c <= 0x7F)
          {
            arr[dst_i++] = c;
          }
      }


    g_dropped_file_list.push(arr);
  };


  var  textarea = document.createElement('textarea');

  textarea.id = 'source';
  textarea.cols = 60;
  textarea.rows = 24;
  textarea.value = 'txt = text{l5v4f1 l5v4f2 l5v4f1 l5v4f2 l5v4f1 l5v4f2}\n sq = square{txt}\n main = column{sq}';

  free.appendChild(textarea);
  free.appendChild(button);
);


  emscripten_set_main_loop(main_loop,0,false);
#else
report;
  --argc;
  ++argv;

  int  n = 0;

    while(argc--)
    {
      onch_space  sp;

      auto  path = *argv++;

      sp.load_from_file(path);

      auto  def = sp.find("main");

        if(def && def->get_element())
        {
          auto&  e = def->get_element();

          std::string  s(path);

          s += ".wav";

          auto  wav = make_wave(e,sp);

          wav.save_to_file(s.data());

          ++n;

          printf("%s -> %s\n",path,s.data());
        }
    }


  printf("%d files were processed\n",n);
#endif


  return 0;
}




