#ifndef gbstd_onch_hpp_is_included
#define gbstd_onch_hpp_is_included


#include"libgbstd/utility.hpp"
#include"libgbstd/sound.hpp"
#include"libgbstd/parser.hpp"
#include<memory>


namespace gbstd{




class onch_definition;
class onch_space;


class
onch_volume
{
  double  m_value;

public:
 constexpr onch_volume(double  v=0) noexcept: m_value(v){}

 constexpr double  get_value() const noexcept{return m_value;}

};


class
onch_frequency
{
  double  m_value;

public:
 constexpr onch_frequency(double  v=0) noexcept: m_value(v){}

 constexpr double  get_value() const noexcept{return m_value;}

};


class
onch_length
{
  uint32_t  m_value;

public:
 constexpr onch_length(uint32_t  v=0) noexcept: m_value(v){}

 constexpr uint32_t  get_value() const noexcept{return m_value;}

};


class
onch_sound
{
  sound_kind  m_kind;

  onch_frequency  m_frequency;
  onch_volume     m_volume;
  onch_length     m_length;

public:
  onch_sound(sound_kind  k) noexcept: m_kind(k){}

  void  set(onch_frequency  f) noexcept{m_frequency = f;}
  void  set(onch_volume     v) noexcept{m_volume    = v;}
  void  set(onch_length     l) noexcept{m_length    = l;}

  uint32_t  get_length() const noexcept{return m_length.get_value();}

  void  output(f32_t*  it, f32_t*  end) const noexcept;

  void  print() const noexcept;

};


enum class
onch_table_kind
{
  row,
  column,
};


class
onch_table
{
  onch_table_kind  m_kind;

  std::vector<const onch_definition*>  m_elements;

  uint32_t  m_length;

public:
  onch_table(onch_table_kind  k) noexcept{reset(k);}

  void  reset(                  ) noexcept;
  void  reset(onch_table_kind  k) noexcept;

  bool  is_row()    const noexcept{return m_kind == onch_table_kind::row;}
  bool  is_column() const noexcept{return m_kind == onch_table_kind::column;}

  void  push(const onch_definition&  def) noexcept;

  uint32_t  get_length() const noexcept{return m_length;}

  std::vector<f32_t>  generate_wave() const noexcept;

  void  output(f32_t*  it, f32_t*  end, std::vector<const onch_table*>&  entry_list) const noexcept;

  void  print() const noexcept;

};


class
onch_definition
{
  std::string  m_name;

  enum class kind{
    null,
    frequency,
    volume,
    length,
    sound,
    table,

  } m_kind=kind::null;

  union data{
    onch_frequency  f;
    onch_volume     v;
    onch_length     l;

    onch_sound   snd;
    onch_table   tbl;

     data(){}
    ~data(){}

  } m_data;

public:
  onch_definition(){}
  onch_definition(const onch_definition& ) noexcept=delete;
  onch_definition(      onch_definition&&) noexcept=delete;
  onch_definition(const std::string&  name, onch_volume      v) noexcept{assign(name,v);}
  onch_definition(const std::string&  name, onch_frequency   f) noexcept{assign(name,f);}
  onch_definition(const std::string&  name, onch_length      l) noexcept{assign(name,l);}
  onch_definition(const std::string&  name, onch_sound     snd) noexcept{assign(name,snd);}
  onch_definition(const std::string&  name, onch_table&&   tbl) noexcept{assign(name,std::move(tbl));}
 ~onch_definition(){clear();}

  onch_definition&  operator=(const onch_definition& ) noexcept=delete;
  onch_definition&  operator=(      onch_definition&&) noexcept=delete;

  void                set_name(const std::string&  name)       noexcept{       m_name = name;}
  const std::string&  get_name(                        ) const noexcept{return m_name       ;}

  void  clear() noexcept;

  onch_definition&  assign(const std::string&  name, onch_volume      v) noexcept;
  onch_definition&  assign(const std::string&  name, onch_frequency   f) noexcept;
  onch_definition&  assign(const std::string&  name, onch_length      l) noexcept;
  onch_definition&  assign(const std::string&  name, onch_sound     snd) noexcept;
  onch_definition&  assign(const std::string&  name, onch_table&&   tbl) noexcept;

  bool  is_volume()    const noexcept{return m_kind == kind::volume;}
  bool  is_length()    const noexcept{return m_kind == kind::length;}
  bool  is_frequency() const noexcept{return m_kind == kind::frequency;}
  bool  is_sound()     const noexcept{return m_kind == kind::sound;}
  bool  is_table()     const noexcept{return m_kind == kind::table;}

  const onch_frequency&  get_frequency() const noexcept{return m_data.f;}
  const onch_volume&     get_volume()    const noexcept{return m_data.v;}
  const onch_length&     get_length()    const noexcept{return m_data.l;}
  const onch_sound&      get_sound()     const noexcept{return m_data.snd;}
  const onch_table&      get_table()     const noexcept{return m_data.tbl;}

  void  print() const noexcept;

};


class
onch_space
{
  std::vector<std::unique_ptr<onch_definition>>  m_definition_table;

  std::unique_ptr<onch_definition>  read_define(token_block_view&  tbv) noexcept;

  std::unique_ptr<onch_definition>  read_sound(const std::string&  id, sound_kind  sk, token_block_view  tbv) noexcept;
  std::unique_ptr<onch_definition>  read_table(const std::string&  id, onch_table_kind  tk, token_block_view  tbv) noexcept;

public:
  const onch_definition*  find(const char*  name) const noexcept;

  void  load_from_file(const char*  filepath) noexcept;

  void  print() const noexcept;

};




}




#endif




