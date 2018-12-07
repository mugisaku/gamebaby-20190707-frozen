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
      box.uncheck();
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

      box.set_space(this);

      box.set_index(point3d{x,y,z});

      box.be_null();

      box.set_down_box(get_box_pointer(x,y,z-1));
      box.set_up_box(  get_box_pointer(x,y,z+1));

      box.set_middle_box(get_box_pointer(x  ,y-1,z),0);
      box.set_middle_box(get_box_pointer(x+1,y-1,z),1);
      box.set_middle_box(get_box_pointer(x+1,y  ,z),2);
      box.set_middle_box(get_box_pointer(x+1,y+1,z),3);
      box.set_middle_box(get_box_pointer(x  ,y+1,z),4);
      box.set_middle_box(get_box_pointer(x-1,y+1,z),5);
      box.set_middle_box(get_box_pointer(x-1,y  ,z),6);
      box.set_middle_box(get_box_pointer(x-1,y-1,z),7);

      box.set_lower_box(get_box_pointer(x  ,y-1,z-1),0);
      box.set_lower_box(get_box_pointer(x+1,y-1,z-1),1);
      box.set_lower_box(get_box_pointer(x+1,y  ,z-1),2);
      box.set_lower_box(get_box_pointer(x+1,y+1,z-1),3);
      box.set_lower_box(get_box_pointer(x  ,y+1,z-1),4);
      box.set_lower_box(get_box_pointer(x-1,y+1,z-1),5);
      box.set_lower_box(get_box_pointer(x-1,y  ,z-1),6);
      box.set_lower_box(get_box_pointer(x-1,y-1,z-1),7);

      box.set_upper_box(get_box_pointer(x  ,y-1,z+1),0);
      box.set_upper_box(get_box_pointer(x+1,y-1,z+1),1);
      box.set_upper_box(get_box_pointer(x+1,y  ,z+1),2);
      box.set_upper_box(get_box_pointer(x+1,y+1,z+1),3);
      box.set_upper_box(get_box_pointer(x  ,y+1,z+1),4);
      box.set_upper_box(get_box_pointer(x-1,y+1,z+1),5);
      box.set_upper_box(get_box_pointer(x-1,y  ,z+1),6);
      box.set_upper_box(get_box_pointer(x-1,y-1,z+1),7);

      box.update();
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

        if(!box.is_checked())
        {
          box.check();

            if(box.is_null())
            {
              buf.emplace_back(&box);

                if(box.get_middle_box(0)){box_list.emplace_back(box.get_middle_box(0));}
                if(box.get_middle_box(2)){box_list.emplace_back(box.get_middle_box(2));}
                if(box.get_middle_box(4)){box_list.emplace_back(box.get_middle_box(4));}
                if(box.get_middle_box(6)){box_list.emplace_back(box.get_middle_box(6));}
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

              box.get_index().print();
            }


          printf("\n");
        }


      printf("}\n");
    }


  printf("}\n");
}


}




