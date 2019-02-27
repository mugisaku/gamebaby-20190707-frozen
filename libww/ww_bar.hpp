#ifndef ww_bar_hpp_is_included
#define ww_bar_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/stage.hpp"
#include<list>
#include<vector>
#include<functional>


namespace ww{




class
bar
{
public:
  enum class mode{
    left_to_right,
    right_to_left,
  };

private:
  gbstd::fixed_t  m_target_length=0;
  gbstd::fixed_t  m_length=0;

  gbstd::fixed_t  m_increment=0;

  gbstd::point  m_offset;
  gbstd::point  m_pos;

  int  m_thickness=0;

  gbstd::color  m_color;

  bool  m_frozen=false;

  mode  m_mode=mode::left_to_right;

  static void  render(const bar&  b, const gbstd::canvas&  cv) noexcept;
  static void   drive(uint32_t&  delay, bar*  b) noexcept;

public:
  bar() noexcept{}

  void  set_mode(bar::mode  mode) noexcept{m_mode = mode;}

  bool  is_left_to_right() const noexcept{return m_mode == mode::left_to_right;}
  bool  is_right_to_left() const noexcept{return m_mode == mode::right_to_left;}

  void                 set_offset(gbstd::point  pt)       noexcept{       m_offset = pt;}
  const gbstd::point&  get_offset(                ) const noexcept{return m_offset     ;}

  void                 set_position(gbstd::point  pt)       noexcept{       m_pos = pt;}
  const gbstd::point&  get_position(                ) const noexcept{return m_pos     ;}

  void        set_thickness(int  v)       noexcept{       m_thickness = v;}
  const int&  get_thickness(      ) const noexcept{return m_thickness    ;}

  int   get_length(      ) const noexcept{return m_length    ;}
  void  set_length(int  v)       noexcept{       m_length = v;}

  const gbstd::fixed_t&   get_increment(                            ) const noexcept{return m_increment;}
  void                    set_increment(int  length, int  num_frames)       noexcept;

  const gbstd::color&  get_color(                   ) const noexcept{return m_color        ;}
  void                 set_color(gbstd::color  color)       noexcept{       m_color = color;}

  void    freeze() noexcept{m_frozen =  true;}
  void  unfreeze() noexcept{m_frozen = false;}

  bool  is_frozen()   const noexcept{return m_frozen;}
  bool  is_finished() const noexcept{return m_length == m_target_length;}

  void                   set_target_length(int  length)       noexcept;
  const gbstd::fixed_t&  get_target_length(           ) const noexcept{return m_target_length;}

  void  startup() noexcept;
  void  cleanup() noexcept;

};


}




#endif




