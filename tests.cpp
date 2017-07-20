#include <stdio.h>
#include "PoseChainer.h"

int main()
{
    PoseChainer poseChainer(2, 3);

    uint8_t chain1[3] = {3, 1, 2};
    uint8_t chain2[3] = {1, 1, 2};
    uint8_t chain3[3] = {1, 2, 2};

    poseChainer.addChain(chain1, 3);
    poseChainer.addChain(chain2, 3);
    poseChainer.addChain(chain3, 3);

    poseChainer.insertPoseToChainRecord(1);

    volatile sVibeChain vibeChain = poseChainer.whichChain();
    printf("Found Chain: %d\n", vibeChain.found_chain);
    printf("Vibration Level: %d\n", vibeChain.vibe_level);


    return 1;
}