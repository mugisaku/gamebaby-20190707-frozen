#ifndef ww_battle_section_hpp_is_included
#define ww_battle_section_hpp_is_included


#include"libww/ww_character.hpp"
#include"libww/ww_bar.hpp"
#include"libww/ww_spilling_text.hpp"
#include"libww/ww_party.hpp"
#include<list>




namespace ww{




template<typename  T, int  N>
class
filtering_table
{
  T*  m_buffer[N];

  int  m_number=0;

public:
  filtering_table() noexcept: m_number(0){}

  template<typename  CONTAINER>
  filtering_table(CONTAINER&  c) noexcept{assign(c);}

  template<typename  IT>
  filtering_table(IT  begin_, IT  end_) noexcept{assign(begin_,end_);}

  template<typename  CONTAINER>
  filtering_table&  assign(CONTAINER&  c) noexcept{return assign(std::begin(c),std::end(c));}

  template<typename  IT>
  filtering_table&  assign(IT  begin_, IT  end_) noexcept
  {
    m_number = 0;

      while(begin_ != end_)
      {
        push(&*begin_++);
      }


    return *this;
  }

  template<typename  TESTER>
  filtering_table&  filter(TESTER  tester) noexcept
  {
    filtering_table  tmp(begin(),end());

    m_number = 0;

      for(auto  ptr: tmp)
      {
          if(tester(*ptr))
          {
            push(ptr);
          }
      }


    return *this;
  }

  template<typename  CMP>
  filtering_table&  sort(CMP  cmp) noexcept
  {
    std::stable_sort(begin(),end(),cmp);

    return *this;
  }

  T&  operator[](int  i) const noexcept{return *m_buffer[i];}

  T**  begin() noexcept{return &m_buffer[       0];}
  T**    end() noexcept{return &m_buffer[m_number];}

  void  push(T*   t) noexcept{m_buffer[m_number++] =  t;}
  void  push(T**  t) noexcept{m_buffer[m_number++] = *t;}

  int  size() const noexcept{return m_number;}

  filtering_table&  reset() noexcept{  m_number = 0;  return *this;}

};




enum class
battle_result
{
  continuing,
  draw,
  left_won,
  right_won,

};


enum class
battle_action_kind
{
  null,
  commit,
  attack,
  avoid,
  receive,

};


class
battle_action
{
  battle_action_kind  m_kind;

  int  m_code;

  battles::character*   m_actor;
  battles::character*  m_target;

public:
  battle_action(battle_action_kind  k, int  c, battles::character&  actor, battles::character*  target=nullptr) noexcept:
  m_kind(k), m_code(c), m_actor(&actor), m_target(target){}

  battle_action_kind  get_kind() const noexcept{return m_kind;}

  int  get_code() const noexcept{return m_code;}

  battles::character&  get_actor()  const noexcept{return *m_actor;}
  battles::character*  get_target() const noexcept{return m_target;}

};


struct
square
{
  int  m_left_value=0;
  int  m_right_value=0;

  square() noexcept{}

};


struct
piece
{
  square*  m_base;
  square*  m_end;

  double  m_offense_power;
  double  m_defense_power;
  double  m_movement_power;
  double  m_weight;

  double  m_previous_position;
  double  m_position;

  battles::side  m_side;

  int  m_hp;

};


struct
battle_section
{
  static constexpr int  m_line_length = 30;

  square  m_square_line[m_line_length];

  piece  m_left_piece;
  piece  m_right_piece;

  static constexpr int  m_table_length = 10;

  using character_table = filtering_table<battles::character,m_table_length>;

  gbstd::clock_watch  m_clock_watch;

  gbstd::task_control  m_task_control;
  gbstd::task_list     m_task_list;

  battles::character  m_character_table[m_table_length];

  character_table  m_filtering_table;

  int  m_number_of_total_characters;

  int  m_number_of_left_characters;
  int  m_number_of_right_characters;

  int  m_number_of_playing_characters;

  std::vector<battle_action>  m_action_queue;

  int  m_action_index;

  gbstd::text  m_text;

  gbstd::typewriter  m_typewriter;

  bool  m_command_request;


  battle_section() noexcept;

  void  clear() noexcept;

  template<typename  TESTER>
  character_table&  filter(TESTER  t) noexcept{
    return m_filtering_table.assign(m_character_table).filter(t);
  }

  void  entry_party(const party&  p, battles::side  side) noexcept;

};


}




#endif




