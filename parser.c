#include "Global.h"
extern void parser(int argc, char* argv[]){
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
		if(strcmp(argv[3], "blob") == 0){

		}
		else if(strcmp(argv[3],"commit") ==0){
			
		}
		else if(strcmp(argv[3],"tag") ==0){

		}
		else if(strcmp(argv[3],"tree" ) ==0 ){

		}
		else{//未指定类型
			printf("未指定文件类型！\n");
			return;
		}
      }
      else{//未指定对象
        printf("未指定对象！\n");
        return;
      }
    }
    else if(strcmp(argv[1],"hash-object") == 0){
        int arg_len = argc - 2;//得到hash-object 命令的参数个数
        if(arg_len < 1){//文件名都没有
            printf("未输入文件名！\n");
            return;
        }
        else{
          int i;
            for(i = 2;i<2 + arg_len;i++){
                if(strcmp(argv[i],"-w") == 0){//实际上要写入
                    
                }
                else if(strcmp(argv[i],"-t")){//指定类型
                    i++;
                    if(strcmp(argv[i],"blob") == 0){//类型blob

                    }
                    else if(strcmp(argv[i],"commit") == 0){//类型为commit

                    }
                    else if(strcmp(argv[i],"tree") == 0){//类型为tree

                    }
                    else if(strcmp(argv[i],"tag") == 0){//类型为tag

                    }
                    else{//未指定类型，默认为blob
                        
                		return;
                    }
                }

            }
        }
    }
    else{
      printf("'%s' is not a command. See 'main.exe --help'. \n",argv[1]);
      return;
    }
  }
}