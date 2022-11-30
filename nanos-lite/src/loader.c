#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
size_t ramdisk_read(void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  Elf_Ehdr ehdr;
  Elf_Phdr phdr; //programming header table
  ramdisk_read(&ehdr,0,sizeof(ehdr));
  ramdisk_read(&phdr,ehdr.e_ehsize,sizeof(phdr)*ehdr.e_phnum);
  assert(*(uint32_t *)ehdr.e_ident == 0x7f454c46); // check magic number

  for(int i=0; i<ehdr.e_phnum;i++)
  {
    if(phdr.p_type != PT_LOAD) continue;
    ramdisk_read((void *)(phdr.p_vaddr),phdr.p_offset,phdr.p_memsz);//
    memset((void *)(phdr.p_vaddr + phdr.p_filesz),0,phdr.p_memsz-phdr.p_filesz);//将[VirtAddr + FileSiz, VirtAddr + MemSiz)对应的物理区间清零.
  }

  //return 0;
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

