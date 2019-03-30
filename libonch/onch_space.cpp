#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




onch_word
read_word(const char*  s) noexcept
{
  int  li = 0;
  int  vi = 0;
  int  fi = 0;

  const char*  cp = s;
  const char*  np = s;

    for(;;)
    {
      int*  ip = nullptr;

        while(!ip)
        {
          auto  c = *cp++;

               if(!c){goto QUIT;}
          else if(c == 'l'){ip = &li;}
          else if(c == 'v'){ip = &vi;}
          else if(c == 'f'){ip = &fi;}
        }


        for(;;)
        {
          auto  c = *np++;

            if(!c)
            {goto QUIT;}

          else
            if((c >= '1') && (c <= '8'))
            {
              *ip = c-'1';

              break;
            }
        }
    }


QUIT:
  return onch_word(li,vi,fi);
}


onch_element
onch_space::
read_text(token_block_view  tbv) noexcept
{
  onch_text  txt;

    while(tbv)
    {
      auto&  tok = *tbv++;

        if(tok.is_identifier())
        {
          auto&  id = tok.get_string();

          auto  def = find(id);

            if(def)
            {
              auto&  e = def->get_element();

                if(e.is_text())
                {
                  txt.push(e.get_text());
                }
            }

          else
            {
              txt.push(read_word(id.data()));
            }
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
              auto  def = find(id);

              tbv += 1;

                if(def)
                {
                  auto&  e = def->get_element();

                    if(e.is_text())
                    {
                      cel.push(e.get_text());
                    }
                }
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
              auto  def = find(id);

              tbv += 1;

                if(def)
                {
                  tbl.push(onch_element(def->get_element()));
                }
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
//      printf("%s is unknown keyword\n",keyword.data());
    }


  return onch_element();
}


onch_definition
onch_space::
read_define(token_block_view&  tbv) noexcept
{
    if(tbv[0].is_identifier() &&
       tbv[1].is_operator_code("=") &&
       tbv[2].is_identifier() &&
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
    {
      ++tbv;
    }


  return onch_definition();
}




const onch_definition*
onch_space::
find(const std::string&  name) const noexcept
{
    for(auto&  def: m_definition_table)
    {
        if(def.get_name() == name)
        {
          return &def;
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

  auto  def = find("main");

    if(def && def->get_element())
    {
      auto&  e = def->get_element();

      auto  wave_data = e.generate_wave(*this);

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

  auto  def = find("main");

    if(def && def->get_element())
    {
      auto&  e = def->get_element();

      auto  wave_data = e.generate_wave(*this);

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




