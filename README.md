# Multi Level Page Tables
Simple simulation of OS code that handles a multi-level (trie-based) page table. 

Functions include:
 - page_table_update (creates/destroys virtual memory mappings per request) 
 - page_table_query (searches if an address is mapped in the page table)

Pre-built main function provides numerous tests.

Compiles cleanly with the following flags: 

     gcc -O3 -Wall -std=c11 os.c pt.c
