#include"libge/ge.hpp"
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


constexpr int  g_cell_size = 48;


ge::context*
g_ge_context;


namespace animator{


int
index;


dial*
interval_dial;


label*
state_label;


uint32_t
last_time;


void
update_state_label() noexcept
{
    if(g_ge_context->m_animation_points.empty())
    {
      state_label->set_text("  / 0");
    }

  else
    {
      string_form  sf;

      state_label->set_text(sf("%2d/%2d",index+1,g_ge_context->m_animation_points.size()));
    }
}


class
view: public widget
{
public:
  view() noexcept: widget(g_cell_size,g_cell_size){}

  void  advance() noexcept
  {
      if(++index >= g_ge_context->m_animation_points.size())
      {
        index = 0;
      }


    update_state_label();

    request_redraw();
  }


  void  render(const canvas&  cv) noexcept override
  {
    g_ge_context->m_core->render_underlay(1,cv);

      if(index < g_ge_context->m_animation_points.size())
      {
        auto  rect = g_ge_context->get_rect(g_ge_context->m_animation_points[index]);

        canvas  src_cv(g_ge_context->m_source_image,rect);

        cv.draw_canvas(src_cv,0,0);
      }
  }

};


animator::view
view;




void
check_time(uint32_t  now) noexcept
{
    if(g_ge_context->m_animation_points.size())
    {
        if(now >= (last_time+g_ge_context->m_animation_delay))
        {
          last_time = now;

          view.advance();
        }
    }
}


}




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


  animator::check_time(g_time);

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


widget*
create_animation_widget() noexcept
{
  animator::interval_dial = new dial(1,4,[](dial_event  evt){
    static const uint32_t  table[] = {1000,600,200,80};

    g_ge_context->m_animation_delay = table[evt.get_new_value()-1];
  });

  auto  psh_btn = new button(new label(u"Push",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
//        g_ge_context->m_animation_points.emplace_back(g_ge_context->m_current_cursor.);

        animator::view.request_redraw();
      }
  });

  auto  pop_btn = new button(new label(u"Pop",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
          if(g_ge_context->m_animation_points.size())
          {
            g_ge_context->m_animation_points.pop_back();

            animator::update_state_label();

            animator::view.request_redraw();
          }
      }
  });


  auto  op_col = make_column({psh_btn,pop_btn});

  animator::state_label = new label(u"  / 0",colors::black);

  auto  speed_frame = new frame("speed",animator::interval_dial);

  auto  frm_col = make_column({&animator::view,animator::state_label});

  auto  urow = make_row({frm_col,op_col});

  return new frame("animation",make_column({urow,speed_frame}));
}


}


int
main(int  argc, char**  argv)
{
#ifdef EMSCRIPTEN
  set_caption("mkanigra - " __DATE__);
  set_description("<pre>"
                  "*マウスの左ボタンで、任意色を置き、\n"
                  " 右ボタンで透明色を置く\n"
                  "*画像の編集単位をセルと呼ぶ\n"
                  "*PNGファイルをドラッグ・アンド・ドロップで読み込む\n"
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
                  "◇animation\n"
                  "  *pushはアニメーションの最後尾に現在のセルを付け加える\n"
                  "  *popはアニメーションの最後尾から、ひとつ取り除く\n"
                  "◇underlay\n"
                  "  *pushは下敷きの最上段に現在のセルを乗せる\n"
                  "  *popは下敷きの最上段から、ひとつ取り除く\n"
                  "  *show/hideは下敷きの有効/無効を切り替える\n"
                  "</pre>"
  );
#endif

  g_ge_context = ge::create_context(g_cell_size,g_cell_size,6,2);

  auto  ani = create_animation_widget();
  

  g_ge_context->m_callback = [](){animator::view.request_redraw();};

  auto  coloring_widget = make_column({g_ge_context->m_color_maker_frame,g_ge_context->m_bg_change_buttons,ani});

  auto  to_col = make_column({g_ge_context->m_tool_widget_frame,g_ge_context->m_operation_widget_frame});

  auto  last_w = make_row({g_ge_context->m_cell_table_frame,g_ge_context->m_underlay.m_frame});

  auto  right_upper = make_row({g_ge_context->m_color_holder_frame,coloring_widget,to_col});
  auto  right       = make_column({right_upper,last_w});


  auto  left = make_column({g_ge_context->m_core_frame,g_ge_context->m_png_save_button,g_ge_context->m_apng_save_button});

  g_window.set_root_widget(make_row({left,right}));

  sdl::init(g_window.get_width(),g_window.get_height());

  g_canvas = sdl::make_screen_canvas();


#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }


  sdl::quit();
#endif


  return 0;
}




