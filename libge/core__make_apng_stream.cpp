#include"libge/ge.hpp"
#include"libgbpng/png.hpp"


namespace ge{


using namespace gbstd;
using namespace gbpng;


std::vector<uint8_t>
core::
make_apng_stream(const std::vector<point>&  pts, uint32_t  delay) const noexcept
{
  std::vector<uint8_t>   buf;
/*
    try
    {
      const int  w = m_canvas.get_width() ;
      const int  h = m_canvas.get_height();

      image_header  ihdr(w,h,pixel_format::rgba);

      animation_builder  ani(ihdr,delay);

      direct_color_image  dst_img(w,h);

      auto&  img = m_image;

        for(auto&  pt: pts)
        {
          auto  dst = dst_img.get_row_pointer(0);

          const int  x_base = width() *pt.x;
          const int  y_base = _height()*pt.y;

            for(int  y = 0;  y < h;  ++y){
            for(int  x = 0;  x < w;  ++x){
              auto  color = m_image->get_pixel_pointer(x_base+x,y_base+y)->color;

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
*/

  return std::move(buf);
}




}




