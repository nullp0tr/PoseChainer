#ifndef PoseChainer_h
#define PoseChainer_h

#include "Arduino.h"

bool pushRecordBackwards(uint8_t record_size, uint8_t *chain_record, bool check_if_record_is_full);

bool insertToRecord(uint8_t record_size, uint8_t *chain_record, uint8_t pose_number);

void gEmptyRecord(uint8_t record_size, uint8_t *chain_record);

struct sVibeChain
{
    uint8_t vibe_level;
    int8_t found_chain;
};

class Chain
{
public:
    Chain(uint8_t *pose_chain, uint8_t chain_size);

    Chain();

    uint8_t *_pose_chain;

    uint8_t _pose_chain_size;

private:

};


class PoseChainer
{
public:
    bool addChain(uint8_t pose_chain[], uint8_t chain_size);

    void insertPoseToChainRecord(uint8_t pose_number);

    sVibeChain whichChain();

    void printChains();

    void printRecord();

    void emptyRecord();

    PoseChainer(uint8_t CHAIN_LIMIT, uint8_t RECORD_LIMIT);

private:
    uint8_t _num_of_chains;

    bool _allowed;

    uint8_t _record_size;

    uint8_t _chain_limit;

    uint8_t *_chain_record;

    Chain *_chains;
};

#endif
