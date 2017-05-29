#include "PoseChainer.h"
#include "Arduino.h"

/*******************************************************/
/*******************************************************/
//----------------PoseChainManager---------------------//
/*******************************************************/
/*******************************************************/
/*******************************************************/

//CONSTRUCTOR FOR PoseChainManager//
PoseChainManager::PoseChainManager(long CHAIN_TIME_OUT, long RECORD_TIME_OUT)
{
    _num_of_chains = 0;
    _in_pose = -1;
    _last_bingo_time = millis();
    _BINGO_TIME_OUT = CHAIN_TIME_OUT;
    _RECORD_TIME_OUT = RECORD_TIME_OUT;
    _allowed = true;

}

//ADD A PoseChain TO PoseChainManager//
void PoseChainManager::addPoseChain(int pose_chain[], int chain_size)
{

    _chains[_num_of_chains] = PoseChain(pose_chain, chain_size);
    _chains[_num_of_chains].printDetails();
    _num_of_chains += 1;
}

//INSERT POSE TO POSE CHAIN//
int PoseChainManager::insertToPose(int pose_number)
{
    int vibration = 0;

    if(pose_number > 1 && pose_number < 9 && _allowed) {

        if (_in_pose == -1) {
            whichPoseChain(pose_number);

        }
        if (_in_pose != -1) {
            vibration = _chains[_in_pose].insertPoseToRecord(pose_number);
        }
    }

    return vibration;
}

//CHECK TIMEOUTS//***HAS TO BE CALLED FROM SKETCH IN LOOP***//
void PoseChainManager::checkWithTime()
{
    if (_in_pose != -1)
    {
        if (millis() > _chains[_in_pose].getLastInsertTime() + _RECORD_TIME_OUT)
        {
            _chains[_in_pose].emptyRecord();
            _in_pose = -1;
        }
    }
    if(millis() > _last_bingo_time + _BINGO_TIME_OUT){
        _allowed = true;
    }
    else {
        _allowed = false;
    }
}

//CHECK IF A PoseChain BINGOS//
int PoseChainManager::poseManagerBingo()
{
    bool bingo = false;
    if (_in_pose != -1)
    {
        bingo = _chains[_in_pose].poseChainBingo();
    }

    if (bingo){
        _last_bingo_time = millis();
        return _in_pose+1;

    }
    else {
        return 0;
    }
}

//CHECK IN WHICH PoseChain USER CURRENTLY IS//
bool PoseChainManager::whichPoseChain(int pose_number)
{
    for (int i = 0; i < _num_of_chains; i++)
    {

        if (pose_number == _chains[i]._pose_chain[0])
        {
            _in_pose = i;
            return true;
        }
    }
    return false;
}

/*******************************************************/
/*******************************************************/
//--------------------PoseChain------------------------//
/*******************************************************/
/*******************************************************/
/*******************************************************/

//CONSTRUCTOR FOR PoseChain//
PoseChain::PoseChain(int pose_chain[], int chain_size)
{
    _pose_chain_size = chain_size;
    _pose_chain[_pose_chain_size];
    for (int i = 0; i < _pose_chain_size; i++){
            _pose_chain[i] = pose_chain[i];
        }
    _current_time = millis();
    _last_insert_time = millis();

}

//EPTY CONSTRUCTOR//
PoseChain::PoseChain() {}


//EMPTY THE POSE RECORD//
void PoseChain::emptyRecord()
{
    for (int i = 0; i < _pose_chain_size; i++){
        _chain_record[i] = 0;
    }
}


//POP OLDEST ITEM OF RECORD AND PUSH ALL POSES BACKWARDS//
void PoseChain::pushRecordBackwards()
{
    int i = 0;
    for ( ; i < _pose_chain_size-1; )
    {
        _chain_record[i] = _chain_record[i+1];
        ++i;
    }
    _chain_record[i] = 0;
}


//CHECK IF POSE IS IN REQUIRED PoseChain//
bool PoseChain::isPoseRequired(int pose_number)
{
    for (int i=0 ; i < _pose_chain_size; i++)
    {
        if(pose_number == _pose_chain[i])
        {
            return true;
        }
    }
    return false;
}


//INSERT NEW POSE TO THE RECORD//
int PoseChain::insertPoseToRecord(int pose_number)
{
    int vibration_mode = 0;

    if(isPoseRequired(pose_number))
    {
        if(isEmpty())
        {
            vibration_mode = 2;
        }
        else
        {
            vibration_mode = 1;
        }
        pushRecordBackwards();
        for (int i = _pose_chain_size-1; i >= 0; i--)
        {
            if(_chain_record[i] == 0)
            {
                _chain_record[i] = pose_number;
                _last_insert_time = millis();
                return vibration_mode;
            }
        }
    }

    return vibration_mode;
}


//CHECK IF POSE_CHAIN WAS MET//
bool PoseChain::poseChainBingo()
{
    for(int i = 0; i < _pose_chain_size; i++)
    {
        if(_pose_chain[i] != _chain_record[i])
        {
            return false;
        }
    }
    emptyRecord();
    _current_time = millis();
    return true;

}

//PRINT CURRENT RECORD//***USE ONLY FOR DEBUGGING AND IF SERIAL IS ALREADY SET***//
/*********************************************************************************/
void PoseChain::printDetails()
{
    for (int i = 0; i < _pose_chain_size; i++)
    {
        Serial.println(_chain_record[i]);
    }
}

//GET FIRST POSE//
int PoseChain::firstPose()
{
    return _pose_chain[0];
}

//GET LAST INSERT TIME//
long PoseChain::getLastInsertTime()
{
    return _last_insert_time;
}

//CHECK IF RECORD IS EMPTY//
bool PoseChain::isEmpty()
{
    for (int i = 0; i < _pose_chain_size; i++){
        if (_chain_record != 0)
        {
            return false;
        }
    }

    return true;
}






