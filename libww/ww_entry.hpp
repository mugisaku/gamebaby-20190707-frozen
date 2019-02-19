#ifndef ww_entry_hpp_is_included
#define ww_entry_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include<list>
#include<vector>
#include<functional>


namespace ww{




class
battle_position
{
  int  m_value;

public:
  constexpr battle_position(int  v=0) noexcept: m_value(v){}

  constexpr operator int() const noexcept{return m_value;}

  constexpr bool  operator==(battle_position  rhs) const noexcept{return m_value == rhs.m_value;}
  constexpr bool  operator!=(battle_position  rhs) const noexcept{return m_value != rhs.m_value;}

};


constexpr battle_position   front_pos(0);
constexpr battle_position    back_pos(1);
constexpr battle_position  backup_pos(2);


class
entry
{
  std::string  m_name;

  int  m_hp=0;
  int  m_hp_max=0;

  battle_position  m_pos=front_pos;

public:
  entry() noexcept{}

  entry(const char*  name) noexcept:
  m_name(name){};

  entry(const char*  name, int  hp, battle_position  pos) noexcept:
  m_name(name), m_hp(hp), m_hp_max(hp), m_pos(pos){};

  const std::string&  get_name(              ) const noexcept{return m_name    ;}
  void                set_name(const char*  s)       noexcept{       m_name = s;}

  int   get_hp() const noexcept{return m_hp;}
  void  set_hp(int  v) noexcept{m_hp = (v > m_hp_max)? m_hp_max:((v < 0)? 0:v);}
  void  add_hp(int  v) noexcept{set_hp(m_hp+v);}

  void  fillup_hp() noexcept{m_hp = m_hp_max;}

  int   get_hp_max() const noexcept{return m_hp_max;}
  void  set_hp_max(int  v) noexcept{m_hp_max  = v;}
  void  add_hp_max(int  v) noexcept{m_hp_max += v;}

  battle_position  get_position() const noexcept{return m_pos;}

  void  set_position(battle_position  pos) noexcept{m_pos = pos;}

  bool  is_front_position()  const noexcept{return m_pos == front_pos;}
  bool  is_back_position()   const noexcept{return m_pos == back_pos;}
  bool  is_backup_position() const noexcept{return m_pos == backup_pos;}

  void  render(gbstd::color  force_color, const gbstd::canvas&  cv) const noexcept;

};


}




#endif




