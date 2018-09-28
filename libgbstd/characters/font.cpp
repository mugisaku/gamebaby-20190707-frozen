#include"libgbstd/character.hpp"




namespace gbstd{


namespace{


struct
glyph
{
  uint16_t  unicode;

  uint16_t  data[g_font_height];

};


const glyph
g_glyphs[] =
{
#include"libgbstd/characters/font_data.txt"
};


const uint16_t*
g_table[0x10000] = {0};


struct
initializer
{
  initializer() noexcept;

} init;


initializer::
initializer() noexcept
{
    for(auto&  gly: g_glyphs)
    {
      g_table[gly.unicode] = gly.data;
    }
}
}


const uint16_t*
get_glyph_data(char16_t  c) noexcept
{
  return g_table[c];
}


}




