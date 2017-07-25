#include <MyoBridge.h>
#include <SoftwareSerial.h>
#include <PoseChainer.h>

//poseChainer limits on how many chains allowed and how many poses should be kept in the record
#define CHAIN_LIMIT 2
#define RECORD_LIMIT 3
//SoftwareSerial connection to MyoBridge
SoftwareSerial bridgeSerial(2, 3);

//initialize MyoBridge object with software serial connection
MyoBridge bridge(bridgeSerial);

//initialize poseChainer object with the limits
PoseChainer poseChainer(CHAIN_LIMIT, RECORD_LIMIT);

//desired pose chains
uint8_t turn_led_on_chain[2] = {MYO_POSE_WAVE_IN, MYO_POSE_WAVE_IN};
#define SIZE_TURN_LED_ON_CHAIN 2

uint8_t turn_led_off_chain[3] = {MYO_POSE_WAVE_OUT, MYO_POSE_DOUBLE_TAP, MYO_POSE_DOUBLE_TAP};
#define SIZE_TURN_LED_OFF_CHAIN 3

//declare a function to handle pose data
void handlePoseData(MyoPoseData& data) {

  //convert pose data to MyoPose
  MyoPose pose;
  pose = (MyoPose)data.pose;

  //print the pose
  Serial.println(bridge.poseToString(pose));

  //poseChainer doesn't care if the pose is MYO_POSE_REST or not, so let's make sure we don't record REST poses
  if (pose != MYO_POSE_REST && pose != MYO_POSE_UNKNOWN) {
    //insert the registered pose to the record
    poseChainer.insertPoseToChainRecord(pose);
    //check if any chain matches our record
    sVibeChain vibeChain = poseChainer.whichChain();

    //check which chain matches or about to match the record
    switch (vibeChain.found_chain) {
      //-1 means no chain, 0 is the first chain we added to the record, 1 the second etc.
      case -1:
        //no match, break and do noting
        break;

      case 0:
        //check if full match happened
        if (vibeChain.vibe_level == SIZE_TURN_LED_ON_CHAIN) {
          //LED ON
          digitalWrite(LED_BUILTIN, HIGH);
          //level 2 myo vibration to signal completion
          bridge.vibrate(2);
          //empty record
          poseChainer.emptyRecord();
          break;
        }

      case 1:
        //check if full match happened
        if (vibeChain.vibe_level == SIZE_TURN_LED_OFF_CHAIN) {
          //LED OFF
          digitalWrite(LED_BUILTIN, LOW);
          //level 2 myo vibration to signal completion 
          bridge.vibrate(2);
          //empty record
          poseChainer.emptyRecord();
          break;
        }
      default:
        //in the past cases we didn't use break when a full match didn't happen
        //switch falls to default, and does a level 1 myo vibration to signal progress
        bridge.vibrate(1);

    }
  }
}

void setup() {
  //initialize both serial connections
  Serial.begin(115200);
  bridgeSerial.begin(115200);

  //wait until MyoBridge has found Myo and is connected. Make sure Myo is not connected to anything else and not in standby!
  Serial.println("Searching for Myo...");
  bridge.begin();
  Serial.println("connected!");

  //set the function that handles pose events
  bridge.setPoseEventCallBack(handlePoseData);
  //tell the Myo we want Pose data
  bridge.enablePoseData();
  //make sure Myo is unlocked
  bridge.unlockMyo();
  /*You have to perform the sync gesture to receive Pose data!*/

  //add the chains we defined earlier to our poseChainer
  poseChainer.addChain(turn_led_on_chain, 2);
  poseChainer.addChain(turn_led_off_chain, 3);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  //update the connection to MyoBridge
  bridge.update();
}
