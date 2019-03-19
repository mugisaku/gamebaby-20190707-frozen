#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




namespace{
std::unique_ptr<onch_definition>
read_frequency(const std::string&  id, const token&  tok) noexcept
{
  return tok.is_integer()?               std::make_unique<onch_definition>(id,onch_frequency(tok.get_integer()))
        :tok.is_floating_point_number()? std::make_unique<onch_definition>(id,onch_frequency(tok.get_floating_point_number()))
        :nullptr;
}
std::unique_ptr<onch_definition>
read_volume(const std::string&  id, const token&  tok) noexcept
{
  return tok.is_integer()?               std::make_unique<onch_definition>(id,onch_volume(tok.get_integer()))
        :tok.is_floating_point_number()? std::make_unique<onch_definition>(id,onch_volume(tok.get_floating_point_number()))
        :nullptr;
}
std::unique_ptr<onch_definition>
read_length(const std::string&  id, const token&  tok) noexcept
{
  return tok.is_integer()?               std::make_unique<onch_definition>(id,onch_length(tok.get_integer()))
        :tok.is_floating_point_number()? std::make_unique<onch_definition>(id,onch_length(tok.get_floating_point_number()))
        :nullptr;
}
}




std::unique_ptr<onch_definition>
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

           if(keyword ==      "volume"){return read_volume(     id,deftbv[0]);}
      else if(keyword ==   "frequency"){return read_frequency(  id,deftbv[0]);}
      else if(keyword ==      "length"){return read_length(     id,deftbv[0]);}
      else if(keyword ==      "square"){return read_sound(id,sound_kind::square_wave,deftbv);}
      else if(keyword ==       "noise"){return read_sound(id,sound_kind::noise      ,deftbv);}
      else if(keyword == "short_noise"){return read_sound(id,sound_kind::short_noise,deftbv);}
      else if(keyword ==         "row"){return read_table(id,onch_table_kind::row   ,deftbv);}
      else if(keyword ==      "column"){return read_table(id,onch_table_kind::column,deftbv);}
      else
        {
          printf("%s is unknown keyword\n",keyword.data());
        }
    }


  return nullptr;
}


std::unique_ptr<onch_definition>
onch_space::
read_sound(const std::string&  id, sound_kind  sk, token_block_view  tbv) noexcept
{
  onch_sound  snd(sk);

    while(tbv)
    {
      auto&  tok = *tbv++;

        if(tok.is_identifier())
        {
          auto  def = find(tok.get_string().data());

            if(def)
            {
                   if(def->is_frequency()){snd.set(def->get_frequency());}
              else if(def->is_volume()   ){snd.set(def->get_volume());}
              else if(def->is_length()   ){snd.set(def->get_length());}
            }
        }
    }


  return std::make_unique<onch_definition>(id,snd);
}


std::unique_ptr<onch_definition>
onch_space::
read_table(const std::string&  id, onch_table_kind  tk, token_block_view  tbv) noexcept
{
  onch_table  tbl(tk);

    while(tbv)
    {
      auto&  tok = *tbv++;

        if(tok.is_identifier())
        {
          auto  def = find(tok.get_string().data());

            if(def)
            {
              tbl.push(*def);
            }
        }
    }


  return std::make_unique<onch_definition>(id,std::move(tbl));
}




const onch_definition*
onch_space::
find(const char*  name) const noexcept
{
    for(auto&  defptr: m_definition_table)
    {
        if(defptr->get_name() == name)
        {
          return defptr.get();
        }
    }


  return nullptr;
}


void
onch_space::
load_from_file(const char*  filepath) noexcept
{
  auto  s = make_string_from_file(filepath);

  tokenizer  tknzr;

  auto  blk = tknzr(s.data());

  token_block_view  tbv(blk);

    while(tbv)
    {
      auto  ptr = read_define(tbv);

        if(ptr)
        {
          m_definition_table.emplace_back(std::move(ptr));
        }
    }
}




void
onch_space::
print() const noexcept
{
    for(auto&  ptr: m_definition_table)
    {
        if(ptr->get_name().size())
        {
          printf("%s = ",ptr->get_name().data());
        }


      ptr->print();

      printf("\n");
    }
}




}




