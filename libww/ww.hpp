#ifndef ww_hpp_is_included
#define ww_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include<list>
#include<vector>


namespace ww{


constexpr int  g_frame_w = gbstd::g_font_width *6;
constexpr int  g_frame_h = gbstd::g_font_height*2;


class force;
class row;


enum class
side
{
  left,
  right,

};


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
window
{
protected:
  gbstd::point  m_pos;

  int  m_width =0;
  int  m_height=0;

public:
  

};


struct
company
{
  const char*  m_name;

  int  m_hp=6000;

  battle_position  m_pos=front_pos;

  const char*  get_name() const noexcept{return m_name;}

  int  get_hp() const noexcept{return m_hp;}


  void  render(gbstd::color  force_color, const gbstd::canvas&  cv) const noexcept;

};


struct
force_initializer
{
  gbstd::color  m_color;

  std::vector<company*>  m_company_list;

  void  clear_list() noexcept{m_company_list.clear();}

  void  add(company*  com, int  n=1) noexcept
  {
      while(n--)
      {
        m_company_list.emplace_back(com++);
      }
  }

};


class
blink_context
{
  int  m_state=0;

public:
  void   enable() noexcept{m_state |= 1;}
  void  disable() noexcept{m_state  = 0;}

  bool  is_valid()   const noexcept{return m_state&1;}
  bool  is_visible() const noexcept{return m_state&2;}

  void  step() noexcept{if(is_visible()){m_state &= 1;} else{m_state |= 2;}}

};


struct
row
{
  force*  m_force=nullptr;

  company*  m_original;
  company   m_variable;

  int  m_ap=0;

  bool  m_entry_flag=false;

  blink_context  m_blink_context;

  bool  m_white_flag=false;

  gbstd::process  m_process;

  int  m_animation_phase;
  int  m_animation_counter;

  gbstd::point  m_base_pos;
  gbstd::point  m_current_pos;

  bool  is_surviving() const noexcept{return m_original && m_variable.get_hp();}

  bool  is_left()  const noexcept;
  bool  is_right() const noexcept;

  void  move_to_advance(int  n) noexcept{m_current_pos.x += -n;}
  void  move_to_back(   int  n) noexcept{m_current_pos.x -= -n;}

  void  reset_animation(int  phase=0) noexcept;

  void  reset(force&  force, company*  org) noexcept;

  void  render(const gbstd::canvas&  cv) const noexcept;

};


struct
force
{
  static constexpr int  m_number_of_rows = 3;

  row  m_rows[m_number_of_rows];

  side  m_side=side::left;

  gbstd::color  m_color;

  std::list<row*>  m_actor_queue;

  gbstd::process  m_process;

  int  get_rows(battle_position  pos, row**  buf, int  n) noexcept;

  void  increase_ap() noexcept;

  bool  can_continue_fight() const noexcept;

  void  reset(ww::side  side, const force_initializer&  init) noexcept;

  void  update() noexcept;

  void  render(const gbstd::canvas&  cv) const noexcept;

};




struct
attack_context
{
  row*  m_actor;
  row*  m_target;

  void  reset(row&  actor, row&  target) noexcept
  {
    m_actor  = &actor;
    m_target = &target;

     actor.reset_animation();
    target.reset_animation();
  }

};


struct
battle_context
{
  force  m_left_force ;
  force  m_right_force;

  uint32_t  m_time=0;

  std::list<row*>  m_actor_queue;

  gbstd::text  m_text;

  gbstd::typewriter  m_typewriter;

  static constexpr gbstd::point  m_console_pos = gbstd::point(0,0);

  attack_context  m_attack_context;

  gbstd::process  m_base_process;

  void  reset(const force_initializer&  l,
              const force_initializer&  r) noexcept;

  void  pump_queue(std::list<row*>&  q) noexcept;

  row*  pump(std::list<row*>&  q) noexcept;

  void  pump_queue() noexcept;

  void  step() noexcept;

  void  render(const gbstd::canvas&  cv) const noexcept;

};




void  attack(gbstd::execution&  exec, battle_context*  ctx) noexcept;
void   fight(gbstd::execution&  exec, battle_context*  ctx) noexcept;
void   judge(gbstd::execution&  exec, battle_context*  ctx) noexcept;




}




#endif




