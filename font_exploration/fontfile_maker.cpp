#include <math.h>
#include <hb.h>
#include <hb-ft.h>
#include <cairo.h>
#include <cairo-ft.h>
#include <iostream>



void make_font(const char *text,  const int fontSize)
{
    const char *fontfile = "DejaVuSansMono.ttf";

    std::cout << "aaa" << std::endl;

  
}

int main(int argc, char **argv)
{
  
  if (argc < 3)
  {
    throw std::runtime_error("Too few arguments.");
    
  }

  make_font(argv[1], 36);


}
