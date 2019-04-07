#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




namespace{
bool
isnum(int  c) noexcept
{
  return (c == '?') || ((c >= '0') && (c <= '8')) ;
}


struct
decoder
{
  char  symbol;

  char   first_spec;
  char  second_spec;

  char   first_number;
  char  second_number;

  decoder(const char*&  ptr) noexcept;

};


decoder::
decoder(const char*&  ptr) noexcept
{
  symbol = 0;

   first_spec = onch_word::specs::no_spec;
  second_spec = onch_word::specs::no_spec;

   first_number = 0;
  second_number = 0;

    for(;;)
    {
      auto  c = *ptr++;

        if(c == 0)
        {break;}

      else
        if((c == 'p') ||
           (c == 'r') ||
           (c == 'v') ||
           (c == 'f'))
        {
          symbol = c         ;
                   c = *ptr++;

            if(isnum(c))
            {
              first_spec = (c == '?')? onch_word::specs::no_spec
                          :(c == '0')? onch_word::specs::zero
                          :            onch_word::specs::index;

                if(first_spec == onch_word::specs::index)
                {
                  first_number = c-'1';
                }


              c = *ptr++;

                if(isnum(c))
                {
                  second_spec = (c == '?')? onch_word::specs::no_spec
                               :(c == '0')? onch_word::specs::zero
                               :            onch_word::specs::index;


                    if(second_spec == onch_word::specs::index)
                    {
                      second_number = c-'1';
                    }
                }
            }


          break;
        }
    }
}


onch_word
read_word(const char*  ptr, const char*  end) noexcept
{
  onch_word  w;

    for(;;)
    {
      decoder  dec(ptr);

        if(dec.symbol == 'p')
        {
          w.set_l(dec.first_spec,dec.first_number);
        }

      else
        if(dec.symbol == 'r')
        {
          w.set_rest_flag();

          w.set_l(dec.first_spec,dec.first_number);
        }

      else
        if(dec.symbol == 'v')
        {
          w.set_v( dec.first_spec, dec.first_number,
                  dec.second_spec,dec.second_number);
        }

      else
        if(dec.symbol == 'f')
        {
          w.set_f( dec.first_spec, dec.first_number,
                  dec.second_spec,dec.second_number);
        }

      else
        {
          break;
        }
    }


  return w;
}
}


onch_element
onch_space::
read_text(token_block_view  tbv) noexcept
{
  onch_text  txt;

    while(tbv)
    {
        if(tbv[0].is_double_quoted() || tbv[0].is_single_quoted())
        {
          auto&  s = tbv[0].get_string();

          ++tbv;

          txt.push(read_word(s.data(),s.data()+s.size()));
        }

      else
        if(tbv[0].is_operator_code("*") &&
           tbv[1].is_identifier())
        {
          auto&  id = tbv[1].get_string();

          tbv += 2;

          auto  e = find(id);

            if(e)
            {
                   if(e->is_text()){txt.push(e->get_text());}
              else if(e->is_word()){txt.push(e->get_word());}
            }
        }

      else
        {
          ++tbv;
        }
    }


  return onch_element(std::move(txt));
}


onch_element
onch_space::
read_cell(sound_kind  sk, token_block_view  tbv) noexcept
{
  onch_cell  cel(sk);

    while(tbv)
    {
        if(tbv[0].is_identifier())
        {
          auto&  id = tbv[0].get_string();
                
            if(tbv[1].is_block("{","}"))
            {
              auto  e = read_element(id,tbv[1].get_block());

              tbv += 2;

                if(e && e.is_text())
                {
                  cel.push(e.get_text());
                }
            }

          else
            {
              ++tbv;
            }
        }

      else
        if(tbv[0].is_operator_code("*") &&
           tbv[1].is_identifier())
        {
          auto&  id = tbv[1].get_string();

          tbv += 2;

          auto  e = find(id);

            if(e)
            {
                   if(e->is_text()){cel.push(e->get_text());}
              else if(e->is_word()){cel.push(e->get_word());}
            }
        }

      else
        {
          ++tbv;
        }
    }


  return onch_element(std::move(cel));
}


onch_element
onch_space::
read_table(onch_table_kind  tk, token_block_view  tbv) noexcept
{
  onch_table  tbl(tk);

    while(tbv)
    {
        if(tbv[0].is_identifier())
        {
          auto&  id = tbv[0].get_string();
                
            if(tbv[1].is_block("{","}"))
            {
              auto  e = read_element(id,tbv[1].get_block());

              tbv += 2;

                if(e)
                {
                  tbl.push(std::move(e));
                }
            }

          else
            {
              ++tbv;
            }
        }

      else
        if(tbv[0].is_operator_code("*") &&
           tbv[1].is_identifier())
        {
          auto&  id = tbv[1].get_string();

          tbv += 2;

          auto  e = find(id);

            if(e && (e->is_table() || e->is_cell()))
            {
              tbl.push(onch_element(*e));
            }
        }

      else
        {
          ++tbv;
        }
    }


  return onch_element(std::move(tbl));
}


