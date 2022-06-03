#include <iostream>
#include <queue>
#include <string>
#include <algorithm>
#include <vector>
#include "lab4.h"

using namespace std;

struct PageBlock {
    string page;
    bool r;
    int counter;
};

int PAGES_COUNT = 6;
vector<PageBlock> pageBlocks;
deque<int> indexes;

void flow(int algorithm);

PageBlock initialize_page_block();

void print_page_blocks();

void interrupt();

void second_chance(PageBlock &block);

void nfu(PageBlock &block);

int get_page_idx(string &page_value);

int get_nfu_based_deleting_idx();

int get_nfu_based_deleting_idx() {
    int tmp = pageBlocks[0].counter;
    vector<int> pageCount;
    pageCount.push_back(0);

    for (int i = 1; i < PAGES_COUNT; i++) {
        if (pageBlocks[i].counter < tmp) {
            pageCount.clear();
            pageCount.push_back(i);
            tmp = pageBlocks[i].counter;
        } else if (pageBlocks[i].counter == tmp) {
            pageCount.push_back(i);
        }
    }

    if (pageCount.size() > 1) {
        return pageCount[uniform_rnd(0, int(pageCount.size() - 1))];
    }

    return pageCount[0];
}

int get_page_idx(string &page_value) {
    for (int i = 0; i < pageBlocks.size(); i++) {
        if (pageBlocks[i].page == page_value) {
            return i;
        }
    }

    return -1;
}

void nfu(PageBlock &block) {
    int idx = get_nfu_based_deleting_idx();
    pageBlocks.erase(pageBlocks.begin() + idx);
    pageBlocks.insert(pageBlocks.begin() + idx, block);
}

void second_chance(PageBlock &block) {
    auto headPage = pageBlocks[indexes.front()];
    while (headPage.r) {
        headPage.r = false;
        indexes.push_back(indexes.front());
        indexes.pop_front();
        headPage = pageBlocks[indexes.front()];
    }

    pageBlocks.erase(pageBlocks.begin() + indexes.front());
    pageBlocks.insert(pageBlocks.begin() + indexes.front(), block);
    rotate(indexes.begin(), indexes.begin() + 1, indexes.end());
}

void interrupt() {
    for (auto &i: pageBlocks) {
        i.counter += i.r;
        i.r = false;
    }
}

void print_page_blocks() {
    for (int i = 0; i < PAGES_COUNT; i++) {
        if (i < pageBlocks.size()) {
            cout << pageBlocks[i].page;
        } else {
            cout << "#";
        }
        cout << ((i != PAGES_COUNT - 1) ? " " : "");
    }
    cout << endl;
}

PageBlock initialize_page_block() {
    string action;
    string page;
    cin >> action;
    cin >> page;

    PageBlock block;
    block.page = page;
    block.r = true;
    block.counter = 0;
    return block;
}

void flow(int algorithm) {
    int iteration = 0;
    while (cin) {
        auto block = initialize_page_block();
        if (!cin) {
            break;
        }

        if (iteration % 5 == 0) {
            interrupt();
        }

        if (get_page_idx(block.page) == -1) {
            if (pageBlocks.size() < PAGES_COUNT) {
                pageBlocks.push_back(block);
                indexes.push_back(iteration);
            } else if (algorithm == 1) {
                second_chance(block);
            } else if (algorithm == 2) {
                nfu(block);
            }
        } else {
            pageBlocks[get_page_idx(block.page)].r = true;
        }

        print_page_blocks();
        iteration++;

    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        flow(atoi(argv[1]));
    }

    return 0;
}