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


const uint8_t
g_stairs_binary[] =
{
#include"stairs.png.txt"
};


gbstd::image
g_green_image(g_green_binary);


gbstd::image
g_wall_image(g_wall_binary);


gbstd::image
g_waterwall_image(g_waterwall_binary);


gbstd::image
g_water_image(g_water_binary);


gbstd::image
g_stairs_image(g_stairs_binary);
}




void
plane::
render(direction  dir, int  flags, const gbstd::canvas&  cv, int  z_base) const noexcept
{
  gbstd::point  pt(1,1);

  gbstd::canvas  bgcv;

  const gbstd::image*  img = &g_green_image;

  int  z_add_amount = 0;

    if(m_box->is_stairs())
    {
      img = &g_stairs_image;

      auto  d = m_box->get_direction()-dir;

        if(is_top())
        {
            switch(d)
            {
          case(directions::front): pt = {2,0};break;
          case(directions::right): pt = {1,0};break;
          case(directions::back ): pt = {3,0};break;
          case(directions::left ): pt = {0,0};break;
            }
        }

      else
        {
          z_add_amount = -1;

            switch(d)
            {
          case(directions::front): pt = {2,1};break;
          case(directions::right): pt = {1,1};break;
          case(directions::back ): pt = {3,1};break;
          case(directions::left ): pt = {0,1};break;
            }
        }
    }

  else
    {
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


        if(is_top())
        {
            switch(m_box->get_kind())
            {
          case(box::kind::water ): img = &g_water_image;break;
          case(box::kind::earth ): img = &g_green_image;break;
          case(box::kind::stairs): img = &g_stairs_image;break;
            }
        }

      else
        {
          z_add_amount = -1;

            switch(m_box->get_kind())
            {
          case(box::kind::earth ): img = &g_wall_image;break;
          case(box::kind::water ): img = &g_waterwall_image;break;
          case(box::kind::stairs): img = &g_stairs_image;break;
            }
        }
    }


  bgcv = gbstd::canvas(*img,g_plane_size*pt.x,g_plane_size*pt.y,g_plane_size,g_plane_size);

  cv.blend_canvas(bgcv,0,0,z_base,z_add_amount);
}




}




