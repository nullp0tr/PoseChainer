#ifndef PoseChainer_h
#define PoseChainer_h

#define POSE_LIMIT 10
#define CHAIN_LIMIT 10
#define RECORD_LIMIT 3
#define POSE_TIMEOUT 4000

#include <stdint.h>

bool pushRecordBackwards(uint8_t record_size, uint8_t *chain_record, bool check_if_record_is_full);

bool insertToRecord(uint8_t record_size, uint8_t *chain_record, uint8_t pose_number);

void gEmptyRecord(uint8_t record_size, uint8_t *chain_record);

struct sVibeChain {
    volatile uint8_t vibe_level;
    volatile int8_t found_chain;
};

class Chain {
public:
    Chain(uint8_t *pose_chain, uint8_t chain_size);

    Chain();

    uint8_t *_pose_chain;
    uint8_t _pose_chain_size;

private:

};


class PoseChainer {
public:
    void addChain(uint8_t pose_chain[], uint8_t chain_size);

    void insertPoseToChainRecord(uint8_t pose_number);

    sVibeChain whichChain();

    void printChains();

    void printRecord();

    void emptyRecord();

    PoseChainer(long CHAIN_TIME_OUT, long RECORD_TIME_OUT);

private:
    Chain _chains[CHAIN_LIMIT];
    uint8_t _num_of_chains;
    long _last_bingo_time;
    long _BINGO_TIME_OUT;
    long _RECORD_TIME_OUT;
    bool _allowed;
    uint8_t _record_size = RECORD_LIMIT;
    uint8_t _chain_record[RECORD_LIMIT];

};

#endif
