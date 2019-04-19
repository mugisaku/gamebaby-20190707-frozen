#ifndef gbstd_onch_hpp_is_included
#define gbstd_onch_hpp_is_included


#include"libgbstd/utility.hpp"
#include"libgbstd/sound.hpp"
#include"libgbstd/parser.hpp"
#include"libgbstd/file_op.hpp"
#include<memory>


namespace gbstd{




class onch_element;
class onch_word;
class onch_text;
class onch_table;
class onch_definition;
class onch_parameter;
class onch_space;


struct
onch_output_context
{
  uint32_t  m_sampling_rate=0;

  const onch_space*  m_space=nullptr;

  f32_t     m_last_volume=0;
  f32_t     m_last_frequency=0;
  f32_t     m_last_vibrato_frequency=0;
  f32_t     m_last_vibrato_intensity=0;
  uint32_t  m_last_play_length=0;
  uint32_t  m_last_rest_length=0;

  f32_t*   m_it=nullptr;
  f32_t*  m_end=nullptr;

  f32_t     get_vibrato_frequency(int  spec, int value) noexcept;
  f32_t     get_vibrato_intensity(int  spec, int value) noexcept;

  f32_t     get_volume(     int  spec, int value) noexcept;
  f32_t     get_frequency(  int  spec, int value) noexcept;
  uint32_t  get_play_length(int  spec, int value) noexcept;
  uint32_t  get_rest_length(int  spec, int value) noexcept;

};




enum class
onch_word_kind
{
  null,
  play,
  rest,

};


class
onch_word
{
  static constexpr int   m_k_shift_amount = 30;
  static constexpr int  m_bf_shift_amount = 25;
  static constexpr int   m_l_shift_amount = 20;
  static constexpr int  m_v0_shift_amount = 15;
  static constexpr int  m_v1_shift_amount = 10;
  static constexpr int  m_f0_shift_amount =  5;
  static constexpr int  m_f1_shift_amount =  0;

  uint32_t  m_data;

  gbstd::sound_instruction  make_instruction(onch_output_context&  ctx) const noexcept;

public:
  struct specs{
    static constexpr int  no_spec = 0;
    static constexpr int     zero = 1;
    static constexpr int    index = 2;
  };

  constexpr onch_word() noexcept: m_data(0){}

  onch_word_kind  get_kind() const noexcept{return static_cast<onch_word_kind>((m_data>>m_k_shift_amount)&3);}

  bool  is_null() const noexcept{return get_kind() == onch_word_kind::null;}
  bool  is_play() const noexcept{return get_kind() == onch_word_kind::play;}
  bool  is_rest() const noexcept{return get_kind() == onch_word_kind::rest;}

  int  get_l_spec()  const noexcept{return ((m_data>> m_l_shift_amount)>>3&3);}
  int  get_bf_spec() const noexcept{return ((m_data>>m_bf_shift_amount)>>3&3);}
  int  get_v0_spec() const noexcept{return ((m_data>>m_v0_shift_amount)>>3&3);}
  int  get_v1_spec() const noexcept{return ((m_data>>m_v1_shift_amount)>>3&3);}
  int  get_f0_spec() const noexcept{return ((m_data>>m_f0_shift_amount)>>3&3);}
  int  get_f1_spec() const noexcept{return ((m_data>>m_f1_shift_amount)>>3&3);}

  int  get_l_value()  const noexcept{return (m_data>> m_l_shift_amount)&7;}
  int  get_bf_value() const noexcept{return (m_data>>m_bf_shift_amount)&7;}
  int  get_v0_value() const noexcept{return (m_data>>m_v0_shift_amount)&7;}
  int  get_v1_value() const noexcept{return (m_data>>m_v1_shift_amount)&7;}
  int  get_f0_value() const noexcept{return (m_data>>m_f0_shift_amount)&7;}
  int  get_f1_value() const noexcept{return (m_data>>m_f1_shift_amount)&7;}

  onch_word&  reset() noexcept{  m_data = 0;  return *this;}

  onch_word&  set_kind(onch_word_kind  k) noexcept{  m_data |= static_cast<int>(k)<<m_k_shift_amount;  return *this;}

  onch_word&  set_l(int  lspec, int  l) noexcept;
  onch_word&  set_b(int  bfspec, int  bf) noexcept;
  onch_word&  set_v(int  v0spec, int  v0, int  v1spec, int  v1) noexcept;
  onch_word&  set_f(int  f0spec, int  f0, int  f1spec, int  f1) noexcept;

  uint32_t  get_output_length(onch_output_context&  ctx) const noexcept;

  void  output(sound_kind  k, onch_output_context&  ctx) const noexcept;

  void  print() const noexcept;

};


class
onch_text
{
  std::vector<onch_word>  m_words;

public:
  onch_text() noexcept{}

  onch_word&  get_last_word() noexcept{return m_words.back();}

  int  get_length() const noexcept{return m_words.size();}

  void  push(onch_word  w) noexcept{m_words.emplace_back(w);}
  void  push(const onch_text&  txt) noexcept;

  uint32_t  get_output_length(onch_output_context&  ctx) const noexcept;

  void  output(sound_kind  k, onch_output_context&  ctx) const noexcept;

  void  print() const noexcept;

};


class
onch_cell: public onch_text
{
  sound_kind  m_kind;

public:
  onch_cell(sound_kind  k) noexcept: m_kind(k){}

  void  output(onch_output_context&  ctx) const noexcept{onch_text::output(m_kind,ctx);}

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

  std::vector<onch_element>  m_elements;

public:
  onch_table(onch_table_kind  k) noexcept: m_kind(k){}

  bool  is_row()    const noexcept{return m_kind == onch_table_kind::row;}
  bool  is_column() const noexcept{return m_kind == onch_table_kind::column;}

  void  push(onch_element&&  e) noexcept;

  uint32_t  get_output_length(onch_output_context&  ctx) const noexcept;

  void  output(onch_output_context&  ctx) const noexcept;

  void  print() const noexcept;

};


class
onch_element
{
  enum class kind{
    null,
    word,
    text,
    cell,
    table,

  } m_kind=kind::null;

  union data{
    onch_word    wor;
    onch_text    txt;
    onch_cell    cel;
    onch_table   tbl;

     data(){}
    ~data(){}

  } m_data;

public:
  onch_element() noexcept{}
  onch_element(const onch_element&   rhs) noexcept{assign(rhs);}
  onch_element(      onch_element&&  rhs) noexcept{assign(std::move(rhs));}
  onch_element(onch_word     wor) noexcept{assign(wor);}
  onch_element(onch_cell&&   cel) noexcept{assign(std::move(cel));}
  onch_element(onch_text&&   txt) noexcept{assign(std::move(txt));}
  onch_element(onch_table&&  tbl) noexcept{assign(std::move(tbl));}
 ~onch_element(){clear();}

  onch_element&  operator=(const onch_element&   rhs) noexcept{return assign(rhs);}
  onch_element&  operator=(      onch_element&&  rhs) noexcept{return assign(std::move(rhs));}

  operator bool() const noexcept{return m_kind != kind::null;}

  void  clear() noexcept;

  onch_element&  assign(const onch_element&   rhs) noexcept;
  onch_element&  assign(      onch_element&&  rhs) noexcept;
  onch_element&  assign(onch_word         wor) noexcept;
  onch_element&  assign(onch_cell&&       cel) noexcept;
  onch_element&  assign(onch_text&&       txt) noexcept;
  onch_element&  assign(onch_table&&       tbl) noexcept;

  bool  is_word()  const noexcept{return m_kind == kind::word;}
  bool  is_cell()  const noexcept{return m_kind == kind::cell;}
  bool  is_text()  const noexcept{return m_kind == kind::text;}
  bool  is_table() const noexcept{return m_kind == kind::table;}

  const onch_word&    get_word()  const noexcept{return m_data.wor;}
  const onch_text&    get_text()  const noexcept{return m_data.txt;}
  const onch_cell&    get_cell()  const noexcept{return m_data.cel;}
  const onch_table&   get_table() const noexcept{return m_data.tbl;}

  uint32_t  get_output_length(onch_output_context&  ctx) const noexcept;

  std::vector<f32_t>  generate_wave(uint32_t  sampling_rate, const onch_space&  sp) const noexcept;

  void  output(onch_output_context&  ctx) const noexcept;

  void  print() const noexcept;

};


class
onch_definition
{
  std::string  m_name;

  onch_element  m_element;

public:
  onch_definition() noexcept{}
  onch_definition(const std::string&  name, onch_element&&  e) noexcept{assign(name,std::move(e));}

  const std::string&  get_name() const noexcept{return m_name;}

  onch_definition&  assign(const std::string&  name, onch_element&&  e) noexcept
  {
    m_name    = name;
    m_element = std::move(e);

    return *this;
  }

  const onch_element&  get_element() const noexcept{return m_element;}

};


struct
onch_sound_spec
{
  int  sampling_rate;
  int      bit_depth;

  double  volume;

};


class
onch_space
{
  std::vector<onch_definition>  m_definition_table;

  onch_element  read_text(token_block_view  tbv) noexcept;
  onch_element  read_cell(sound_kind  sk, token_block_view  tbv) noexcept;
  onch_element  read_table(onch_table_kind  tk, token_block_view  tbv) noexcept;

  onch_element  read_element(const std::string&  keyword, token_block_view  tbv) noexcept;
  onch_definition  read_define(token_block_view&  tbv) noexcept;

public:
  onch_space() noexcept{}

  void  clear() noexcept;

  const onch_element*  find(const std::string&  name) const noexcept;

  void  load_from_file(const char*  filepath) noexcept;
  void  load_from_string(const char*  s) noexcept;

  std::vector<gbstd::f32_t>  make_f32_raw_binary(int  sampling_rate, double  volume) const noexcept;

  std::vector<int16_t>  make_16bit_raw_binary(int  sampling_rate, double  volume) const noexcept;
  std::vector<uint8_t>   make_8bit_raw_binary(int  sampling_rate, double  volume) const noexcept;

  std::vector<uint8_t>  make_wave_format_binary(onch_sound_spec  spec) const noexcept;

  void  print() const noexcept;

};




}




#endif




