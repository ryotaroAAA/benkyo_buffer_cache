#include <fstream>
#include <iostream>
#include <cstddef>
#include <cstdlib> 
#include <cstdio>
#include <map> 
#include <string>
#include <vector>
#include <string.h>

typedef struct buffer {
    uint8_t dev_no;
    uint8_t blk_no;
    uint8_t stat;
    struct buffer *free_next;
    struct buffer *free_prev;
    struct buffer *hash_next;
    struct buffer *hash_prev;
} BUF;

#define B_BUSY (1<<0)
#define B_DWRITE (1<<1)
#define B_OLD (1<<2)

void createPool(BUF **buf, int hoge);
void display(BUF *buf);
void display1(BUF *buf);

BUF* getblk(uint8_t dev_no, uint8_t blk_no);
void brelese(BUF *buf);

int main (int argc, char* argv[]) {
    printf("hello world!");
}