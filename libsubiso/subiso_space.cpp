#include"libsubiso/subiso.hpp"




namespace subiso{




void
space::
reset_distance_all() noexcept
{
    for(auto&  box: m_boxes)
    {
      box.set_distance(0xFFFFFFFF);
    }
}


void
space::
clear_search_flags() noexcept
{
    for(auto&  box: m_boxes)
    {
      box.m_search_flags = 0;
    }
}


box*
space::
get_box_pointer(int  x, int  y, int  z) noexcept
{
    if((x >= 0) && (x < m_x_length) &&
       (y >= 0) && (y < m_y_length) &&
       (z >= 0) && (z < m_z_length))
    {
      return &get_box(x,y,z);
    }


  return nullptr;
}


void
space::
resize(int  xl, int  yl, int  zl) noexcept
{
  m_boxes.resize(xl*yl*zl);

  m_x_length = xl;
  m_y_length = yl;
  m_z_length = zl;

    for(int  z = 0;  z < zl;  ++z){
    for(int  y = 0;  y < yl;  ++y){
    for(int  x = 0;  x < xl;  ++x){
      auto&  box = get_box(x,y,z);

      box.m_space = this;

      box.m_index = point3d{x,y,z};

      box.m_kind = box::kind::null;

      box.m_top_plane.m_box  = &box;
      box.m_top_plane.m_kind = plane::kind::top;

        for(auto&  pl: box.m_side_planes)
        {
          pl.m_box  = &box;
          pl.m_kind = plane::kind::side;
        }


      box.m_down_box = get_box_pointer(x,y,z-1);
      box.m_up_box   = get_box_pointer(x,y,z+1);

      box.m_middle_boxes[0] = get_box_pointer(x  ,y-1,z);
      box.m_middle_boxes[1] = get_box_pointer(x+1,y-1,z);
      box.m_middle_boxes[2] = get_box_pointer(x+1,y  ,z);
      box.m_middle_boxes[3] = get_box_pointer(x+1,y+1,z);
      box.m_middle_boxes[4] = get_box_pointer(x  ,y+1,z);
      box.m_middle_boxes[5] = get_box_pointer(x-1,y+1,z);
      box.m_middle_boxes[6] = get_box_pointer(x-1,y  ,z);
      box.m_middle_boxes[7] = get_box_pointer(x-1,y-1,z);

      box.m_lower_boxes[0] = get_box_pointer(x  ,y-1,z-1);
      box.m_lower_boxes[1] = get_box_pointer(x+1,y-1,z-1);
      box.m_lower_boxes[2] = get_box_pointer(x+1,y  ,z-1);
      box.m_lower_boxes[3] = get_box_pointer(x+1,y+1,z-1);
      box.m_lower_boxes[4] = get_box_pointer(x  ,y+1,z-1);
      box.m_lower_boxes[5] = get_box_pointer(x-1,y+1,z-1);
      box.m_lower_boxes[6] = get_box_pointer(x-1,y  ,z-1);
      box.m_lower_boxes[7] = get_box_pointer(x-1,y-1,z-1);

      box.m_upper_boxes[0] = get_box_pointer(x  ,y-1,z+1);
      box.m_upper_boxes[1] = get_box_pointer(x+1,y-1,z+1);
      box.m_upper_boxes[2] = get_box_pointer(x+1,y  ,z+1);
      box.m_upper_boxes[3] = get_box_pointer(x+1,y+1,z+1);
      box.m_upper_boxes[4] = get_box_pointer(x  ,y+1,z+1);
      box.m_upper_boxes[5] = get_box_pointer(x-1,y+1,z+1);
      box.m_upper_boxes[6] = get_box_pointer(x-1,y  ,z+1);
      box.m_upper_boxes[7] = get_box_pointer(x-1,y-1,z+1);
    }}}
}






std::vector<box*>
space::
search_slice(int  x, int  y, int  z) noexcept
{
  std::vector<box*>  buf;

  std::vector<box*>  box_list;

  int  i = 0;

  box_list.emplace_back(&get_box(x,y,z));

    while(i < box_list.size())
    {
      auto&  box = *box_list[i++];

      auto&  flag = box.m_search_flags;

        if(!flag)
        {
          flag = 1;

            if(box.is_null())
            {
              buf.emplace_back(&box);

                if(box.m_middle_boxes[0]){box_list.emplace_back(box.m_middle_boxes[0]);}
                if(box.m_middle_boxes[2]){box_list.emplace_back(box.m_middle_boxes[2]);}
                if(box.m_middle_boxes[4]){box_list.emplace_back(box.m_middle_boxes[4]);}
                if(box.m_middle_boxes[6]){box_list.emplace_back(box.m_middle_boxes[6]);}
            }
        }
    }


  return std::move(buf);
}


void
space::
print() const noexcept
{
  printf("{\n");

    for(int  z = m_z_length-1;  z >= 0;  --z)
    {
      printf("{\n");

        for(int  y = m_y_length-1;  y >= 0;  --y)
        {
            for(int  x = 0;  x < m_x_length;  ++x)
            {
              auto&  box = get_box(x,y,z);

              box.m_index.print();
            }


          printf("\n");
        }


      printf("}\n");
    }


  printf("}\n");
}


}




