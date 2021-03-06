#include "cleaner_alpha/util/Pose.h"

namespace youbot_proxy {

namespace util {

const util::Pose Pose::createPose(const std::string* jointNameArray, int size){
    // create pose object and initialize the poses
    util::Pose pose(jointNameArray, size);

    double defaultPose[5] = ARM_POSE_INIT;
    pose.addPose(INIT, defaultPose);
    double poseObserve[5] = ARM_POSE_OBSERVE_FAR;
    pose.addPose(OBSERVE_FAR, poseObserve);
    double poseObserveNear[5] = ARM_POSE_OBSERVE_NEAR;
    pose.addPose(OBSERVE_NEAR, poseObserveNear);
    double poseTower[5] = ARM_POSE_TOWER;
    pose.addPose(TOWER, poseTower);
    double poseDrop[5] = ARM_POSE_DROP;
    pose.addPose(DROP_AT_PLATE, poseDrop);

    return pose;
}

Pose::Pose(const std::string* joint_names, size_t n):
    ARM_JOINT_NAMES(joint_names),
    N(n)
{}

void Pose::addPose(POSE_ID poseId, double j1, double j2, double j3, double j4, double j5) {
    double pose[N];
    pose[0] = j1;
    pose[1] = j2;
    pose[2] = j3;
    pose[3] = j4;
    pose[4] = j5;
    addPose(poseId, pose);
}

void Pose::addPose(POSE_ID poseId, double* pose) {
    poses[poseId] = pose;

    brics_actuator::JointPositions positions;
    for(int i=0; i<N; i++) {
        brics_actuator::JointValue val;
        val.joint_uri = ARM_JOINT_NAMES[i];
        val.unit = "rad";
        val.value = pose[i];
        positions.positions.push_back(val);
    }
    joint_positions[poseId] = positions;
}

const std::vector<double> Pose::pose(POSE_ID poseId) const {
    std::vector<double> returned = std::vector<double>(N);
    try {
        double* pose = poses.at(poseId);
        for(int i=0; i<N; i++) {
            returned[i] = pose[i];
        }
    } catch(std::out_of_range ex) {
        ROS_WARN("pose was not initialized ... returning empty pose: cause %s", ex.what());
    }
    return returned;
}

const brics_actuator::JointPositions Pose::jointPositions(POSE_ID poseId) const {
    brics_actuator::JointPositions positions;
    try {
        positions = joint_positions.at(poseId);
    } catch(std::out_of_range ex) {
        ROS_WARN("pose was not initialized ... returning empty pose: cause %s", ex.what());
    }
    return positions;
}

}
}
