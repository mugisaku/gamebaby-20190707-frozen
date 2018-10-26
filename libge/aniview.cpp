#include"libge/ge.hpp"
#include"libgbpng/png.hpp"




namespace ge{


using namespace gbstd;


namespace{
const uint32_t  g_time_table[] = {1000,600,200,80};
}


void
aniview::
psh(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  ctx = *evt->get_userdata<context>();

      auto&  pt = ctx.m_current_index;

      int  w = ctx.get_cell_width() ;
      int  h = ctx.get_cell_height();

      ctx.m_aniview->m_frames.emplace_back(ctx.m_source_image,w*pt.x,h*pt.y,w,h);
      ctx.m_aniview->m_points.emplace_back(pt);

      ctx.m_aniview->update_state_label();
    }
}


void
aniview::
pop(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  ctx = *evt->get_userdata<context>();

        if(ctx.m_aniview->m_frames.size())
        {
          ctx.m_aniview->m_frames.pop_back();
          ctx.m_aniview->m_points.pop_back();

          ctx.m_aniview->update_state_label();
        }
    }
}


void
aniview::
dial_callback(dial_event  evt) noexcept
{
  auto&  ctx = *evt->get_userdata<context>();

  ctx.m_aniview->m_delay_time = g_time_table[evt.get_new_value()-1];
}




aniview::
aniview(context&  ctx) noexcept:
widget()
{
  auto  d = new dial(1,4,dial_callback);

  auto  psh_btn = new button(new label(u"Push",colors::black),psh);
  auto  pop_btn = new button(new label( u"Pop",colors::black),pop);

  gbstd::set_userdata({this,d,psh_btn,pop_btn},&ctx);

  auto  op_col = make_column({psh_btn,pop_btn});

  m_state_label = new label(u"--/ 0",colors::white);

  m_delay_time = g_time_table[0];
  m_next_time  =               0;

  auto  speed_frame = new frame("speed",d);

  m_first_child = make_row({m_state_label,op_col});

  append_child(m_first_child,0,0);

  append_column_child(speed_frame);

  clear();
}




void
aniview::
clear() noexcept
{
  m_index = 0;

  update_state_label();

  request_redraw();
}


void
aniview::
check() noexcept
{
    if(m_frames.size())
    {
        if(g_time >= m_next_time)
        {
          m_next_time = (g_time+m_delay_time);

            if(++m_index >= m_frames.size())
            {
              m_index = 0;
            }


          update_state_label();

          request_redraw();
        }
    }
}


namespace{
void
transfer(const gbstd::canvas  src_cv, gbpng::direct_color_image&  dst_img) noexcept
{
  int  w = src_cv.get_width() ;
  int  h = src_cv.get_height();

  auto  dst = dst_img.get_row_pointer(0);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto  color = src_cv.get_pixel_pointer(x,y)->color;

        if(color)
        {
          *dst++ = color.get_r255();
          *dst++ = color.get_g255();
          *dst++ = color.get_b255();
          *dst++ = 0xFF;
        }

      else
        {
          *dst++ = 0;
          *dst++ = 0;
          *dst++ = 0;
          *dst++ = 0;
        }
    }}
}
}


void
aniview::
save_as_apng(const char*  filepath) const noexcept
{
    if(m_frames.size())
    {
      std::vector<uint8_t>  buf;

        try
        {
          using namespace gbpng;

          auto&  ctx = *get_userdata<context>();

          auto  it     = m_frames.begin();
          auto  it_end = m_frames.end();

          const int  w = it->get_width() ;
          const int  h = it->get_height();

          image_header  ihdr(w,h,pixel_format::rgba);

          animation_builder  ani(ihdr,m_delay_time);

          direct_color_image  dst_img(w,h);

          gbstd::image  tmp_img(w,h);

          gbstd::canvas  tmp_cv(tmp_img);

          bool  show_underlay = ctx.m_core->get_display().test_whether_show_underlay();

            while(it != it_end)
            {
              ctx.m_core->get_display().render_background(tmp_cv,2);

                if(show_underlay)
                {
                  ctx.m_core->get_display().render_underlay(tmp_cv,1);
                }


              tmp_cv.draw_canvas(*it++,0,0);

              transfer(tmp_cv,dst_img);

              ani.append(dst_img);
            }


          auto  ls = ani.build(0);

          buf.resize(ls.calculate_stream_size());

          ls.write_png_to_memory(buf.data());
        }


        catch(std::exception&  e)
        {
          printf("error: %s\n",e.what());
        }



#ifdef __EMSCRIPTEN__
      download(buf.data(),buf.size(),filepath);
#else
      write_to_file(buf.data(),buf.size(),filepath);
#endif
    }
}


void
aniview::
process_before_reform() noexcept
{
  auto&  ctx = *get_userdata<context>();

  int  w = ctx.get_cell_width() ;
  int  h = ctx.get_cell_height();

  set_content_width( w);
  set_content_height(h);

  m_first_child->set_offset(0,h);
}


void
aniview::
rebase() noexcept
{
  m_frames.clear();

  auto&  ctx = *get_userdata<context>();

  const gbstd::image&  img = ctx.m_source_image;

  int  img_w = img.get_width() ;
  int  img_h = img.get_height();

  int  w = ctx.get_cell_width() ;
  int  h = ctx.get_cell_height();

    for(auto&  pt: m_points)
    {
        if(((pt.x+w) < img_w) &&
           ((pt.y+h) < img_h))
        {
          m_frames.emplace_back(img,w*pt.x,h*pt.y,w,h);
        }
    }
}


void
aniview::
update_state_label() noexcept
{
    if(m_frames.empty())
    {
      m_state_label->set_text("--/ 0");
    }

  else
    {
      string_form  sf;

      m_state_label->set_text(sf("%2d/%2d",m_index+1,m_frames.size()));
    }
}


void
aniview::
render(const canvas&  cv) noexcept
{
  auto&  ctx = *get_userdata<context>();

    if(m_index < m_frames.size())
    {
      auto&  frm = m_frames[m_index];

      canvas  cocv(cv,0,0,frm.get_width(),frm.get_height());

      ctx.m_core->get_display().render_background(cocv,2);

        if(ctx.m_core->get_display().test_whether_show_underlay())
        {
          ctx.m_core->get_display().render_underlay(cv,1);
        }


      cv.draw_canvas(frm,0,0);
    }
}


}




