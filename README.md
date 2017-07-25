# PoseChainer
PoseChainer is a little support library for the arduino MyoBridge library. It can be used if it's very important to not have any false positives in Myo<->Arduino project, but it's okay to miss a comple right positives.

The idea is to keep track of the latest registered poses from the Myo armband in a record, and check if a certain pattern or chain as we call it matches the record whether partially or completely.

[Tutorial/Blog post](https://blog.raquenaengineering.com/myo-powered-arduino-sketches-and-applications/)

# Documentation 

### `PoseChainer(uint8_t CHAIN_LIMIT, uint8_t RECORD_LIMIT);`

A manager that keeps track of the record and has functions to add chains to it and or empty it.

`CHAIN_LIMIT`: Limit of how many chains can be tracked.

`RECORD_LIMIT`: Record size. 

### `PoseChainer.addChain(uint8_t pose_chain[], uint8_t chain_size);`

Adds a chain to the tracked chains

`pose_chain[]`: An array of the pose chain in order.

`chain_size`: The size of pose_chain[].

NOTE: The order in which the chains were added is used later to determine which chain matches the record

### `PoseChainer.emptyRecord();`

Empties record of registered poses, use it after a chain matches the record.

### `PoseChainer.whichChain();`

Returns a sVibeChain struct with the found chain that matches fully or partially the record, and how many poses in the chain match. 

### `sVibeChain`
```c

struct sVibeChain  
{
    uint8_t vibe_level;
    int8_t found_chain;
};
```
Returned by `PoseChainer.whichChain();`. `found_chain` being a found chain, the int comes from the order in which the chain was added to PoseChainer. 0 meaning the first chain, 1 the second etc.
`vibe_level` is the number of poses in the chain the match the record so far, if it is equal to the size of the chain(array) then the chain fully matches the record.
