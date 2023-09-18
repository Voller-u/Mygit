#include "Global.h"
int main(int argc, char* argv[])
{
  if (argc > 1) {
    if (strcmp(argv[1], "init") == 0) {
      if (argc > 2) {//指定目录初始化仓库
        GitRepository git;
        repo_create(&git, argv[2]);
      }
      //当前目录初始化仓库
      else {
        //GitRepository git_repository = repo_create(argv[1]);

      }
    }
    else if(strcmp(argv[1],"cat-file") == 0){
      if(argc > 2){//打印原始内容

      }
      else{//未指定对象
        printf("未指定对象！\n");
      }
    }
    else{
      printf("'%s' is not a command. See 'main.exe --help'. \n",argv[1]);
    }
  }
  return 0;
}
