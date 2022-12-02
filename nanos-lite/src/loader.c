#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

//check ISA type
#if defined(__ISA_AM_NATIVE__)
# define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_X86__)
# define EXPECT_TYPE EM_X86_64
#elif defined (__ISA_MIPS32__)
# define EXPECT_TYPE EM_MIPS
#elif defined(__ISA_RISCV32__) || defined(__ISA_RISCV64__)
# define EXPECT_TYPE EM_RISCV
#else
# error Unsupported ISA
#endif

// 从ramdisk中`offset`偏移处的`len`字节读入到`buf`中
size_t ramdisk_read(void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  Elf_Ehdr ehdr; //FLF header
  ramdisk_read(&ehdr,0,sizeof(Elf_Ehdr));
  assert(*(uint32_t *)ehdr.e_ident == 0x464c457f); //检查魔数（注意是小端排列）

  Elf_Phdr phdr[ehdr.e_phnum]; //programming header table
  ramdisk_read(phdr,ehdr.e_ehsize,sizeof(Elf_Phdr)*ehdr.e_phnum);

  for(int i=0; i<ehdr.e_phnum;i++)
  {
    if(phdr[i].p_type != PT_LOAD) continue;
    ramdisk_read((void *)(phdr[i].p_vaddr),phdr[i].p_offset,phdr[i].p_memsz);//
    memset((void *)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, (phdr[i].p_memsz-phdr[i].p_filesz));//将[VirtAddr + FileSiz, VirtAddr + MemSiz)对应的物理区间清零.
  }

  //return 0;
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

