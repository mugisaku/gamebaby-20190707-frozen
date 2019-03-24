#ifndef gbstd_onch_hpp_is_included
#define gbstd_onch_hpp_is_included


#include"libgbstd/utility.hpp"
#include"libgbstd/sound.hpp"
#include"libgbstd/parser.hpp"
#include<memory>


namespace gbstd{




class onch_element;
class onch_text;
class onch_table;
class onch_definition;
class onch_parameter;
class onch_space;


struct
onch_output_context
{
  const onch_space*  m_space;

  f32_t*   m_it;
  f32_t*  m_end;

  std::vector<const onch_text*>   m_text_table;
  std::vector<const onch_table*>  m_table_table;

};


class
onch_word
{
  uint16_t  m_data;

public:
  constexpr onch_word(int  l=0, int  v=0, int  f=0) noexcept: m_data((l<<8)|(v<<4)|f){}

  constexpr int  get_length_index() const noexcept{return (m_data>>8);}
  constexpr int  get_volume_index() const noexcept{return (m_data>>4)&15;}
  constexpr int  get_frequency_index() const noexcept{return (m_data&15);}

  uint32_t  get_length(const onch_space&  sp) const noexcept;

  void  output(sound_kind  k, onch_output_context&  ctx) const noexcept;

  void  print() const noexcept;

};


class
onch_text
{
  std::vector<onch_word>  m_words;

public:
  onch_text() noexcept{}

  void  push(onch_word  w) noexcept{m_words.emplace_back(w);}

  uint32_t  get_length(const onch_space&  sp) const noexcept;

  void  output(sound_kind  k, onch_output_context&  ctx) const noexcept;

  void  print() const noexcept;

};


class
onch_cell
{
  sound_kind  m_kind;

  std::vector<onch_text>  m_texts;

public:
  onch_cell(sound_kind  k) noexcept: m_kind(k){}

  void  push(const onch_text&  txt) noexcept{m_texts.emplace_back(txt);}

  uint32_t  get_length(const onch_space&  sp) const noexcept;

  void  output(onch_output_context&  ctx) const noexcept;

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

  uint32_t  get_length(const onch_space&  sp) const noexcept;

  void  output(onch_output_context&  ctx) const noexcept;

  void  print() const noexcept;

};


class
onch_element
{
  enum class kind{
    null,
    text,
    cell,
    table,

  } m_kind=kind::null;

  union data{
    onch_text    txt;
    onch_cell    cel;
    onch_table   tbl;
    std::string   id;

     data(){}
    ~data(){}

  } m_data;

public:
  onch_element() noexcept{}
  onch_element(const onch_element&   rhs) noexcept{assign(rhs);}
  onch_element(      onch_element&&  rhs) noexcept{assign(std::move(rhs));}
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
  onch_element&  assign(onch_cell&&       cel) noexcept;
  onch_element&  assign(onch_text&&       txt) noexcept;
  onch_element&  assign(onch_table&&       tbl) noexcept;

  bool  is_cell()  const noexcept{return m_kind == kind::cell;}
  bool  is_text()  const noexcept{return m_kind == kind::text;}
  bool  is_table() const noexcept{return m_kind == kind::table;}

  const onch_text&    get_text()  const noexcept{return m_data.txt;}
  const onch_cell&    get_cell()  const noexcept{return m_data.cel;}
  const onch_table&   get_table() const noexcept{return m_data.tbl;}

  uint32_t  get_length(const onch_space&  sp) const noexcept;

  std::vector<f32_t>  generate_wave(const onch_space&  sp) const noexcept;

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
onch_parameter
{
  uint32_t  m_length_table[16];
  double    m_volume_table[16];
  double    m_frequency_table[16];

};


class
onch_space
{
  onch_parameter  m_parameter;

  std::vector<onch_definition>  m_definition_table;

  onch_element  read_text(token_block_view  tbv) noexcept;
  onch_element  read_cell(sound_kind  sk, token_block_view  tbv) noexcept;
  onch_element  read_table(onch_table_kind  tk, token_block_view  tbv) noexcept;

  onch_element  read_element(const std::string&  keyword, token_block_view  tbv) noexcept;
  onch_definition  read_define(token_block_view&  tbv) noexcept;

public:
  onch_space() noexcept{}
  onch_space(const onch_parameter&  pr) noexcept: m_parameter(pr){}

  const onch_parameter&  get_parameter() const noexcept{return m_parameter;}

  const onch_definition*  find(const std::string&  name) const noexcept;

  void  load_from_file(const char*  filepath) noexcept;

  void  print() const noexcept;

};




}




#endif




