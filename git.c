#include "Global.h"
extern void parser(int argc, char* argv[]);
int main(int argc, char* argv[])
{
  //解析命令
  parser(argc,argv);
  return 0;
}
