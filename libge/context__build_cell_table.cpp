#include"libge/ge.hpp"




namespace ge{


using namespace gbstd;


namespace{
class
cell_menu: public menu
{
public:
  using menu::menu;

  void  render_background(const canvas&  cv) noexcept override
  {
    auto&  ctx = *get_userdata<context>();

    ctx.m_core->get_display().render_background(cv,2);
  }

  void  render_item(point  item_index, const canvas&  cv) noexcept override
  {
    auto&  ctx = *get_userdata<context>();

    int  w = ctx.get_cell_width() ;
    int  h = ctx.get_cell_height();

    canvas  src_cv(ctx.m_source_image,w*item_index.x,h*item_index.y,
                                      w             ,h             );

    cv.draw_canvas(src_cv,0,0);

         if(ctx.m_current_index == item_index){cv.draw_rectangle(colors::white,0,0,w  ,h  );}
    else if(ctx.m_seeking_index == item_index){cv.draw_rectangle(colors::black,1,1,w-2,h-2);}
  }

};


void
process(menu_event  evt) noexcept
{
  auto&  ctx = *evt->get_userdata<context>();

    if(evt.is_press())
    {
      ctx.m_current_index = evt->get_item_cursor().get_result();

      auto&  pt = ctx.m_current_index;

      auto&  sz = evt->get_item_size();

      int  w = sz.width;
      int  h = sz.height;

      gbstd::canvas  cv(ctx.m_source_image,w*ctx.m_current_index.x,h*ctx.m_current_index.y,w,h);

      ctx.m_paint.reset(cv);
      ctx.m_core->set_canvas(cv);

      ctx.m_core->request_redraw();

      ctx.m_seamless_pattern_view->request_redraw();
      ctx.m_menu->request_redraw();
    }

  else
    if(evt.is_enter())
    {
      ctx.m_seeking_index = evt->get_item_cursor().get_result();

      ctx.m_menu->request_redraw();
    }
}
}


void
context::
build_cell_table() noexcept
{
  m_menu = new cell_menu(process);

  m_menu->set_userdata(this);


  m_table_offset_label = new label(u" 1/ 1",colors::white);

  auto  up_btn = new button(new iconshow({&icons::up}),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_release())
      {
        ctx.m_menu->move_base(0,-ctx.m_menu->get_number_of_rows());

        ctx.update_table_offset_label();
      }
  });

  auto  down_btn = new button(new iconshow({&icons::down}),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_release())
      {
        ctx.m_menu->move_base(0,ctx.m_menu->get_number_of_rows());

        ctx.update_table_offset_label();
      }
  });

  auto  ext_btn = new button(new iconshow({&icons::plus}),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_release())
      {
        item_table_size  sz{ctx.get_table_width(),ctx.get_table_height()+ctx.m_menu->get_number_of_rows()};

        ctx.resize_table(sz);

        ctx.update_table_offset_label();
      }
  });


  gbstd::set_userdata({up_btn,down_btn,ext_btn},this);

  auto     btn_col = make_column({up_btn,down_btn,ext_btn});
  auto  celtbl_col = make_column({m_menu,m_table_offset_label});

  m_cell_table_frame = new frame("cell table",make_row({btn_col,celtbl_col}));
}




void
context::
update_table_offset_label() noexcept
{
  string_form  sf;

  int  num = m_menu->get_item_cursor().get_base().y/m_menu->get_number_of_rows();
  int  den = m_menu->get_item_table_size().y_length/m_menu->get_number_of_rows();

  m_table_offset_label->set_text(sf("%2d/%2d",num+1,den));
}




}




