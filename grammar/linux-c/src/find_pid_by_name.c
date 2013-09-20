long* find_pid_by_name( char* pidName)  
{
    DIR *dir;
    struct dirent *next;  
    long* pidList=NULL;  
    int i=0;  
  
    ///proc中包括当前的进程信息,读取该目录
    dir = opendir("/proc");
    if (!dir)
        perror_msg_and_die("Cannot open /proc");  
       
    //遍历  
    while ((next = readdir(dir)) != NULL) {  
        FILE *status;  
        char filename[READ_BUF_SIZE];  
        char buffer[READ_BUF_SIZE];  
        char name[READ_BUF_SIZE];  
  
        /* Must skip ".." since that is outside /proc */
        if (strcmp(next->d_name, "..") == 0)
            continue;  

        /* If it isn't a number, we don't want it */ 
        if (!isdigit(*next->d_name))  
            continue;  

        //设置进程
        sprintf(filename, "/proc/%s/status", next->d_name);
        if (! (status = fopen(filename, "r")) ) {
            continue;
        }  

        if (fgets(buffer, READ_BUF_SIZE-1, status) == NULL) {
            fclose(status);
            continue;
        }

        fclose(status);  

        //得到进程id
        /* Buffer should contain a string like "Name:   binary_name" */
        sscanf(buffer, "%*s %s", name);
        if (strcmp(name, pidName) == 0) {
            pidList=realloc( pidList, sizeof(long) * (i+2));
            pidList[i++]=strtol(next->d_name, NULL, 0);  
        }
    }  

    if (pidList) {  
        pidList[i]=0;  
    }  

    return NULL;
} 

