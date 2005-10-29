#include <free.h>

int main( int argc, char* argv[] )
{
  int code = 0;
  code = vm_free( argc, argv );
  return code;
}
