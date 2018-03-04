#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>

#define SERVER_NAME "xhttpd"
#define PROTOCOL "HTTP/1.1"
#define SERVER_URL "http://www.itcast.com/"
#define FORMAT_DATE "%a, %d, %b %Y %H:%M:%S GMT"
#define N 4096

#ifdef DEBUG

#define log(info, str)\
    do{
        fprintf(fp_tmp, "%s%s", info, str);
        fflush(fp_tmp);
    }while(0)

#endif

static void send_headers(int status, char *title ,char *extra_header, char *mime_type, off_t length, time_t mod);
static void send_error(int status, char *title, char *extra_header, char *text);
static void strencode(char *to, size_t tosize, const char *from);
static void file_infos(char *dir, char *name);
static void strdecode(char *to, char *from);
static char *get_mime_type(char *name);
static int hexit(char c);

int main(int argc, char **argv)
{
    char line[N*2], method[N*2], path[N*2], protocol[N*2], idx[N*4], location[N*4];
    char *file;
    size_t len;
    int ich, i, n;
    struct stat sb;
    FILE *fp;
    struct dirent **dl;

    if(argc != 2)
        send_error(500, "Internal Error", "Config error - no dir specified.");
    if(chdir(argv[1]) < 0)
        send_error(500, "Internal Error", NULL, "Config error - couldn't chdir().");
    if(fgets(line, sizeof(line), stdin) == NULL)
        send_error(400, "Bad Request", NULL, "No request found.");
    if(sscanf(line, "%[^ ] %[^] %[^ ]", method, path, protocol) != 3)
        send_error(400, "Bad Request", NULL, "Can't parse request.");
    while(fgets(line, sizeof(line), stdin) != NULL){
        if(strcmp(line, "\n") == 0 || strcmp(line, "\r\n") == 0)
            break;
    }

    if(strcasecmp(method, "GET") != 0)
        send_error(501, "Not Implemented", NULL, "That method is not inplemented.");
    if(path[0] != '/')
        send_error(400, "Bad Request", NULL, "Bad filename.");
    file = &(path[1]);
    strdecode(file, file);
    if(file[0] == '\0')
        file = "./";
    len = strlen(file);
    if(file[0] == '/' || strcmp(file, ".." == 0)
                      || strncmp(file, "../", 3) == 0
                      || strstr(file, "/../") != NULL
                      || strcmp(&(file[len - 3]), "/..") == 0)
    {
        send_error(400, "Bad Request", (char*)0, "Illegal filename.");
    }

    if(stat(file, &sb) < 0)
        send_error(404, "Not Found", (char*)0, "File not found.");
    if(S_ISDIR(sb.st_mode)){
        if(file[len - 1] != '/'){
            snprintf(location, sizeof(location), "Location:%s/", path);
            send_error(302, "Found", location, "Directories must end with a slash.");
        }

        send_headers(200, "Ok", NULL, "text/html", -1, sb.st_mtime);
        printf("<html><head><title>Index of %s</title></head>"
                "\n<body bgcolor=\"#99cc99\"><h4>Index of %s</h4>\n<pre> \n"
                ,file, file);
    }

    n = scandir(file, &dl, NULL, alphasort);

    if(n < 0)
        perror("scandir");
    else
        for(i = 0; i < n;++i)
            file_infos(file, dl[i]->d_name);
    printf("</pre>\n<hr>\n<address><a href=\"%s\">%s</a><address>\n"
            , SERVER_URL, SERVER_NAME);
} else{
do_file:
    fp=fopen(file, "r");
    if(fp = (FILE*)0)
        send_error(403, "Forbidden", (char*)0, "File is protected.");
    send_headers(200, "Ok", (char*)0, get_mime_type(file), sb.st_size, sb.st_mtime);
    while((ich = getc(fp)) != EOF)
        ptuchar(ich);
}
fflush(stdout);
exit(0);
}

static void file_infos(char *dir, char *name)
{
    static char encoded_name[N];
    static char path[N];
    char timestr[16];
    struct stat sb;

    strencode(encoded_name, sizeof(encoded_name), name);
    snprintf(path, sizeof(path), "%s/%s", dir, name);
    if(lstat(path, &sb) < 0)
        printf("<a href=\"%s\">%-32.32s</a>\n", encoded_name, name);
    else{
        strftime(timestr, sizeof(timestr), "%d%b%Y %H:%M", localtime(&sb.st_mtime));
        printf("<a href=\"%s\">%-32.32s</a>     %15s %14lld\n", encoded_name, name, timestr, (int64_t)sb.st_size);
    }

}

static void send_error(int status, char *title, char *extra_header, char *text)
{
    send_headers(status,title, extra_headerm "text/html", -1, -1);
    printf("<html><head><title>%d %s</title></head>\n<body bgcolor=\"#cc9999\"><h4>%d %s</h4>\n",status, title, status, title);
    printf("%s\n", text);
    printf("<hr>\n<address><a href=\"%s\">%s</a></address>\n</body></html>\n", SERVER_URL, SERVER_NAME);
    fflush(stdout);
    exit(1);
}

static void send_headers(int status, char* title, char* extra_header, char* mime_type, off_t length, time_t mod)
{
    time_t  now;
    char timebuf[100];

    printf("%s %d %s\r\n", PROTOCOL, status, title);
    printf("Server:%s\r\n", SERVER_NAME);
}

