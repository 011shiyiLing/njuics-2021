#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t serial_write(const void *buf, size_t offset, size_t len); 
size_t events_read(void *buf, size_t offset, size_t len);
//size_t dispinfo_read(void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read; // 读函数指针
  WriteFn write; // 写函数指针
  ////为每一个已经打开的文件引入偏移量属性open_offset, 来记录目前文件操作的位置. 每次对文件读写了多少个字节, 偏移量就前进多少
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here"); //panic:系统发现无法继续运行下去的故障时将调用它，会导致程序中止，然后由系统显示错误号
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/events", 0, 0, events_read, invalid_write},
  //{"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},//获取屏幕大小
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

//通过open()系统调用来打开一个文件, 并返回相应的文件描述符
int fs_open(const char *pathname,int flags,int mode)
{
  int n = (sizeof(file_table)/sizeof(file_table[0]));
  int i;
  for(i=0;i<n;i++)
  {
    if(strcmp(file_table[i].name,pathname) == 0)
    {
      file_table[i].open_offset = 0;
      return i;
      Log("File open:%s\n",pathname);
    }
  }
  assert(0);
}

size_t fs_write(int fd,const void *buf,size_t len)
{
  size_t count;
  /*if(fd == 1 || fd == 2) //stdout stderr
  {
    count = len;
    for(int i=0; i<len; i++)
    {
      putch(*(char *)buf);
      buf++;
    }
  }*/
  //特殊文件
  if(file_table[fd].write != NULL)
  {
    count = file_table[fd].write(buf,0,len);
    file_table[fd].open_offset += count;
  }
  else
  {
    if(file_table[fd].size - file_table[fd].open_offset <= len) assert(0);
    count = len;
    ramdisk_write(buf,file_table[fd].disk_offset+file_table[fd].open_offset,count);
    file_table[fd].open_offset += count;
  }

  Log("File write:%s\n",file_table[fd].name);
  return count;
}

size_t fs_read(int fd,void *buf,size_t len)
{ 
  size_t count;
  if(file_table[fd].read != NULL) //stdin stderr
  {
    count = file_table[fd].read(buf,0,len);
    file_table[fd].open_offset += count;
  }
  else
  {
    if(file_table[fd].open_offset + len <= file_table[fd].size)
    {
      count = len;
    }
    else
    {
      count = file_table[fd].size - file_table[fd].open_offset;
    }

    ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,count);
    file_table[fd].open_offset += count;
  }

  //Log("File read:%s\n",file_table[fd].name);
  return count;
}

//调整偏移量
size_t fs_lseek(int fd,size_t offset,int whence)
{
  switch (whence)
  {
  case SEEK_SET:
    file_table[fd].open_offset = offset;
    break;
  case SEEK_CUR:
    file_table[fd].open_offset += offset;
    break;
  case SEEK_END:
    file_table[fd].open_offset = file_table[fd].size + offset;
    break;
  default:
    break;
  }
  Log("File lseek:%s\n",file_table[fd].name);
  return file_table[fd].open_offset;
}

int fs_close(int fd)
{
  file_table[fd].open_offset = 0;
  Log("File close:%s\n",file_table[fd].name);
  return 0;
}
