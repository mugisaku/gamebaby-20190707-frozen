#include"libww/ww_menu.hpp"
#include"libww/ww_context.hpp"




namespace ww{
namespace menus{




table&
table::
resize(int  w, int  h) noexcept
{
  m_width  = w;
  m_height = h;

  m_container.resize(w*h);

  return *this;
}


void
table::
draw(int  x, int  y, int  w, int  h, void  (*cb)(const entry&,gbstd::dummy&,const gbstd::canvas&), const gbstd::canvas&  cv) noexcept
{
    for(int  yy = 0;  yy < h;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      auto&  ent = *get_entry_pointer(x+xx,
                                      y+yy);

      gbstd::canvas  dst_cv(cv,m_entry_width *xx,
                               m_entry_height*yy,
                               m_entry_width,
                               m_entry_height);

      cb(ent,ent.get_data<gbstd::dummy>(),dst_cv);
    }}
}




}}




