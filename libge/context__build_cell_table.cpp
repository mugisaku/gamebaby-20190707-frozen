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

    ctx.m_core->render_background(2,cv);
  }

  void  render_item(point  item_index, const canvas&  cv) noexcept override
  {
    auto&  ctx = *get_userdata<context>();

    int  w = ctx.m_cell_width ;
    int  h = ctx.m_cell_height;

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

      canvas  cv(ctx.m_source_image,w*pt.x,h*pt.y,w,h);

      ctx.m_core->set_canvas(cv);

        if(ctx.m_callback)
        {
          ctx.m_callback();
        }


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
  m_menu = new cell_menu(      item_size{ m_cell_width, m_cell_height},
                         item_table_size{m_table_width,m_table_height},
                          m_table_width,m_table_height,process);

  m_menu->set_userdata(this);


  m_table_offset_label = new label(u" 1/ 1",colors::black);

  auto  up_btn = new button(new iconshow({&icons::up}),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_press())
      {
        ctx.m_menu->move_up();

        ctx.update_table_offset_label();
      }
  });

  auto  down_btn = new button(new iconshow({&icons::down}),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_press())
      {
        ctx.m_menu->move_down();

        ctx.update_table_offset_label();
      }
  });

  auto  ext_btn = new button(new iconshow({&icons::plus}),[](button_event  evt){
    auto&  ctx = *evt->get_userdata<context>();

      if(evt.is_press())
      {
        ctx.m_source_image.resize(ctx.m_source_image.get_width(),
                                  ctx.m_source_image.get_height()+(ctx.m_cell_height*ctx.m_table_height));

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

  int  num = m_menu->get_item_cursor().get_base().y;
  int  den = m_menu->get_item_table_size().y_length;

  m_table_offset_label->set_text(sf("%2d/%2d",num+1,den));
}




}




