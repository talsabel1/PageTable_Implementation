#include "os.h"
#define MASK 0x1FF000000000
#define ZERO 0

// function declarations

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn);

uint64_t page_table_query(uint64_t pt, uint64_t vpn);

// functions

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {
    int i;
    uint64_t phys_frame;
    uint64_t *pageTable = phys_to_virt(pt << 12); // pt is ppn and we need to add 12 bits for offset to pass a phys address
    uint64_t level = vpn & MASK;  // bits 48-56
    level = level >> 36;

    for (i = 0; i < 4; i++) {
        if ((*(pageTable + level) & 1) == ZERO) { // the pte pointer has not been allocated (valid bit = 0)
            if (ppn == NO_MAPPING) { // the va mapping we are trying to remove does not exist so we don't need to destroy and can just exit the func
                return;
            }
            phys_frame = alloc_page_frame(); // alloc space for next level PT
            *(pageTable + level) = (phys_frame << 12) + 1; // +1 to update valid bit
            pageTable = phys_to_virt(*(pageTable + level) - 1); // -1 to remove valid bit and just have frame + zeros
            vpn = vpn << 9;
            level = vpn & MASK;
            level = level >> 36;
        }
    }
    if (ppn == NO_MAPPING) {
        *(pageTable + level) = ZERO;
    }
    else {
        *(pageTable + level) = (ppn << 12) + 1; // +1 to update valid bit
    }

}

uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
    int i;
    uint64_t pte;
    uint64_t ppn;
    uint64_t *pageTable = phys_to_virt(pt << 12); // pt is ppn and we need to add 12 bits for offset to pass a phys address
    uint64_t level = vpn & MASK;  // bits 48-56
    level = level >> 36;

    for (i = 0; i < 5; i++) {
        pte = *(pageTable + level);
        if ((pte & 1) == ZERO) { // the pte pointer has not been allocated (valid bit = 0)
            return NO_MAPPING;
        }
        pageTable = phys_to_virt(pte - 1); // -1 to remove valid bit and just have frame + zeros
        vpn = vpn << 9;
        level = vpn & MASK;
        level = level >> 36;

    }
    ppn = pte >> 12;
    return ppn;
}
