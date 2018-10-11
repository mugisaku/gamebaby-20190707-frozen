#include"libgbstd/control.hpp"
#include<cstdio>
#include<cstring>




namespace gbstd{




uint32_t  g_time;

std::vector<point>  g_point_buffer;

std::vector<uint8_t>  g_dropped_file;

bool  g_needed_to_redraw;

user_input  g_modified_input;
user_input           g_input;




}




