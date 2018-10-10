#include"libge/ge.hpp"
#include"libgbstd/windows/component.hpp"



using namespace gbstd;


widget*
core::
create_operation_widget() noexcept
{
  auto  undo_btn = new button(new label(u"Undo",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core>()->undo();
      }
  });


  auto  copy_btn = new button(new label(u"Copy",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core>()->take_copy();
      }
  });


  auto  rou_btn = new button(new label(u"Rotate ↑",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core>()->shift_up(true);
      }
  });


  auto  rol_btn = new button(new label(u"Rotate ←",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core>()->shift_left(true);
      }
  });


  auto  ror_btn = new button(new label(u"Rotate →",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core>()->shift_right(true);
      }
  });


  auto  rod_btn = new button(new label(u"Rotate ↓",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core>()->shift_down(true);
      }
  });




  auto  rvl_btn = new button(new label(u"Revolve →",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core>()->revolve();
      }
  });

  auto  rvh_btn = new button(new label(u"Reverse -",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core>()->reverse_horizontally();
      }
  });

  auto  rvv_btn = new button(new label(u"Reverse |",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core>()->reverse_vertically();
      }
  });

  auto  mir_btn = new button(new label(u"Mirror |",colors::black),[](button_event  evt){
      if(evt.is_release())
      {
        evt->get_userdata<core>()->mirror_vertically();
      }
  });




  rou_btn->set_userdata(this);
  rol_btn->set_userdata(this);
  ror_btn->set_userdata(this);
  rod_btn->set_userdata(this);
  undo_btn->set_userdata(this);
  copy_btn->set_userdata(this);
  rvl_btn->set_userdata(this);
  rvh_btn->set_userdata(this);
  rvv_btn->set_userdata(this);
  mir_btn->set_userdata(this);

  auto  ro_col = make_column({rou_btn,rol_btn,ror_btn,rod_btn});
  auto  ed_row = make_row(   {undo_btn,copy_btn});
  auto  tr_col = make_column({rvl_btn,rvh_btn,rvv_btn,mir_btn});

  auto  row = make_row({ro_col,tr_col});

  return make_column({row,ed_row});
}


widget*
core::
create_tool_widget() noexcept
{
  auto  cb = [](checkbox_event  evt){
    auto  cor = evt->get_common_userdata<core>();

    cor->cancel_drawing();

      switch(evt->get_entry_number())
      {
    case(0): cor->change_mode_to_draw_dot();break;
    case(1): cor->change_mode_to_draw_line();break;
    case(2): cor->change_mode_to_draw_rectangle();break;
    case(3): cor->change_mode_to_fill_rectangle();break;
    case(4): cor->change_mode_to_fill_area();break;
    case(5): cor->change_mode_to_select();break;
    case(6): cor->change_mode_to_paste();break;
    case(7): cor->change_mode_to_layer();break;
      }
  };


  auto  container = new widget;

  auto  first = new radio_button(u"draw dot",cb);

  first->set_common_userdata(this);

  container->append_column_child(first);

  container->append_column_child(new radio_button(*first,u"draw line")     );
  container->append_column_child(new radio_button(*first,u"draw rectangle"));
  container->append_column_child(new radio_button(*first,u"fill rectangle"));
  container->append_column_child(new radio_button(*first,u"fill area")     );
  container->append_column_child(new radio_button(*first,u"select")        );
  container->append_column_child(new radio_button(*first,u"paste")         );
  container->append_column_child(new radio_button(*first,u"layer")         );


  return container;
}