onch_element
onch_space::
read_element(const std::string&  keyword, token_block_view  tbv) noexcept
{
       if(keyword ==      "square"){return read_cell(sound_kind::square_wave,tbv);}
  else if(keyword ==       "noise"){return read_cell(sound_kind::noise      ,tbv);}
  else if(keyword == "short_noise"){return read_cell(sound_kind::short_noise,tbv);}
  else if(keyword ==         "row"){return read_table(onch_table_kind::row   ,tbv);}
  else if(keyword ==      "column"){return read_table(onch_table_kind::column,tbv);}
  else if(keyword ==        "text"){return read_text(tbv);}
  else
    {
      printf("%s is unknown keyword\n",keyword.data());
    }


  return onch_element();
}


onch_definition
onch_space::
read_define(token_block_view&  tbv) noexcept
{
    if(tbv[0].is_identifier() &&
       tbv[1].is_operator_code("="))
    {
        if(tbv[2].is_identifier() &&
           tbv[3].is_block("{","}"))
        {
          auto&       id = tbv[0].get_string();
          auto&  keyword = tbv[2].get_string();

          auto  deftbv = token_block_view(tbv[3].get_block());

          tbv += 4;

          auto  e = read_element(keyword,deftbv);

            if(e)
            {
              return onch_definition(id,std::move(e));
            }
        }

      else
        if(tbv[2].is_operator_code("*") &&
           tbv[3].is_identifier())
        {
          auto&  id = tbv[3].get_string();

          tbv += 4;

          auto  e = find(id);

            if(e)
            {
              return onch_definition(id,onch_element(*e));
            }
        }
    }

  else
    {
      ++tbv;
    }


  return onch_definition();
}




void
onch_space::
clear() noexcept
{
  m_definition_table.clear();
}




const onch_element*
onch_space::
find(const std::string&  name) const noexcept
{
    for(auto&  def: m_definition_table)
    {
        if(def.get_name() == name)
        {
          return &def.get_element();
        }
    }


  return nullptr;
}


void
onch_space::
load_from_file(const char*  filepath) noexcept
{
  auto  s = make_string_from_file(filepath);

  load_from_string(s.data());
print();
}


void
onch_space::
load_from_string(const char*  s) noexcept
{
  tokenizer  tknzr;

  auto  blk = tknzr(s);

  token_block_view  tbv(blk);

    while(tbv)
    {
      auto  def = read_define(tbv);

        if(def.get_element())
        {
          m_definition_table.emplace_back(std::move(def));
        }

      else
        {
          break;
        }
    }
}


std::vector<int16_t>
onch_space::
make_16bit_raw_binary() const noexcept
{
  std::vector<int16_t>  buf;

  auto  e = find("main");

    if(e)
    {
      auto  wave_data = e->generate_wave(*this);

      buf.resize(wave_data.size());

      auto  src     = wave_data.data();
      auto  src_end = wave_data.data()+wave_data.size();
      auto  dst     = buf.data();

        while(src != src_end)
        {
          auto  v = (*src++)*32767.0;

          *dst++ = static_cast<int16_t>(v);
        }
    }


  return std::move(buf);
}


std::vector<uint8_t>
onch_space::
make_8bit_raw_binary() const noexcept
{
  std::vector<uint8_t>  buf;

  auto  e = find("main");

    if(e)
    {
      auto  wave_data = e->generate_wave(*this);

      buf.resize(wave_data.size());

      auto  src     = wave_data.data();
      auto  src_end = wave_data.data()+wave_data.size();
      auto  dst     = buf.data();

        while(src != src_end)
        {
          auto  v = (*src++)*127.0;

          *dst++ = static_cast<uint8_t>(v+128);
        }
    }


  return std::move(buf);
}


std::vector<uint8_t>
onch_space::
make_wave_format_binary(int  number_of_bits_per_sample) const noexcept
{
  auto&  bps = number_of_bits_per_sample;

  gbstd::wave_format  fmt;

  fmt.set_sampling_rate(gbstd::g_number_of_samples_per_second);
  fmt.set_number_of_bits_per_sample(bps);
  fmt.set_number_of_channels(1);

  fmt.update();

    if(bps == 8)
    {
      auto  raw_bin = make_8bit_raw_binary();

        if(raw_bin.size())
        {
          gbstd::wave  wav(raw_bin.data(),raw_bin.size(),fmt);

          return wav.to_binary();
        }
    }

  else
    if(bps == 16)
    {
      auto  raw_bin = make_16bit_raw_binary();

        if(raw_bin.size())
        {
          gbstd::wave  wav(raw_bin.data(),raw_bin.size(),fmt);

          return wav.to_binary();
        }
    }


  return {};
}


void
onch_space::
print() const noexcept
{
    for(auto&  def: m_definition_table)
    {
      printf("%s = ",def.get_name().data());

      def.get_element().print();

      printf("\n");
    }


  printf("\n");
}




}




