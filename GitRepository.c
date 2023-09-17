#include "Global.h"
GitRepository init(char path[],bool force){
    GitRepository gitRepository;
    strcpy(gitRepository.worktree,path);

    strcpy(gitRepository.gitdir,path);
    strcat(gitRepository.gitdir,"/.git");

    strcpy(gitRepository.config,gitRepository.gitdir);
    strcat(gitRepository.config,"/config");
    // if (!(force || isFolder(gitRepository.gitdir))){
    //     printf("ERROR:Not a Git repository %s",path);
    //     return;
    // }

    /* ---------------------------- 读取.git/config配置文件 --------------------------- */
    // char* cf = repo_file( gitRepository,"config",false);
    // if(cf && access(cf,F_OK)==0){

    // }
    // else if (!force){
    //     printf("Configuration file missing");
    //     return;
    // }

    // if(!force){
    //     //TODO 读取版本号
        
    // }
    return gitRepository;
}

void to_repo_path(char repoPath[],GitRepository repo,char path[]){
    printf("repo.gitdir: %s\npath:%s\n",repo.gitdir,path);
    memcpy(repoPath,repo.gitdir,sizeof(repo.gitdir));
    strcat(repoPath,"/");
    strcat(repoPath,path);
    mkdirs(repoPath);
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
    FILE* config_fp = NULL;
    printf("config: %s\n",repo->config);
    config_fp = fopen(repo->config,"w+");
    if(config_fp == NULL){
        printf("配置文件打开失败");
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
    //避免把最后的路径也变成文件夹
    /*if( len>0 && access(str,0)!=0 )
    {
        mkdir( str);
    }*/
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

void repo_create(GitRepository* repo,char path[]){
    *repo = init(path,true);
    if(exists((*repo).worktree)){
        if(!isFolder((*repo).worktree)){
            printf("%s is not a directory!" ,path);
            return ;
        }
        if(exists((*repo).gitdir) && isEmptyFolder((*repo).gitdir)){
            printf("%s is not empty!" , path);
            return;
        }
    }
    else{
        mkdirs((*repo).worktree);
    }
    //TODO assert

    //写.git/description文件
    FILE* des_fp = NULL;
    char des_path[30];
    to_repo_path(des_path,(*repo),"description");
    printf("%s\n",des_path);
    des_fp = fopen(des_path,"w+");
    if(des_fp == NULL){
        printf("ERROR:description file open failed.");
        return;
    }
    fprintf(des_fp,"Unnamed repository; edit this file 'description' to name the repository.\n");
    fclose(des_fp);

    //写./git/HEAD文件
    FILE* head_fp = NULL;
    char head_path[30];
    to_repo_path(head_path,(*repo),"HEAD");
    printf("%s\n",head_path);
    head_fp = fopen(head_path,"w+");
    if(head_fp == NULL){
        printf("ERROR:HEAD file open failed.");
        return;
    }
    fprintf(head_fp,"ref: refs/heads/master\n");
    fclose(head_fp);

    //写./git/config文件
    FILE* conf_fp = NULL;
    char conf_path[30];
    to_repo_path(conf_path,(*repo),"config");
    printf("%s\n",conf_path);
    conf_fp = fopen(conf_path,"w+");
    if(conf_fp == NULL){
        printf("ERROR:config file open failed.");
        return;
    }
    printf("repo->config %s\n",repo->config);
    set_repo_default_config(repo);
    fclose(conf_fp);
}

//将路径转为系统绝对路径
void fullPath(char s[MAX_PATH]){
    DWORD len = GetFullPathName(".",MAX_PATH,s,NULL);

    if(len == 0){
        printf("GetFullPathName failed: %d\n",GetLastError());
        return;
    }
}

void repo_find(char real_path[MAX_PATH]){
    fullPath(real_path);
    printf("%s",real_path);
    char real_path_plus_git[MAX_PATH];
    memcpy(real_path_plus_git,real_path,sizeof real_path);
    strcat(real_path_plus_git,"/.git");
    if(isFolder(real_path_plus_git))
        return;
    //如果没有返回，说明.git应该在父级或更高的目录中，继续递归
    char parent[MAX_PATH];
    memcpy(parent,real_path_plus_git,sizeof real_path_plus_git);
    strcat(parent,"../");
    fullPath(parent);
    if(strcmp(parent,real_path)==0){
        //若父目录与当前目录相同，说明当前已经是根目录并且没有.git文件夹
        printf("No git directory.");
        return;
    }

    //否则继续递归

    repo_find(parent);

}

// 构建模式串的部分匹配表
void buildPartialMatchTable(const char *pattern, int *partialMatchTable) {
    int len = 0;  // 当前部分匹配长度
    int i = 1;

    partialMatchTable[0] = 0;  // 第一个字符的部分匹配长度总是0

    while (pattern[i]) {
        if (pattern[i] == pattern[len]) {
            len++;
            partialMatchTable[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = partialMatchTable[len - 1];
            } else {
                partialMatchTable[i] = 0;
                i++;
            }
        }
    }
}

// KMP算法主函数，用于在文本中查找模式串
int KMP(const char *text, const char *pattern) {
    int textLen = strlen(text);
    int patternLen = strlen(pattern);

    // 构建部分匹配表
    int *partialMatchTable = (int*)malloc(sizeof(int) * patternLen);
    buildPartialMatchTable(pattern, partialMatchTable);

    int i = 0;  // 在文本中的当前位置
    int j = 0;  // 在模式串中的当前位置

    while (i < textLen) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == patternLen) {
            // 找到匹配，返回 
            free(partialMatchTable);
            return i-j;
            j = partialMatchTable[j - 1];
        } else if (i < textLen && pattern[j] != text[i]) {
            if (j != 0) {
                j = partialMatchTable[j - 1];
            } else {
                i++;
            }
        }
    }
    free(partialMatchTable);
    return -1;
}

