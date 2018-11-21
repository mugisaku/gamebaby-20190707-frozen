#include"libsubiso/subiso.hpp"




namespace subiso{


namespace{
const uint8_t
g_green_binary[] =
{
#include"green.txt"
};


const uint8_t
g_wall_binary[] =
{
#include"wall.txt"
};


const uint8_t
g_waterwall_binary[] =
{
#include"waterwall.txt"
};


const uint8_t
g_water_binary[] =
{
#include"water.txt"
};


gbstd::image
g_green_image(g_green_binary);


gbstd::image
g_wall_image(g_wall_binary);


gbstd::image
g_waterwall_image(g_waterwall_binary);


gbstd::image
g_water_image(g_water_binary);
}




void
plane::
render(int  flags, const gbstd::canvas&  cv, int  z_base) const noexcept
{
  gbstd::point  pt(1,1);

    switch(flags)
    {
  case(top_flag|left_flag    ): pt = {0,0};break;
  case(top_flag              ): pt = {1,0};break;
  case(top_flag|right_flag   ): pt = {2,0};break;
  case(left_flag             ): pt = {0,1};break;
  case(right_flag            ): pt = {2,1};break;
  case(left_flag| bottom_flag): pt = {0,2};break;
  case(           bottom_flag): pt = {1,2};break;
  case(right_flag|bottom_flag): pt = {2,2};break;

  case( top_flag|bottom_flag): pt = {3,0};break;
  case(left_flag| right_flag): pt = {4,0};break;

  case(top_flag| left_flag|bottom_flag): pt = {3,1};break;
  case(top_flag|right_flag|bottom_flag): pt = {4,1};break;

  case(left_flag|right_flag|bottom_flag): pt = {3,2};break;
  case(left_flag|right_flag|   top_flag): pt = {4,2};break;

  case(top_flag|left_flag|right_flag|bottom_flag): pt = {0,3};break;
    }


  gbstd::canvas  bgcv;

  const gbstd::image*  img = &g_green_image;

  int  z_add_amount = 0;

    if(is_top())
    {
        switch(m_box->get_kind())
        {
//      case(box::kind::null ): if(m_box->m_water_value >= m_waterization_level){img = &g_water_image;}break;
      case(box::kind::earth): img = &g_green_image;break;
        }
    }

  else
    {
      z_add_amount = -1;

        switch(m_box->get_kind())
        {
//      case(box::kind::null ): if(m_box->m_water_value >= m_waterization_level){img = &g_waterwall_image;}break;
      case(box::kind::earth): img = &g_wall_image;break;
        }
    }


  bgcv = gbstd::canvas(*img,g_plane_size*pt.x,g_plane_size*pt.y,g_plane_size,g_plane_size);

  cv.blend_canvas(bgcv,0,0,z_base,z_add_amount);
}




}




