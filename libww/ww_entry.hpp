#ifndef ww_entry_hpp_is_included
#define ww_entry_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libww/ww_visual_object.hpp"
#include<list>
#include<vector>
#include<functional>


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

};


namespace positions{
constexpr position   front(0);
constexpr position    back(1);
constexpr position  backup(2);
}


class
side
{
  int  m_data;

public:
  static constexpr int   m_left_value = 0;
  static constexpr int  m_right_value = 1;

  constexpr side(int  v=0) noexcept: m_data(v){}

  constexpr bool  operator==(side  rhs) const noexcept{return m_data == rhs.m_data;}
  constexpr bool  operator!=(side  rhs) const noexcept{return m_data != rhs.m_data;}

  constexpr bool  is_left()  const noexcept{return m_data == m_left_value;}
  constexpr bool  is_right() const noexcept{return m_data == m_right_value;}

  constexpr side  get_opposite() const noexcept{return side(is_left()? m_right_value:m_left_value);}

};


namespace sides{
constexpr side   left = side(side::m_left_value);
constexpr side  right = side(side::m_right_value);
}


class
character_head
{
  std::string  m_name;

public:
  character_head() noexcept{};

  character_head(std::string_view  name) noexcept:
  m_name(name){};

  const std::string&  get_name(              ) const noexcept{return m_name    ;}
  void                set_name(const char*  s)       noexcept{       m_name = s;}

};


class
character_body
{
  int  m_hp=0;
  int  m_hp_max=0;

  position  m_pos=positions::front;

public:
  character_body() noexcept{}

  character_body(int  hp, position  pos) noexcept:
  m_hp(hp), m_hp_max(hp), m_pos(pos){};

  int   get_hp() const noexcept{return m_hp;}
  void  set_hp(int  v) noexcept{m_hp = (v > m_hp_max)? m_hp_max:((v < 0)? 0:v);}
  void  add_hp(int  v) noexcept{set_hp(m_hp+v);}

  void  fillup_hp() noexcept{m_hp = m_hp_max;}

  int   get_hp_max() const noexcept{return m_hp_max;}
  void  set_hp_max(int  v) noexcept{m_hp_max  = v;}
  void  add_hp_max(int  v) noexcept{m_hp_max += v;}

  position  get_position() const noexcept{return m_pos;}

  void  set_position(position  pos) noexcept{m_pos = pos;}

  bool  is_front_position()  const noexcept{return m_pos == positions::front;}
  bool  is_back_position()   const noexcept{return m_pos == positions::back;}
  bool  is_backup_position() const noexcept{return m_pos == positions::backup;}

};




class
character
{
  struct flags{
    static constexpr int  opened = 1;
  };


  gbstd::status_value<int>  m_status;

  battles::character_head  m_head;

  battles::character_body  m_original_body;
  battles::character_body  m_product_body;
  battles::character_body  m_proof_body;

  int  m_ap=0;

public:
  company() noexcept;

  operator bool() const noexcept{return m_status.test(flags::opened);}

  const int&  get_ap() const noexcept{return m_ap;}

  const battles::character_body&  get_product_body() const noexcept{return m_product_body;}
  const battles::character_body&  get_proof_body()   const noexcept{return m_proof_body;}

  void  reset() noexcept;

  void  open(const battles::character_head&  h, const battles::character_body&  b) noexcept;
  void  close() noexcept;

};


class
instruction_queue
{
  uint32_t*  m_data;

  uint32_t*  m_bp;
  uint32_t*  m_qp;

public:
  struct block{
    uint32_t   opcode;
    uint32_t  opvalue;
  };

  void  push(std::initializer_list<block>  ls) noexcept;;

};




struct
field
{
  int  m_index;

  gbstd::color  m_color;

  side  m_side;

  real_point  m_backup_point;
  real_point  m_back_point;
  real_point  m_front_point;
 
  real_point  m_name_point;
  real_point  m_hp_point;

  bar  m_hp_bar;

  character  m_character;

};




}


}




#endif




