#include <fstream>
#include <iostream>
#include <cstddef>
#include <cstdlib> 
#include <cstdio>
#include <map> 
#include <string>
#include <vector>
#include <string.h>

using namespace std;
/* data structure */
#define BNUM 5
#define BSIZE 64*1024

// validならキャッシュ済み、validでないならディスクから読みvalidをセット
#define B_VALID (1<<0)
// dirtyならdiskに書き出し、dirtyはクリア
#define B_DIRTY (1<<1)

typedef struct buffer {
    uint8_t id;
    uint8_t dev_no;
    uint8_t blk_no;
    uint16_t refcnt;
    uint8_t flags;
    uint8_t data[BSIZE];
    struct buffer *next;
    struct buffer *prev;
} BUF;

/* function */
void binit();
BUF* bget(uint8_t dev_no, uint8_t blk_no);
BUF* bread(uint8_t dev_no, uint8_t blk_no);
void bwrite(BUF *bp);
void brel(BUF *bp);

void show_buf();

/* global variable */
BUF *buf_head = NULL;

/* code */
void show_buf() {
    cout << "hoge" << endl;
    printf("buf_head:%p\n", buf_head);

    for (BUF *bp = buf_head->next; bp != buf_head; bp = bp->next) {
        printf("id    :%5lu\n", bp->id);
        printf("dev_no:%5lu\n", bp->dev_no);
        printf("blk_no:%5lu\n", bp->blk_no);
        printf("here:%p\n", bp);
        printf("next:%p\n", bp->next);
        printf("prev:%p\n", bp->prev);
    }
}

void binit() {
    BUF *bp = NULL;
    BUF **first = &buf_head;
    for (size_t i = 0; i < BNUM + 1; i++){
        cout << i << endl;
        BUF *new_buf = (BUF *)new BUF[1];
        new_buf->id = i;
        new_buf->flags = 0;
        if (*first == NULL) {
            *first = new_buf;
            bp = new_buf;
            new_buf->next = new_buf;
            new_buf->prev = new_buf;
        } else {
            bp->next = new_buf;
            new_buf->next = (*first);
            new_buf->prev = bp;
            bp = new_buf;
        }
    }
}

BUF* bget(uint8_t dev_no, uint8_t blk_no) {
    // 先頭から探します
    for (BUF *bp = buf_head->next; bp != buf_head; bp = bp->next) {
        if (bp->dev_no == dev_no && bp->blk_no == blk_no) {
            bp->refcnt++;
            return bp;
        }
    }

    // なかった
    // うしろからさがします（LRU意識）
    for (BUF *bp = buf_head->prev; bp != buf_head; bp = bp->prev) {
        if ((bp->flags & B_DIRTY) == 0 && bp->refcnt == 0) {
            bp->dev_no = dev_no;
            bp->blk_no = blk_no;
            bp->refcnt = 1;
            bp->flags = 0;
            return bp;
        }
    }

    // あまりなし
    exit(-1);
}

BUF* bread(uint8_t dev_no, uint8_t blk_no) {
    BUF *bp = bget(dev_no, blk_no);
    if (bp->flags & B_VALID == 0) {
        // read from disk
    }
    return bp;
}

void bwrite(BUF *bp) {
    if (bp->flags & B_DIRTY) {
        bp->flags &= ~B_DIRTY;
        // write to disk
    }
    bp->flags |= B_VALID;
}

void brel(BUF *bp) {
    bp->refcnt--;
    // LRU
    if (bp->refcnt == 0) {
        bp->prev->next = bp->next;
        bp->next->prev = bp->prev;
        bp->next = buf_head->next;
        bp->prev = buf_head;
        buf_head->next->prev = bp;
        buf_head->next = bp;
    }
}

int main (int argc, char* argv[]) {
    binit();
    show_buf();
    return 0;
}