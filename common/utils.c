#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <netdb.h>
#include "utils.h"

int file_select(const struct dirent *entry)
{
   if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
     return (0);
   else
     return (1);
}

int proc_select(const struct dirent *entry)
{
   if (entry->d_name[0] < '1')
        return(0);
   if (entry->d_name[0] > '9')
        return(0);
   return (1);
}

/*****************************************************************************
 *   Description: Read contents of file into dynamically allocated buffer
 *    Parameters: File name to read from
 *       Returns: Pointer to Buffer or NULL (1st word, 16bits contain count)
 ****************************************************************************/
read_buf_t *ReadFile(char *FileName)
{
    int readsize = 1024;
    int fileFD;
    int readcount;
    read_buf_t *readbuf = NULL;

    if (FileName  != NULL) {
        if ((fileFD = open(FileName, O_RDONLY))  == -1) {
            return (NULL);
        }
    } else
        return (NULL);
    if (lseek(fileFD, 0, SEEK_SET) != 0) {
        return (NULL);
    }
    readbuf = malloc(readsize + sizeof(read_buf_t));
    memset(readbuf, 0, (readsize + sizeof(int)));
    readcount = read(fileFD, readbuf->buf, readsize);
    while ((readcount == readsize) && (readcount <= 65535)) {
        free(readbuf);
        if (lseek(fileFD, 0, SEEK_SET) != 0)
            return (NULL);
        readsize = 2 * readsize;
        readbuf = malloc(readsize + sizeof(read_buf_t));
        memset(readbuf, 0, (readsize + sizeof(read_buf_t)));
        readcount = read(fileFD, readbuf->buf, readsize);
    }
    close(fileFD);
    if (readcount <= 0) {
        return (NULL);
    } else {
        readbuf->count = readcount;
        return (readbuf);
    }
}

read_line_t *ReadFileLines(char *FileName)
{
    char *str = NULL;
    unsigned int  lc = 0;
    unsigned int  lines_len = sizeof(read_line_t);

    read_buf_t *buf;
    read_line_t *lines;
    char *splitbuf[256];

    if ((buf = ReadFile(FileName)) == (read_buf_t *) 0 ) {
        return NULL;
    }

    for (lc = 0, str = (char *)buf->buf;;lc++, str = NULL) {
        splitbuf[lc] = strtok(str, "\n");
        if (splitbuf[lc] == NULL)
            break;
    }

    /* allocate buffer our lines, for count+ pointer+ pointer[lc] */
    lines_len += (lc * sizeof(char *));
    lines = malloc(lines_len);
    memset(lines, 0, lines_len);
    lines->read_buf = buf;
    for (lc = 0; splitbuf[lc] != (char *) 0; lc++) {
        lines->buf[lc] = malloc(strlen(splitbuf[lc]) + 1);
        /* copy the null byte when we copy the string */
        strncpy(lines->buf[lc], splitbuf[lc], strlen(splitbuf[lc]) + 1);
    }
    lines->count = lc;
    return (lines);
}

#ifdef UNUSED
read_line_t *ReadFilteredFileLines(char *FileName,char *prefix[])
{
    char *str = NULL;
    int  lc,idx;

    read_buf_t *buf;
    read_line_t *lines;
    int lines_len = sizeof(read_line_t);
    char *splitbuf[128];

    if ((buf = ReadFile(FileName)) == (read_buf_t *) 0 ) {
        return 0;
    }

    for (lc=0, str = (char *)buf->buf;;lc++, str = NULL) {
        splitbuf[lc] = strtok(str,"\n");
        if (splitbuf[lc] == NULL)
            break;
    }

    lines_len += (lc * sizeof(char *));
    lines = malloc(lines_len);
    memset(lines, 0, lines_len);
    lines->read_buf = buf;
    lines->count = 0;
    for (lc = 0; splitbuf[lc] != (char *) 0; lc++) {
        idx = 0;
        while (prefix[idx] != (char *) 0) {
            if (strncmp(prefix[idx],splitbuf[lc],strlen(prefix[idx])) == 0) {
                lines->buf[lines->count] = malloc(strlen(splitbuf[lc]) + 1);
                strncpy(lines->buf[lc], splitbuf[lc], strlen(splitbuf[lc]) + 1);
                lines->count++;
            }
            idx++;   
        }
    }
    return (lines);
}

