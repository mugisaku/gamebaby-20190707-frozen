#include"libge/ge_context.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


canvas
g_canvas;


window
g_window;


constexpr int  g_cell_size = 12;


ge::context2*
g_ge_context;


void
main_loop() noexcept
{
  sdl::update_control();

  auto&  f = g_dropped_file;

    if(f.size())
    {
      g_ge_context->load(f);

      f.clear();
    }


  g_window.do_total_reform_if_necessary();

    for(auto&  pt: g_point_buffer)
    {
      g_window.process_user_input(pt);
    }


    if(g_window.redraw_if_necessary() || g_needed_to_redraw)
    {
      g_canvas.fill(color());

      canvas  cv(g_window.get_content_image());

      g_canvas.draw_canvas(cv,0,0);

      sdl::update_screen(g_canvas);
    }
}


}


int
main(int  argc, char**  argv)
{
#ifdef EMSCRIPTEN
  set_caption("mkbg - " __DATE__);
  set_description("<pre>"
                  "*マウスの左ボタンで、任意色を置き、\n"
                  " 右ボタンで透明色を置く\n"
                  "*画像の編集単位をセルと呼ぶ\n"
                  "*save parts as PNGで作成されたPNGファイルをドラッグ・アンド・ドロップで読み込む\n"
                  "◇tool\n"
                  "  *fill areaは指定地点と同色の領域を新たな色で塗りつぶす\n"
                  "  *selectはundoを除くoperationの有効範囲を変更する\n"
                  "   開始時は全体が範囲\n"
                  "  *select時、右クリックで範囲を全体に戻す\n"
                  "  *pasteは保持されたセルを透明色も含めて貼り付ける\n"
                  "  *layerは保持されたセルを透明色を除いて貼り付ける\n"
                  "◇operation\n"
                  "  *undoは最後の編集操作を取り消す。セルを移ると編集操作記録はクリアされる\n"
                  "  *copyは現在のセルから指定範囲を複製して保持する\n"
                  "◇save parts as PNG\n"
                  "  *各パーツを一枚のPNG形式で出力する\n"
                  "◇save result as PNG\n"
                  "  *previewにあるものをPNG形式で出力する\n"
                  "◇save result as C code\n"
                  "  *previewにあるものをPNG形式にし、C言語の配列リテラルとして出力する\n"
                  "</pre>"
  );


  show_github_link();
  show_twitter_link();

#endif


  g_ge_context = new ge::context2;


  auto  coloring_widget = make_column({g_ge_context->m_color_maker_frame,g_ge_context->m_bg_changer,g_ge_context->m_menu_frame});

  auto  to_col = make_column({g_ge_context->m_tool_widget_frame,g_ge_context->m_operation_widget_frame});

  auto  right = make_row({g_ge_context->m_color_holder_frame,coloring_widget,to_col});


  auto  left = make_column({g_ge_context->m_core_frame,
                            g_ge_context->m_preview_frame,
    g_ge_context->m_parts_save_button,
    g_ge_context->m_result_save_button,
    g_ge_context->m_txt_save_button,
  });


  g_window.set_root_widget(make_row({left,right}));

  sdl::init(g_window.get_width(),g_window.get_height());

  g_canvas = sdl::make_screen_canvas();


#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }


  sdl::quit();
#endif


  return 0;
}