void object_read(GitRepository* repo,char sha[]){
    char path[MAX_PATH];//path:对象文件的路径
    char tc = sha[2];
    sha[2] = '\0';
    to_repo_path(path,*repo,"objects");
    to_repo_path(path,*repo,sha);
    sha[2]= tc;
    to_repo_path(path,*repo,sha+2);

    if(!exists(path)){
        printf("object doesn't exist!\n");
        return;
    }

    FILE* ob_fp;
    if((ob_fp = (path,"rb")) == NULL){
        printf(" %s File open failed!\n",path);
        return;
    };

    // 读取文件内容
    fseek(ob_fp, 0, SEEK_END);
    long fileSize = ftell(ob_fp);
    fseek(ob_fp, 0, SEEK_SET);

    char *rawData = (char *)malloc(fileSize);
    if (rawData == NULL) {
        perror("Failed to allocate memory");
        fclose(ob_fp);
        return;
    }

    if (fread(rawData, 1, fileSize, ob_fp) != fileSize) {
        perror("Failed to read file");
        free(rawData);
        fclose(ob_fp);
        return;
    }

    fclose(ob_fp);

    // 解压缩数据
    char *decompressedData = NULL;
    uLong decompressedSize = 0;

    if (uncompress((Bytef *)decompressedData, &decompressedSize, (const Bytef *)rawData, fileSize) != Z_OK) {
        perror("Failed to decompress data");
        free(rawData);
        free(decompressedData);
        return;
    }

    // 解析格式和大小信息
    char *fmt = strtok(decompressedData, " ");
    char *sizeStr = strtok(NULL, "\x00");
    int size = atoi(sizeStr);

    // 验证数据大小
    if (size != decompressedSize - (sizeStr - decompressedData) - 1) {
        printf("Malformed object: bad length\n");
        free(rawData);
        free(decompressedData);
        return;
    }    
    
    if(strcmp(fmt,"commit")==0){

    }
    else if(strcmp(fmt,"tree")==0){

    }
    else if(strcmp(fmt,"tag") == 0){

    }
    else if(strcmp(fmt,"blob")==0){

    }
    else{
        printf("Unknown type %s for ovject %s",fmt,sha);
    }

}

// 序列化对象
void serialize(Object *obj, char **serializedData, size_t *serializedSize) {
    size_t fmtLen = strlen(obj->fmt);
    size_t dataSize = strlen(obj->data);

    *serializedSize = fmtLen + 1 + dataSize + 1;  // +1 用于空格和 '\x00'

    *serializedData = (char *)malloc(*serializedSize);
    snprintf(*serializedData, *serializedSize, "%s %zu\x00%s", obj->fmt, dataSize, obj->data);
}

// 计算 SHA-1 哈希
void computeSHA1(const char *data, size_t dataSize, char *sha1Hash) {
    SHA_CTX sha1Context;
    SHA1_Init(&sha1Context);
    SHA1_Update(&sha1Context, data, dataSize);
    SHA1_Final(sha1Hash, &sha1Context);
}

void object_write(Object obj,GitRepository* repo,bool isRepo){
    char* serialized_data;
    size_t serialized_size;
    //序列化对象数据
    serialize(&obj,&serialized_data,&serialized_size);

    //加标头
    char *data_len = malloc(sizeof(char) * 100);//求数据长度的数字的长度
    itoa(atoi(serialized_data),data_len,10);
    int result_len = strlen(obj.fmt) + 1 + strlen(data_len) + 1 + strlen(serialized_data);
    free(data_len);  
    char *result = malloc(sizeof(char) * (result_len + 25));

    //计算哈希值
    char sha1Hash[SHA_DIGEST_LENGTH];
    computeSHA1(result,strlen(result),sha1Hash);
    char hexSha1Hash[2 * SHA_DIGEST_LENGTH + 1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(&hexSha1Hash[i * 2], "%02x", sha1Hash[i]);
    }
    hexSha1Hash[2 * SHA_DIGEST_LENGTH] = '\0';

    if(isRepo){
        //计算路径
        char path[MAX_PATH];
        to_repo_path(path,*repo,"objects");
        char t_c = hexSha1Hash[2];
        hexSha1Hash[2]='\0';
        to_repo_path(path,*repo,hexSha1Hash);
        hexSha1Hash[2]=t_c;
        to_repo_path(path,*repo,hexSha1Hash+2);
    }
}