file_tok_t *ReadFilteredFileTokens(char *FileName, char *prefix[],char *s)
{
    line_tok_t *entry;
    file_tok_t *entries;
    read_line_t *lines;
    int idx, tc;
    char *str;
    char *tokens[1000];
    int entry_len = sizeof(line_tok_t);
    int entries_len = sizeof(file_tok_t);
    int lines_len = sizeof(read_line_t);

    lines = ReadFilteredFileLines(FileName, prefix);
    if (lines == (read_line_t *) 0) 
        return 0;

    entries_len += (lines->count * sizeof(char *));
    entries = malloc(entries_len);
    entries->count = lines->count;
    for (idx = 0;  idx < lines->count; idx ++) {
        for (tc = 0, str = lines->buf[idx];;tc++, str = NULL) {
            tokens[tc] = strtok(str,s);
            if (tokens[tc] == NULL)
                break;
        }
        entry_len += (tc * sizeof(line_tok_t));
        entry = malloc(entry_len);
        entry->count = tc;
        for (tc = 0; tc < entry->count; tc++) {
            entry->buf[tc] = malloc(strlen(tokens[tc]) + 1);
            strncpy(entry->buf[tc], tokens[tc], strlen(tokens[tc]) + 1);
        }
        entries->tok[idx] = entry;
    }
    return (entries);
}

#endif
char * get_sysfs_str(char * sysfs_attr)
{
    int fd;
    char *string;
    size_t count;
    char *indx;

    if ((fd = open(sysfs_attr,O_RDONLY)) < 0) 
        return NULL;
    if ((string = malloc(4096)) == NULL)
        return NULL;
    if ((count = read(fd, string, 4096)) <= 0) {
        free(string);
        return NULL;
    } else 
        string = realloc(string, count + 1);

    indx = index(string,'\n');
    if (indx < (string + count))
        *indx = 0;

    close(fd);
    return string;
}

void _get_sysfs_numeric(char * sysfs_attr, char *fmt, void *value)
{
    char *string;

    if ((string = get_sysfs_str(sysfs_attr)) == NULL)
        return ;
    errno = 0;
    sscanf(string, fmt, value);
    free(string);
    return;
}
 
unsigned long get_sysfs_lhex(char * sysfs_attr)
{
    unsigned long  number;
    
    _get_sysfs_numeric(sysfs_attr, "%lx", &number);
    return number;
}

unsigned int get_sysfs_ihex(char * sysfs_attr)
{
    unsigned int  number;
    
    _get_sysfs_numeric(sysfs_attr, "%x", &number);
    return number;
}

unsigned short get_sysfs_shex(char * sysfs_attr)
{
    unsigned short  number;
    
    _get_sysfs_numeric(sysfs_attr, "%hx", &number);
    return number;
}

unsigned char get_sysfs_chex(char * sysfs_attr)
{
    unsigned char  number;
    
    _get_sysfs_numeric(sysfs_attr, "%hhx", &number);
    return number;
}

unsigned int get_sysfs_uint(char * sysfs_attr)
{
    unsigned int number;

    _get_sysfs_numeric(sysfs_attr, "%u", &number);
    return number;
}

unsigned long long get_sysfs_ullong(char * sysfs_attr)
{
    unsigned long long number;

    _get_sysfs_numeric(sysfs_attr, "%lu", &number);
    return number;
}

int get_sysfs_int(char * sysfs_attr)
{
    int number;

    _get_sysfs_numeric(sysfs_attr, "%d", &number);
    return number;
}

long long get_sysfs_llong(char * sysfs_attr)
{
    long long number;

    _get_sysfs_numeric(sysfs_attr, "%ld", &number);

    return number;
}

#ifdef UTEST
        int main(int argc, char **argv)
        {
            char *prefix[] = {"/dev/",0};
            int count, idx;

            file_tok_t *fsentries;
            line_tok_t *fsentry;
            distro_id_t * distro;

            fsentries = ReadFilteredFileTokens("/etc/mtab",prefix,' ');
            if ((distro = getDistroInfo()) != NULL ) {
                fprintf (stderr,"%s\n",distro->LongDistro);
                fprintf (stderr,"%s\n",distro->Vendor);
                fprintf (stderr,"%s\n",distro->Code);
                fprintf (stderr,"%s\n",distro->VersionString);
                fprintf (stderr,"%s\n",distro->Description);
                fprintf (stderr,"%s\n",distro->Role);
                fprintf (stderr,"%d.%d\n",distro->Version,distro->SubVersion);
            } else 
                fprintf(stderr," getDistroInfo() Error\n");
            fprintf(stderr,"hasTelnet() = %d\n",hasTelnet("/proc/net/tcp"));
            fprintf(stderr,"hasTelnet6() = %d\n",hasTelnet("/proc/net/tcp6"));
    
            /*    if (fsentries == (read_line_t *) 0)  
                  fprintf(stderr," number of lines %d \n",0);
                  else
                  fprintf(stderr," number of lines %d \n",fsentries->count);
                  for (count = 0 ;count < fsentries-> count; count++) {
                  fsentry = fsentries->tok[count];
                  for (idx = 0; idx < fsentry->count; idx++) 
                  fprintf(stderr,"%s ",fsentry->buf[idx]);
                  fprintf(stderr,"\n");
                  } 
                  */

        }
#endif