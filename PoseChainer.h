#ifndef PoseChainer_h
#define PoseChainer_h

#define POSE_LIMIT 10
#define CHAIN_LIMIT 10
#define NULL nullptr

#define POSE_TIMEOUT 4000

class PoseChain
{
    public:
        //---------//
        //FUNCTIONS//
        PoseChain(int pose_chain[], int chain_size);
        PoseChain();
        void printDetails();
        int insertPoseToRecord(int pose_number);
        bool poseChainBingo();
        int firstPose();
        void emptyRecord();
        long getLastInsertTime();

        //---------//
        //VARIABLES//
        int _pose_chain[POSE_LIMIT] = {0};


    private:
        //---------//
        //FUNCTIONS//
        void pushRecordBackwards();
        bool isPoseRequired(int pose_number);
        bool PoseChain::isEmpty();

        //---------//
        //VARIABLES//
        int _pose_chain_size;
        int _chain_record[POSE_LIMIT] = {0};
        long _current_time;
        long _last_insert_time;

};

class PoseChainManager
{
    public:
        //---------//
        //FUNCTIONS//
        PoseChainManager(long CHAIN_TIME_OUT, long RECORD_TIME_OUT);
        void addPoseChain(int pose_chain[], int chain_size);
        int insertToPose(int pose_number);
        int poseManagerBingo();
        void checkWithTime();


    private:
        //---------//
        //VARIABLES//
        int _num_of_chains;
        int _in_pose;
        PoseChain _chains[5];
        long _last_bingo_time;
        long _BINGO_TIME_OUT;
        long _RECORD_TIME_OUT;
        bool _allowed;

    //---------//
    //FUNCTIONS//
    bool whichPoseChain(int pose_number);
};



#endif