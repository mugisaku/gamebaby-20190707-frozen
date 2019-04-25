#include"libgbstd/image.hpp"


using namespace gbstd;


int
main(int  argc, char**  argv)
{
  --argc;
  ++argv;

    while(argc--)
    {
      auto  path = *argv++;

      image  img;

      img.load_png(path);

      auto  txts = img.make_txt_stream();

      char  buf[256];

      snprintf(buf,sizeof(buf),"%s.txt",path);

      auto  f = fopen(buf,"wb");

        if(f)
        {
          fwrite(txts.data(),txts.size(),1,f);

          fclose(f);
        }
    }


  return 0;
}




