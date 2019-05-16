#ifndef ww_character_hpp_is_included
#define ww_character_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libww/ww_field.hpp"
#include"libww/ww_string_display.hpp"




namespace ww{
namespace battles{



class
position
{
  int  m_value;

public:
  constexpr position(int  v=0) noexcept: m_value(v){}

  constexpr operator int() const noexcept{return m_value;}

  constexpr bool  operator==(position  rhs) const noexcept{return m_value == rhs.m_value;}
  constexpr bool  operator!=(position  rhs) const noexcept{return m_value != rhs.m_value;}

  constexpr bool  is_front()  const noexcept{return m_value == 0;}
  constexpr bool  is_back()   const noexcept{return m_value == 1;}
  constexpr bool  is_backup() const noexcept{return m_value == 2;}

};


namespace positions{
constexpr position   front(0);
constexpr position    back(1);
constexpr position  backup(2);
}


struct
character_head
{
  std::u16string  m_name;

};


struct
character_body
{
  int  m_hp=0;
  int  m_hp_max=0;

  position  m_position=positions::front;

};




class field;


struct
character
{
  struct flags{
    static constexpr int  active = 1;
  };


  gbstd::status_value<int>  m_status;

  battles::character_head  m_head;

  battles::character_body  m_original_body;
  battles::character_body  m_product_body;
  battles::character_body  m_proof_body;

  display_string  m_name_string;
  display_string  m_hp_string;

  string_display  m_name_display;
  string_display  m_hp_display;

  int  m_ap=0;

  field  m_field;

  static void  draw(gbstd::task_control  ctrl, const gbstd::canvas&  cv, character&  c) noexcept;
  static void  tick(gbstd::task_control  ctrl,                           character&  c) noexcept;

  character() noexcept{m_field.m_character = this;}

  operator bool() const noexcept{return m_status.test(flags::active);}

  bool  can_continue_battle() const noexcept{return *this && (m_product_body.m_hp > 0);}

  void  copy_body() noexcept
  {
    m_product_body = m_original_body;
    m_proof_body   = m_original_body;
  }

};




}}




#endif




