#ifndef PoseChainer_h
#define PoseChainer_h

#define POSE_LIMIT 10
#define CHAIN_LIMIT 10
//#define NULL nullptr
#define RECORD_LIMIT 3
#define POSE_TIMEOUT 4000


bool pushRecordBackwards(int record_size, int *chain_record, bool check_if_record_is_full);
bool insertToRecord(int record_size, int *chain_record, int pose_number);
void gEmptyRecord(int record_size, int* chain_record);

struct sVibeChain {
    volatile short vibe_level;
    volatile short found_chain;
};

class Chain
{
public:
    Chain(int *pose_chain, int chain_size);
    Chain();
    int* _pose_chain;
    int _pose_chain_size;

private:

};


class PoseChainer
{
public:
    void addChain(int pose_chain[], int chain_size);
    void insertPoseToChainRecord(int pose_number);
    sVibeChain whichChain();
    void printChains();
    void printRecord();
    void emptyRecord();

    PoseChainer(long CHAIN_TIME_OUT, long RECORD_TIME_OUT);

private:
    Chain _chains[CHAIN_LIMIT];
    int _num_of_chains;
    long _last_bingo_time;
    long _BINGO_TIME_OUT;
    long _RECORD_TIME_OUT;
    bool _allowed;
    int _record_size = RECORD_LIMIT;
    int _chain_record[RECORD_LIMIT];

};

#endif
