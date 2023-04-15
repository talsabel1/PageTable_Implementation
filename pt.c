#include "os.h"
#define MASK 0x1FF000000000
#define LEVELS 5
#define ZERO 0

/* function declarations */

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn);

uint64_t page_table_query(uint64_t pt, uint64_t vpn);

uint64_t is_allocated(uint64_t pte);

uint64_t pt_row(uint64_t vpn);


/* functions */

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {
    int i;
    uint64_t phys_frame;
    uint64_t key;
    uint64_t *virt_pt_head = phys_to_virt(pt << 12);

    for (i = 0; i < LEVELS; i++) {
        key = pt_row(vpn);
        vpn = vpn << 9;

        if (!is_allocated(*(virt_pt_head + key))) {
            if (ppn == NO_MAPPING) {
                return;
            }
            phys_frame = alloc_page_frame();
            *(virt_pt_head + key) = (phys_frame << 12) + 1;
        }
        virt_pt_head = phys_to_virt(*(virt_pt_head + key)  - 1);
    }

    key = pt_row(vpn);

    if (ppn == NO_MAPPING) {
        *(virt_pt_head + key) = ZERO;
    }
    else {
        *(virt_pt_head + key) = (ppn << 12) + 1;
    }
}


uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
    int i;
    uint64_t ppn;
    uint64_t *virt_pt_head = phys_to_virt(pt << 12);
    uint64_t key;

    for (i = 0; i < LEVELS; i++) {
        key = pt_row(vpn);
        vpn = vpn << 9;

        if (!is_allocated(*(virt_pt_head + key))) {
            return NO_MAPPING;
        }

        virt_pt_head = phys_to_virt(*(virt_pt_head + key) - 1);
    }

    key = pt_row(vpn);

    if (!is_allocated(*(virt_pt_head + key))) {
        return NO_MAPPING;
    }

    ppn = *(virt_pt_head + key) >> 12;
    return ppn;
}

/* Checks if the pte has been previously allocated and is still valid (i.e. the valid bit is 1).
 * returns true if yes, false otherwise. */
uint64_t is_allocated(uint64_t pte) {
    return ((pte & 1) != ZERO);
}

/* Calculates the relevant part of the vpn for the current PT level */
uint64_t pt_row(uint64_t vpn) {
    uint64_t row = vpn & MASK;
    return (row >> 36);
}
