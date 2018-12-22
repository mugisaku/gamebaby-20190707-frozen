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


struct
company
{
  int  m_number_of_soldiers=8000;

  battle_position  m_pos=front_pos;

  int  get_number_of_soldiers() const noexcept{return m_number_of_soldiers;}


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


struct
row
{
  force*  m_force=nullptr;

  company*  m_original;
  company   m_variable;

  int  m_ap=0;

  bool  m_entry_flag=false;

  int  m_animation_phase;
  int  m_animation_counter;
  
  gbstd::point  m_rendering_pos;

  bool  is_surviving() const noexcept{return m_original && m_variable.get_number_of_soldiers();}

  bool  is_left()  const noexcept;
  bool  is_right() const noexcept;

  void  move_to_advance(int  n) noexcept{m_rendering_pos.x += is_left()?  n:-n;}
  void  move_to_back(   int  n) noexcept{m_rendering_pos.x += is_left()? -n: n;}

  void  reset_animation(int  phase=0) noexcept;

  void  reset(force&  force, company*  org) noexcept;

  void  render(ww::side  side, gbstd::color  force_color, const gbstd::canvas&  cv) const noexcept;

};


struct
force
{
  row  m_rows[8];

  side  m_side=side::left;

  gbstd::color  m_color;

  std::list<row*>  m_actor_queue;

  int  get_rows(battle_position  pos, row**  buf, int  n) noexcept;

  void  increase_ap() noexcept;

  bool  can_continue_fight() const noexcept;

  void  reset(ww::side  side, const force_initializer&  init) noexcept;

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

  attack_context  m_attack_context;

  gbstd::process  m_base_process;

  void  reset(const force_initializer&  l,
              const force_initializer&  r) noexcept;

  void  pump_queue(std::list<row*>&  q) noexcept;

  row*  pump(std::list<row*>&  q) noexcept;

  void  pump_queue() noexcept;

  void  step() noexcept{m_base_process.step();}

  void  render(const gbstd::canvas&  cv) const noexcept;

};




void  attack(gbstd::execution&  exec, battle_context*  ctx) noexcept;
void   fight(gbstd::execution&  exec, battle_context*  ctx) noexcept;
void   judge(gbstd::execution&  exec, battle_context*  ctx) noexcept;




}




#endif




