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



std::vector<uint8_t>
g_source_code;

std::vector<uint8_t>
g_wave_bin;

onch_space
g_space;


void
update_wave_binary(std::vector<uint8_t>&&  new_code) noexcept
{
    if(g_source_code != new_code)
    {
      g_source_code = std::move(new_code);

      g_space.clear();

      g_space.load_from_string(reinterpret_cast<const char*>(g_source_code.data()));

      g_wave_bin = g_space.make_wave_format_binary();
#ifdef __EMSCRIPTEN__
      gbstd::update_common_blob(g_wave_bin.data(),g_wave_bin.size());
#endif
    }
}




#ifdef __EMSCRIPTEN__
void
main_loop() noexcept
{
  sdl::update_control();

    if(gbstd::g_dropped_file.size())
    {
      auto&  back = gbstd::g_dropped_file.back();

      auto  o = back    ;
                back = 0;

        if(g_source_code != gbstd::g_dropped_file)
        {
          update_wave_binary(std::move(gbstd::g_dropped_file));
        }


        if(o == 'p')
        {
        EM_ASM(
          g_object.audio = new Audio();

          g_object.audio.src = URL.createObjectURL(g_object.common_blob);

          g_object.audio.play();
        );
        }

      else
        if(o == 's')
        {
          gbstd::download_common_blob("new.wav");
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
  show_twitter_link();

EM_ASM(
  g_object.transfer = function(o)
  {
    var  src = g_object.textarea.value;

    var  arr = new Uint8Array(src.length+1);

    var  dst_i = 0;

      for(var  src_i = 0;  src_i < src.length;  ++src_i)
      {
        var  c = src.charCodeAt(src_i);

        arr[dst_i++] = (c <= 0x7F)? c:' '.charCodeAt(0);
      }


    arr[dst_i++] = o;

    g_dropped_file_list.push(arr);
  };

  var  free = document.getElementById('free');

  var  play_button = document.createElement('button');
  var  save_button = document.createElement('button');

  play_button.innerText = 'play';
  play_button.onclick = function(){g_object.transfer('p'.charCodeAt(0));};

  save_button.innerText = 'save';
  save_button.onclick = function(){g_object.transfer('s'.charCodeAt(0));};


  var  textarea = document.createElement('textarea');

  textarea.id = 'source';
  textarea.cols = 100;
  textarea.rows =  28;
  textarea.value =
   "/*この間がコメント*/"+"\n"
  +"//行末までコメント"+"\n"
  +""+"\n"
  +"//lvfNNN というのが演奏指示の最小単位でword要素と呼びます"+"\n"
  +"//Nの部分に、1から8までの数字を指定します"+"\n"
  +"//lは演奏時間、vは音量、fは音の高低を意味し"+"\n"
  +"//lvf123と書くと、演奏時間1、音量2、音の高低3という指示になります"+"\n"
  +""+"\n"
  +"//word要素の列をtext要素と呼びます"+"\n"
  +"//下記の構文は、txt1と言う名前とtext要素とを結びつけます"+"\n"
  +"txt1 = text{lvf445 lvf443 lvf443 lvf444 lvf445}"+"\n"
  +""+"\n"
  +"//下記のように、名前と結びつけた要素は再利用することができ"+"\n"
  +"//繰り返しを表すのに便利です"+"\n"
  +"txt2 = text{lvf545 txt1 txt1}"+"\n"
  +""+"\n"
  +"//square,noise,short_noiseは、cell要素と呼びます"+"\n"
  +"//text要素を内容とし、その音色を指示します"+"\n"
  +"sq = square{txt2}"+"\n"
  +""+"\n"
  +"//column,rowは、table要素と呼びます"+"\n"
  +"//columnは内容を同時に演奏し、rowは内容を順番に演奏します"+"\n"
  +"//table要素の内容となるのは、table要素とcell要素です"+"\n"
  +"main = column{sq}"+"\n"
  +""+"\n"
  +"//mainという名前と結びついた要素が、最終結果として"+"\n"
  +"//WAVE形式で出力されます"+"\n";

  g_object.textarea = textarea;

  free.appendChild(textarea);
  free.appendChild(play_button);
  free.appendChild(save_button);
);


  emscripten_set_main_loop(main_loop,0,false);
#else
  --argc;
  ++argv;

  int  n = 0;

    while(argc--)
    {
      onch_space  sp;

      auto  path = *argv++;

      sp.load_from_file(path);

      auto  wave_bin = sp.make_wave_format_binary();

        if(wave_bin.size())
        {
          std::string  s(path);

          s += ".wav";

          gbstd::write_to_file(wave_bin.data(),wave_bin.size(),s.data());

          ++n;

          printf("%s -> %s\n",path,s.data());
        }
    }


  printf("%d files were processed\n",n);
#endif


  return 0;
}




