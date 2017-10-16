#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void print_lstat(const char* );
void if_dir(DIR* );
void display_file_type(int );
void display_permisions(int );
char* formatdate(char*, time_t );
int is_dir(const char* );

int main(int argc, char **argv)
{
    if (argc == 1) {
        DIR *dir = opendir(".");
        if_dir(dir);
        closedir(dir);
    } else {
        int i;
        for (i = 1; i < argc; ++i) {
            if (is_dir(argv[i]) == 0) continue;
            print_lstat(argv[i]);
        }
    }
    return 0;
}

void if_dir(DIR *dir) {
    char *file_name;
    struct dirent *curr_file_in_dir; //struct with info about directory
    while ((curr_file_in_dir = readdir(dir)) != NULL) {
        file_name = curr_file_in_dir->d_name;
        print_lstat(file_name);
    }
}

void print_lstat(const char* file_name)
{
    struct stat buf;
    
    if (lstat(file_name, &buf) == -1 ) {
        perror("file not exist");
        return;
    }
    if (is_dir(file_name) == 0) return;
    display_file_type(buf.st_mode);
    display_permisions(buf.st_mode);
    printf(" %lu ", buf.st_nlink); // hard link counter

    struct passwd  *pw_d;
    pw_d = getpwuid(buf.st_uid); //Get user name(from UID) into struct passwf.pw_name
    printf("%s ",pw_d->pw_name); // User Name

    pw_d = getpwuid(buf.st_gid); // Make for GID same as for UID
    printf("%s ",pw_d->pw_name); // Group Name

    printf("%ld ",buf.st_size); //file size
    char date_buf[36];
    printf("%s ", formatdate(date_buf, buf.st_mtime));
    printf("%s\n",file_name);

}

void display_file_type ( int st_mode )
{                                  
    switch ( st_mode & S_IFMT )
    {
        case S_IFDIR:  putchar ( 'd' ); return;
        case S_IFCHR:  putchar ( 'c' ); return;
        case S_IFBLK:  putchar ( 'b' ); return;
        case S_IFREG:  putchar ( '-' ); return;
        case S_IFLNK:  putchar ( 'l' ); return;
        case S_IFSOCK: putchar ( 's' ); return;
    }
}

// Display permissions by format: rwxrwxrwx
void display_permisions(int st_mode)
{
    printf( (st_mode & S_IRUSR) ? "r" : "-");
    printf( (st_mode & S_IWUSR) ? "w" : "-");
    printf( (st_mode & S_IXUSR) ? "x" : "-");
    printf( (st_mode & S_IRGRP) ? "r" : "-");
    printf( (st_mode & S_IWGRP) ? "w" : "-");
    printf( (st_mode & S_IXGRP) ? "x" : "-");
    printf( (st_mode & S_IROTH) ? "r" : "-");
    printf( (st_mode & S_IWOTH) ? "w" : "-");
    printf( (st_mode & S_IXOTH) ? "x" : "-");
}

char* formatdate(char* str, time_t val)
{
        strftime(str, 36, "%b %d %H:%M", localtime(&val));
        return str;
}

// Check that file is a directory
int is_dir(const char* name)
{
    DIR *dir = opendir(name);
    if (dir) {
        closedir(dir);
        return 1;
    } else {
        return 0;
    }
}
