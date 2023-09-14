#include "Global.h"
#include "Global.h"
GitRepository init(char path[],bool force){
    force = false;
    GitRepository gitRepository;
    gitRepository.worktree = path;
    gitRepository.gitdir = path;
    strcat(gitRepository.gitdir,"/.git");

    if (!(force || access(gitRepository.gitdir,F_OK)==0)){
        printf("ERROR:Not a Git repository %s",path);
    }

    /* ---------------------------- 读取.git/config配置文件 --------------------------- */
    char* cf = repo_file( gitRepository,"config",false);
    if(cf && access(cf,F_OK)==0){

    }
    else if (!force){
        printf("Configuration file missing");
    }

    if(!force){
        //TODO 读取版本号
        
    }
}

char* repo_path(GitRepository repo,char path[]){
    char* repoPath = repo.gitdir;
    strcat(repoPath,path);
    return repoPath;
}

char* repo_file(GitRepository repo,char path[],bool mkdir){
    mkdir = false;
    //返回目标文件路径
    if(repo_dir(repo,path,mkdir)){
        return repo_path(repo,path);
    }
}

bool exists(char* path){
    struct stat buffer;
    int exist = stat(path,&buffer);
    if(exist == 0)
        return true;
    else  
        return false;
}

void set_repo_default_config(GitRepository* repo){
    char* config_path = (*repo).config;
    FILE* config_fp = NULL;
    config_fp = fopen(config_path,"w+");
    if(config_fp == NULL){
        assert("配置文件打开失败");
    }
    else{
        fprintf(config_fp,"[core]\n");
        fprintf(config_fp,"\trepositoryformatversion = 0\n");
        fprintf(config_fp,"\tfilemode = false\n");
        fprintf(config_fp,"\tbare = false\n");
    }
}

bool isFolder(char* path){
    struct stat buf;
    int res = stat(path,&buf);
    if(S_IFDIR & buf.st_mode) return true;
    return false; 
}

//创建多级目录
void mkdirs(char *muldir) 
{
    int i,len;
    char str[512];    
    strncpy(str, muldir, 512);
    len=strlen(str);
    for( i=0; i<len; i++ )
    {
        if( str[i]=='/' )
        {
            str[i] = '\0';
            if( access(str,0)!=0 )
            {
                mkdir( str);
            }
            str[i]='/';
        }
    }
    if( len>0 && access(str,0)!=0 )
    {
        mkdir( str);
    }
    return;
}

bool isEmptyFolder(char* path){
    DIR * dir;
    struct dirent * ptr;
    int i;
    dir = opendir (path);
    if ((ptr = readdir (dir)) != NULL) {
        return false;
    }
    closedir (dir);
    return true;
}

char* repo_dir(GitRepository repo,char path[],bool mkdir){
    mkdir = false;
    path = repo_path(repo,path);

    if(exists(path)){
        if(access(repo.gitdir,F_OK)==0) return path;
        else {
            printf("ERROR:Not a directory %s ",path);
        }
    }

    if(mkdir){
        mkdirs(path);
        return path;
    }

    return NULL;
}

GitRepository repo_create(char path[]){
    GitRepository repo = init(path,true);

    if(exists(path)){
        if(!isFolder(repo.worktree)){
            printf("%s is not a directory!" ,path);
        }
        if(exists(repo.gitdir)){
            printf("%s is not empty!" , path);
        }
    }

}