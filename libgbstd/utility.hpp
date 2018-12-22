#ifndef utility_hpp_was_included
#define utility_hpp_was_included


#include<cstdarg>
#include<cstdlib>
#include<cstdio>
#include<cstdint>
#include<random>


#ifndef report
#define report printf("[report %8d] %s, %s, %d\n",gbstd::reporting_counter++,__FILE__,__func__,__LINE__);
#endif




namespace gbstd{


extern int  reporting_counter;


template<typename  T>
inline void
destruct(T&  t) noexcept
{
  t.~T();
}


template<typename  T>
inline T*
duplicate(const T*  src) noexcept
{
  return src? new T(*src):nullptr;
}


template<typename  T>
inline T*
duplicate(const T*  src, size_t  n) noexcept
{
    if(src)
    {
      auto  p = new T[n];

      auto  dst = p;

        while(n--)
        {
          *dst++ = *src++;
        }


      return p;
    }


  return nullptr;
}


bool   is_png(const void*  ptr) noexcept;
bool  is_webp(const void*  ptr) noexcept;


void  printf_with_indent(int  indent, const char*  fmt, ...) noexcept;


class
validity
{
  bool  m_value=true;

public:
  operator bool() const noexcept{return m_value;}

  void   enable() noexcept{m_value =  true;}
  void  disable() noexcept{m_value = false;}

};




#ifdef EMSCRIPTEN
void  show_github_link() noexcept;
void  show_twitter_link() noexcept;
void  set_caption(const char*  s) noexcept;
void  set_description(const char*  s) noexcept;
void  download(const uint8_t*  ptr, size_t  size, const char*  filename) noexcept;
int  get_number_of_dropped_files() noexcept;
std::vector<uint8_t>  pop_front_dropped_file() noexcept;
#endif




extern std::random_device
random_device;


extern std::default_random_engine
random_number_engine;


class
normal_rand
{
  std::normal_distribution<>  m_dist;

public:
  normal_rand() noexcept{}
  normal_rand(double  mean, double  stddev) noexcept{reset(mean,stddev);}

  void  reset(double  mean, double  stddev) noexcept{m_dist = std::normal_distribution<>(mean,stddev);}

  double  operator()() noexcept{return m_dist(random_number_engine);}

};


class
uniform_rand
{
  std::uniform_int_distribution<>  m_dist;

public:
  uniform_rand() noexcept{}
  uniform_rand(int  min, int  max) noexcept{reset(min,max);}

  void  reset(int  min, int  max) noexcept{m_dist = std::uniform_int_distribution<>(min,max);}

  int  operator()() noexcept{return m_dist(random_number_engine);}

};


class
uniform_real_rand
{
  std::uniform_real_distribution<>  m_dist;

public:
  uniform_real_rand() noexcept{}
  uniform_real_rand(double  min, double  max) noexcept{reset(min,max);}

  void  reset(double  min, double  max) noexcept{m_dist = std::uniform_real_distribution<>(min,max);}

  double  operator()() noexcept{return m_dist(random_number_engine);}

};


class
incremental
{
  int  m_value=0;
  int  m_add_amount=1;

public:
  incremental() noexcept{}
  incremental(int  v, int  add=1) noexcept: m_value(v), m_add_amount(add){}

  void  reset(int  v) noexcept{m_value = v;}

  int  operator ++() noexcept
  {
    m_value += m_add_amount;

    return m_value;
  }

  int  operator ++(int) noexcept
  {
    int  v = m_value                ;
             m_value += m_add_amount;

    return v;
  }

  int  operator --() noexcept
  {
    m_value -= m_add_amount;

    return m_value;
  }

  int  operator --(int) noexcept
  {
    int  v = m_value                ;
             m_value -= m_add_amount;

    return v;
  }

};


class
string_form
{
  char  m_buffer[512] = {0};

  int  m_length=0;

public:
  string_form() noexcept{}
  string_form(const char*  fmt, ...) noexcept;

  const char*  operator()(const char*  fmt, ...) noexcept;
  const char*  operator()() const noexcept{return m_buffer;}

  int  size() const noexcept{return m_length;}

};


std::string  make_string_from_file(const char*  filepath) noexcept;


}


#endif




