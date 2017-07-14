#include "PoseChainer1.h"


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
    for (uint8_t i = 0; i < _record_size; i++) {
        _chain_record[i] = 0;
    }

}


void PoseChainer::addChain(uint8_t *pose_chain, uint8_t chain_size) {
    _chains[_num_of_chains] = Chain(pose_chain, chain_size);
    _num_of_chains += 1;
}


void PoseChainer::insertPoseToChainRecord(uint8_t pose_number) {
    pushRecordBackwards(_record_size, _chain_record, 1);
    insertToRecord(_record_size, _chain_record, pose_number);
}


Chain::Chain() {}


Chain::Chain(uint8_t *pose_chain, uint8_t chain_size) {
    _pose_chain_size = chain_size;
    _pose_chain = pose_chain;
}


sVibeChain PoseChainer::whichChain() {
    sVibeChain vibeChain;
    vibeChain.vibe_level = 0;
    vibeChain.found_chain = -1;

    for (short i = 0; i < _num_of_chains; i++) {

        for (short index_jump = 0; index_jump < _record_size; index_jump++) {

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
    for (uint8_t i = 0; i < _num_of_chains; i++) {
        for (uint8_t j = 0; j < _chains[i]._pose_chain_size; j++) {
            //use print func here, be it with serial or with something else
        }
    }
}

void PoseChainer::printRecord() {
    for (uint8_t i = 0; i < _record_size; i++) {
        //use print func here, be it with serial or with something else
    }

}


void PoseChainer::emptyRecord() {
    gEmptyRecord(_record_size, _chain_record);
}


bool pushRecordBackwards(uint8_t record_size, uint8_t *chain_record, bool check_if_record_is_full) {
    if (check_if_record_is_full) {
        for (uint8_t i = 0; i < record_size; ++i) {
            if (chain_record[i] == 0) {
                return 0;
            }
        }
        pushRecordBackwards(record_size, chain_record, 0);
        return 1;
    } else {
        uint8_t i = 0;
        for (; i < record_size - 1;) {
            chain_record[i] = chain_record[i + 1];
            ++i;
        }
        chain_record[i] = 0;
        return 1;
    }
}

bool insertToRecord(uint8_t record_size, uint8_t *chain_record, uint8_t pose_number) {

    for (uint8_t i = 0; i < record_size; i++) {
        if (chain_record[i] == 0) {
            chain_record[i] = pose_number;
            return 0;
        }
    }
    return 1;
}

void gEmptyRecord(uint8_t record_size, uint8_t *chain_record) {
    for (uint8_t i = 0; i < record_size; i++) {
        chain_record[i] = 0;
    }
}
