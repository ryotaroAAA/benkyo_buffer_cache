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
typedef enum {
    busy,
    dwrite,
    old
} buf_stat_t;

typedef struct buffer {
    uint8_t id;
    uint8_t dev_no;
    uint8_t blk_no;
    buf_stat_t stat;
    struct buffer *free_next;
    struct buffer *free_prev;
    struct buffer *hash_next;
    struct buffer *hash_prev;
} BUF;

/* function */
void create_pool(BUF **first, size_t size);
void show_buf(BUF *first);

BUF* getblk(uint8_t dev_no, uint8_t blk_no);
void brelese(BUF *buf);

/* global variable */
BUF *bfree_list = NULL;
BUF hash_que[5] = {};

/* code */
void show_buf(BUF *first) {
    BUF *temp = first;
    int count = 0;
    while (1) {
        printf("id    :%5lu\n", temp->id);
        printf("dev_no:%5lu\n", temp->dev_no);
        printf("blk_no:%5lu\n", temp->blk_no);
        printf("free_next:%p\n", temp);
        printf("free_next:%p\n", temp->free_next);
        printf("free_prev:%p\n", temp->free_prev);
        if (temp->free_next == first) {
            break;
        }
        temp = temp->free_next;
    }
}

void create_buf_pool(BUF **first, size_t size) {
    BUF *temp = NULL;
    for (size_t i = 0; i < size; i++){
        BUF *new_buf = (BUF *)new BUF[1];
        new_buf->id = i;
        cout << i << endl;
        if (*first == NULL) {
            *first = new_buf;
            temp = new_buf;
            new_buf->free_next = new_buf;
            new_buf->free_prev = new_buf;
            new_buf->free_next = new_buf;
            new_buf->free_prev = new_buf;
        } else {
            temp->free_next = new_buf;
            temp->hash_next = new_buf;
            new_buf->free_next = (*first);
            new_buf->hash_next = (*first);
            new_buf->free_prev = temp;
            new_buf->hash_prev = temp;
            temp = new_buf;
        }
    }
}

int main (int argc, char* argv[]) {
    create_buf_pool(&bfree_list, 5);
    show_buf(bfree_list);
    return 0;
}