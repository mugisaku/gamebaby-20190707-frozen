#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




void
onch_table::
push(const onch_definition&  def) noexcept
{
    if(def.is_sound() || def.is_table())
    {
        if(is_row())
        {
               if(def.is_sound()){m_length += def.get_sound().get_length();}
          else if(def.is_table()){m_length += def.get_table().get_length();}
        }

      else
        if(is_column())
        {
               if(def.is_sound()){m_length = std::max(m_length,def.get_sound().get_length());}
          else if(def.is_table()){m_length = std::max(m_length,def.get_table().get_length());}
        }


      m_elements.emplace_back(&def);
    }
}


void
onch_table::
reset() noexcept
{
  m_elements.clear();

  m_length = 0;
}


void
onch_table::
reset(onch_table_kind  k) noexcept
{
  m_kind = k;

  reset();
}




namespace{
uint32_t
tmpl_output(const onch_sound&  t, f32_t*  it, f32_t*  end) noexcept
{
  t.output(it,end);

  return gbstd::get_number_of_samples_by_time(t.get_length());
}
uint32_t
tmpl_output(const onch_table&  t, f32_t*  it, f32_t*  end, std::vector<const onch_table*>&  entry_list) noexcept
{
    for(auto  ptr: entry_list)
    {
        if(ptr == &t)
        {
          return 0;
        }
    }


  t.output(it,end,entry_list);

  return gbstd::get_number_of_samples_by_time(t.get_length());
}
}


void
onch_table::
output(f32_t*  it, f32_t*  end, std::vector<const onch_table*>&  entry_list) const noexcept
{
  entry_list.emplace_back(this);

    if(is_row())
    {
        for(auto  def: m_elements)
        {
            if(def->is_sound())
            {
              it += tmpl_output(def->get_sound(),it,end);
            }

          else
            if(def->is_table())
            {
              it += tmpl_output(def->get_table(),it,end,entry_list);
            }
        }
    }

  else
    if(is_column())
    {
        for(auto  def: m_elements)
        {
            if(def->is_sound())
            {
              tmpl_output(def->get_sound(),it,end);
            }

          else
            if(def->is_table())
            {
              tmpl_output(def->get_table(),it,end,entry_list);
            }
        }
    }


  entry_list.pop_back();
}


std::vector<f32_t>
onch_table::
generate_wave() const noexcept
{
  auto  n = gbstd::get_number_of_samples_by_time(get_length());

  std::vector<gbstd::f32_t>  result(n);

  f32_t*  ptr = result.data();

  std::vector<const onch_table*>  entry_list;

  output(ptr,ptr+n,entry_list);

  return std::move(result);
}


void
onch_table::
print() const noexcept
{
    switch(m_kind)
    {
  case(onch_table_kind::row):
      printf("row{");

        for(auto  def: m_elements)
        {
          def->print();

          printf(" ");
        }

      printf("}");
      break;
  case(onch_table_kind::column):
      printf("column{");

        for(auto  def: m_elements)
        {
          def->print();

          printf("\n");
        }

      printf("}");
      break;
    }
}




}




