#include"libgbstd/parser.hpp"




namespace gbstd{




token
tokenizer::
read_binary_number() noexcept
{
  uint64_t  n = 0;

  update_info();

    for(;;)
    {
      auto  c = *m_pointer;

        if((c == '0') ||
           (c == '1'))
        {
          n <<= 1;

            if(c == '1')
            {
              n |= 1;
            }


          ++m_pointer;
        }

      else
        {
          break;
        }
    }


  return token(m_info,n,'b');
}


token
tokenizer::
read_octal_number() noexcept
{
  uint64_t  n = 0;

  update_info();

    for(;;)
    {
      auto  c = *m_pointer;

        if((c >= '0') &&
           (c <= '7'))
        {
          n <<= 2;
          n  |= c-'0';

          ++m_pointer;
        }

      else
        {
          break;
        }
    }


  return token(m_info,n,'o');
}


token
tokenizer::
read_decimal_number() noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *m_pointer;

        if((c >= '0') &&
           (c <= '9'))
        {
          n *=    10;
          n += c-'0';

          ++m_pointer;
        }

      else
        {
          break;
        }
    }


    if(*m_pointer == '.')
    {
      ++m_pointer;

      auto  i = static_cast<double>(n);

      double  f = 0.1;

        for(;;)
        {
          auto  c = *m_pointer;

            if((c >= '0') &&
               (c <= '9'))
            {
              i += f*(c-'0');
              f /= 10;

              ++m_pointer;
            }

          else
            {
              break;
            }
        }


      return token(m_info,i);
    }


  return token(m_info,n,'d');
}


token
tokenizer::
read_hexadecimal_number() noexcept
{
  uint64_t  n = 0;

  update_info();

    for(;;)
    {
      auto  c = *m_pointer;

        if(((c >= '0') && (c <= '9')) ||
           ((c >= 'a') && (c <= 'f')) ||
           ((c >= 'A') && (c <= 'F')))
        {
          n <<= 4;

            switch(c)
            {
          case('0'):
          case('1'):
          case('2'):
          case('3'):
          case('4'):
          case('5'):
          case('6'):
          case('7'):
          case('8'):
          case('9'):
              n |= c-'0';
              break;
          case('a'):
          case('A'):
              n |= 0xA;
              break;
          case('b'):
          case('B'):
              n |= 0xB;
              break;
          case('c'):
          case('C'):
              n |= 0xC;
              break;
          case('d'):
          case('D'):
              n |= 0xD;
              break;
          case('e'):
          case('E'):
              n |= 0xE;
              break;
          case('f'):
          case('F'):
              n |= 0xF;
              break;
            }


          ++m_pointer;
        }

      else
        {
          break;
        }
    }


  return token(m_info,n,'x');
}


token
tokenizer::
read_number_that_begins_by_zero() noexcept
{
  auto  c = *m_pointer++;

  return ((c == 'b') || (c == 'B'))?      read_binary_number()
        :((c == 'o') || (c == 'O'))?       read_octal_number()
        :((c == 'x') || (c == 'X'))? read_hexadecimal_number()
        : token(m_info,0,'d');
}


token
tokenizer::
read_number() noexcept
{
  return (*m_pointer == '0')? read_number_that_begins_by_zero()
                            : read_decimal_number();
}


}




