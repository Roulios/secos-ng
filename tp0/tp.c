/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

static const char *mmap_type_to_str(uint32_t type) {
   switch (type) {
      case MULTIBOOT_MEMORY_AVAILABLE:
         return "MULTIBOOT_MEMORY_AVAILABLE";
      case MULTIBOOT_MEMORY_RESERVED:
         return "MULTIBOOT_MEMORY_RESERVED";
      case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
         return "MULTIBOOT_MEMORY_ACPI_RECLAIMABLE";
      case MULTIBOOT_MEMORY_NVS:
         return "MULTIBOOT_MEMORY_NVS";
      default:
         return "UNKNOWN";
   }
}

void tp() {
   debug("kernel mem [0x%p - 0x%p]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);

   multiboot_memory_map_t* entry = (multiboot_memory_map_t*)info->mbi->mmap_addr;
   while((uint32_t)entry < (info->mbi->mmap_addr + info->mbi->mmap_length)) {
      // TODO print "[start - end] type" for each entry
      // pour calcul borne sup de l'intervalle faire -1 à la longueur

      debug("[0x%llx - 0x%llx] %s\n" , entry->addr , (entry->addr+entry->len-1), mmap_type_to_str(entry->type));
      entry++;
   }

   // Tentative d'écriture dans la mémoire disponible
   int *ptr_in_available_mem;
   ptr_in_available_mem = (int*)0x0;
   debug("Available mem (0x0): before: 0x%x ", *ptr_in_available_mem); // read
   *ptr_in_available_mem = 0xaaaaaaaa;                           // write
   debug("after: 0x%x\n", *ptr_in_available_mem);                // check

   // Tentative d'écriture dans la mémoire réservée
   int *ptr_in_reserved_mem;
   ptr_in_reserved_mem = (int*)0xf0000;
   debug("Reserved mem (at: 0xf0000):  before: 0x%x ", *ptr_in_reserved_mem); // read
   *ptr_in_reserved_mem = 0xaaaaaaaa;                           // write
   debug("after: 0x%x\n", *ptr_in_reserved_mem);                // check

   // Tentative d'écriture en dehors de la mémoire physique (au delà de 128 MB)
   int *ptr_outside_mem;
   ptr_outside_mem = (int*)0x80f0000;
   debug("Outside mem (at: 0x8000000):  before: 0x%x ", *ptr_outside_mem); // read
   *ptr_outside_mem = 0xaaaaaaaa;                           // write
   debug("after: 0x%x\n", *ptr_outside_mem);                // check
}
