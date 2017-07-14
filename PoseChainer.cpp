#include "PoseChainer.h"
#include <stdio.h>

//#include "Arduino.h"

/*******************************************************/
/*******************************************************/
//----------------PoseChainManager---------------------//
/*******************************************************/
/*******************************************************/
/*******************************************************/
PoseChainer::PoseChainer(long CHAIN_TIME_OUT, long RECORD_TIME_OUT) {
    _num_of_chains = 0;
    _last_bingo_time = 0;
    _BINGO_TIME_OUT = CHAIN_TIME_OUT;
    _RECORD_TIME_OUT = RECORD_TIME_OUT;
    _allowed = true;
    for (int i = 0; i < _record_size; i++) {
        _chain_record[i] = 0;
    }

}


void PoseChainer::addChain(int *pose_chain, int chain_size) {
    _chains[_num_of_chains] = Chain(pose_chain, chain_size);
    _num_of_chains += 1;
}


void PoseChainer::insertPoseToChainRecord(int pose_number) {
    pushRecordBackwards(_record_size, _chain_record, 1);
    insertToRecord(_record_size, _chain_record, pose_number);
}


Chain::Chain() {}


Chain::Chain(int *pose_chain, int chain_size) {
    _pose_chain_size = chain_size;
    _pose_chain = pose_chain;
}

sVibeChain PoseChainer::whichChain() {
    sVibeChain vibeChain;
    vibeChain.vibe_level = 0;
    vibeChain.found_chain = -1;

    for (short i = 0; i < _num_of_chains; i++) {

        for (short index_jump = 0; index_jump < _num_of_chains; index_jump++) {

            sVibeChain tempVibeChain;
            tempVibeChain.vibe_level = 0;
            tempVibeChain.found_chain = -1;

            for (short j = index_jump; j < _record_size; j++) {

                if (_chain_record[j] != 0) {
                    if (_chain_record[j] != _chains[i]._pose_chain[j - index_jump]) {
                        tempVibeChain.found_chain = -1;
                        tempVibeChain.vibe_level = 0;
                        //break loop
                        j = _record_size;

                    } else {
                        tempVibeChain.found_chain = i;
                        ++tempVibeChain.vibe_level;

                    }
                }
            }

            if (tempVibeChain.vibe_level >= vibeChain.vibe_level) {
                vibeChain.vibe_level = tempVibeChain.vibe_level;
                vibeChain.found_chain = tempVibeChain.found_chain;
            }
        }
    }
    return vibeChain;
}

void PoseChainer::printChains() {
    printf("Print chains:\n");
    for (int i = 0; i < _num_of_chains; i++) {
        for (int j = 0; j < _chains[i]._pose_chain_size; j++) {
            printf("%d\n", _chains[i]._pose_chain[j]);
        }
        printf("************\n");
    }
}

void PoseChainer::printRecord() {
    printf("Print record:\n");
    for (int i = 0; i < _record_size; i++) {
        printf("%d\n", _chain_record[i]);
    }
    printf("************\n");

}

void PoseChainer::emptyRecord() {
    gEmptyRecord(_record_size, _chain_record);
}


bool pushRecordBackwards(int record_size, int *chain_record, bool check_if_record_is_full) {
    if (check_if_record_is_full) {
        for (int i = 0; i < record_size; ++i) {
            if (chain_record[i] == 0) {
                return 0;
            }
        }
        pushRecordBackwards(record_size, chain_record, 0);
        return 1;
    } else {
        int i = 0;
        for (; i < record_size - 1;) {
            chain_record[i] = chain_record[i + 1];
            ++i;
        }
        chain_record[i] = 0;
        return 1;
    }
}

bool insertToRecord(int record_size, int *chain_record, int pose_number) {

    for (int i = 0; i < record_size; i++) {
        if (chain_record[i] == 0) {
            chain_record[i] = pose_number;
            return 0;
        }
    }
    return 1;
}

void gEmptyRecord(int record_size, int *chain_record) {
    for (int i = 0; i < record_size; i++) {
        chain_record[i] = 0;
    }
}
