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



int  g_sampling_rate = 24000;

int  g_bps = 8;


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

      g_wave_bin = g_space.make_wave_format_binary(g_sampling_rate,g_bps);
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
             if(g_object.audio)
             {
               g_object.audio.pause();
             }


          g_object.audio = new Audio();

          g_object.audio.src = URL.createObjectURL(g_object.common_blob);

          g_object.audio.play();
        );
        }

      else
        if(o == 's')
        {
        EM_ASM(
             if(g_object.audio)
             {
               g_object.audio.pause();
             }
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
  var  stop_button = document.createElement('button');
  var  save_button = document.createElement('button');

  play_button.innerText = 'play';
  play_button.onclick = function(){g_object.transfer('p'.charCodeAt(0));};

  stop_button.innerText = 'stop';
  stop_button.onclick = function(){g_object.transfer('s'.charCodeAt(0));};

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
  +"//'p5-v55-f?7'や\"r3\"といった記述が演奏指示の最小単位でword要素と呼びます"+"\n"
  +"//pは演奏を表し、後ろに続く数字は演奏時間を表します"+"\n"
  +"//rは休止を表し、後ろに続く数字は休止時間を表します"+"\n"
  +"//vは音量を表し、後ろに続く、ふたつの数字は、開始時と終了時の音量を表します"+"\n"
  +"//fは音高を表し、後ろに続く、ふたつの数字は、開始時と終了時の音高を表します"+"\n"
  +"//間にハイフンがありますが、なくても問題ありません。無関係な文字は読み飛ばされます"+"\n"
  +"//数字は、音量は0～8、それ以外は、1～8を指定します"+"\n"
  +"//数字は省略することができ、その場合はp,r,v,fのそれぞれ最後に指定された数が"+"\n"
  +"//再利用されます"+"\n"
  +"//v,fの一番目の数字を省略し、2番目の数字を指定したい場合は"+"\n"
  +"//数字の代わりに、?を置きます"+"\n"
  +""+"\n"
  +"//word要素の列をtext要素と呼びます"+"\n"
  +"//下記の構文は、txt1と言う名前とtext要素とを結びつけます"+"\n"
  +"txt1 = text{'p5-v07-f?7' 'p5-v70-f57' 'p5-v12-f47' 'p5-v34-f77'}"+"\n"
  +""+"\n"
  +"//下記のように、名前と結びつけた要素は再利用することができ"+"\n"
  +"//繰り返しを表すのに便利です"+"\n"
  +"txt2 = text{'p5-v55-f67' txt1 'r5' txt1}"+"\n"
  +""+"\n"
  +"//square,triangle,sawtooth,noise,short_noiseは、cell要素と呼びます"+"\n"
  +"//text要素を内容とし、その音色を指示します"+"\n"
  +"//squareは、矩形波です"+"\n"
  +"//triangleは、三角波です"+"\n"
  +"//sawtoothは、ノコギリ波です"+"\n"
  +"//noiseは、ノイズです"+"\n"
  +"//short_noiseは、短周期ノイズです"+"\n"
  +"//"+"\n"
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
  free.appendChild(stop_button);
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

        if(*path == '-')
        {
            if(sscanf(path,"-sps:%d",&g_sampling_rate) == 1)
            {
              printf("sampling rate was changed to %d\n",g_sampling_rate);
            }

          else
            if(sscanf(path,"-bps:%d",&g_bps) == 1)
            {
              printf("bits per sample was changed to %d\n",g_bps);
            }
        }

      else
        {
          sp.load_from_file(path);

          auto  wave_bin = sp.make_wave_format_binary(g_sampling_rate,g_bps);

            if(wave_bin.size())
            {
              std::string  s(path);

              s += ".wav";

              gbstd::write_to_file(wave_bin.data(),wave_bin.size(),s.data());

              ++n;

              printf("%s -> %s\n",path,s.data());
            }
        }
    }


  printf("%d files were processed\n",n);
#endif


  return 0;
}




