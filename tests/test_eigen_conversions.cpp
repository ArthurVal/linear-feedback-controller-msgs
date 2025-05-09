#include <gtest/gtest.h>

#include "linear_feedback_controller_msgs/eigen_conversions.hpp"
#include "linear_feedback_controller_msgs/msg/control.hpp"
#include "linear_feedback_controller_msgs/msg/sensor.hpp"

namespace lfc_msgs = linear_feedback_controller_msgs;

class LinearFeedbackControllerMsgsTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}
};
class DISABLED_LinearFeedbackControllerMsgsTest
    : public LinearFeedbackControllerMsgsTest {};

TEST_F(LinearFeedbackControllerMsgsTest, checkEigenConstructors) {
  lfc_msgs::Eigen::Sensor s;
  lfc_msgs::Eigen::Control c;
}

TEST_F(LinearFeedbackControllerMsgsTest, checkRosConstructors) {
  lfc_msgs::msg::Sensor s;
  lfc_msgs::msg::Control c;
}

TEST_F(LinearFeedbackControllerMsgsTest, checkRosEigenMatrixConversion) {
  Eigen::MatrixXd eigen_mat = Eigen::MatrixXd::Random(5, 6);
  std_msgs::msg::Float64MultiArray ros_mat;
  Eigen::MatrixXd eigen_mat_test = Eigen::MatrixXd::Zero(5, 6);

  lfc_msgs::matrixEigenToMsg(eigen_mat, ros_mat);
  lfc_msgs::matrixMsgToEigen(ros_mat, eigen_mat_test);

  ASSERT_EQ(eigen_mat, eigen_mat_test);
}

TEST_F(LinearFeedbackControllerMsgsTest, checkRosEigenJointStateConversion) {
  lfc_msgs::Eigen::JointState eigen_joint_state;
  sensor_msgs::msg::JointState ros_joint_state;
  lfc_msgs::Eigen::JointState eigen_joint_state_test;

  eigen_joint_state.name = {"1", "2", "3", "4", "5", "6"};
  eigen_joint_state.position =
      Eigen::VectorXd::Random(eigen_joint_state.name.size());
  eigen_joint_state.velocity =
      Eigen::VectorXd::Random(eigen_joint_state.name.size());
  eigen_joint_state.effort =
      Eigen::VectorXd::Random(eigen_joint_state.name.size());

  lfc_msgs::jointStateEigenToMsg(eigen_joint_state, ros_joint_state);
  lfc_msgs::jointStateMsgToEigen(ros_joint_state, eigen_joint_state_test);

  ASSERT_EQ(eigen_joint_state.name, eigen_joint_state_test.name);
  ASSERT_EQ(eigen_joint_state.position, eigen_joint_state_test.position);
  ASSERT_EQ(eigen_joint_state.velocity, eigen_joint_state_test.velocity);
  ASSERT_EQ(eigen_joint_state.effort, eigen_joint_state_test.effort);
}

TEST_F(LinearFeedbackControllerMsgsTest, checkRosEigenSensorConversion) {
  lfc_msgs::Eigen::Sensor e;
  lfc_msgs::msg::Sensor m;
  lfc_msgs::Eigen::Sensor etest;

  e.base_pose = Eigen::Matrix<double, 7, 1>::Random();
  Eigen::Quaterniond q;
  q.coeffs() = Eigen::Vector4d::Random();
  q.normalize();
  e.base_pose.tail<4>() = q.coeffs();
  e.base_twist = Eigen::Matrix<double, 6, 1>::Random();
  e.joint_state.name = {"1", "2", "3", "4", "5", "6"};
  e.joint_state.position = Eigen::VectorXd::Random(e.joint_state.name.size());
  e.joint_state.velocity = Eigen::VectorXd::Random(e.joint_state.name.size());
  e.joint_state.effort = Eigen::VectorXd::Random(e.joint_state.name.size());
  e.contacts.resize(2);
  e.contacts[0] = {
      .active = true,
      .name = "left_foot",
      .wrench = Eigen::Matrix<double, 6, 1>::Random(),
      .pose = Eigen::Matrix<double, 7, 1>::Random(),
  };
  e.contacts[1] = {
      .active = false,
      .name = "right_foot",
      .wrench = Eigen::Matrix<double, 6, 1>::Random(),
      .pose = Eigen::Matrix<double, 7, 1>::Random(),
  };

  lfc_msgs::sensorEigenToMsg(e, m);
  lfc_msgs::sensorMsgToEigen(m, etest);

  ASSERT_TRUE(e.base_pose.isApprox(etest.base_pose));
  ASSERT_EQ(e.base_twist, etest.base_twist);
  ASSERT_EQ(e.joint_state.name, etest.joint_state.name);
  ASSERT_EQ(e.joint_state.position, etest.joint_state.position);
  ASSERT_EQ(e.joint_state.velocity, etest.joint_state.velocity);
  ASSERT_EQ(e.joint_state.effort, etest.joint_state.effort);
  for (std::size_t i = 0; i < e.contacts.size(); ++i) {
    ASSERT_EQ(e.contacts[i].active, etest.contacts[i].active);
    ASSERT_EQ(e.contacts[i].name, etest.contacts[i].name);
    ASSERT_EQ(e.contacts[i].pose, etest.contacts[i].pose);
    ASSERT_EQ(e.contacts[i].wrench, etest.contacts[i].wrench);
  }
}

TEST_F(LinearFeedbackControllerMsgsTest, checkRosEigenControlConversion) {
  lfc_msgs::Eigen::Control e;
  lfc_msgs::msg::Control m;
  lfc_msgs::Eigen::Control etest;

  e.initial_state.base_pose = Eigen::Matrix<double, 7, 1>::Random();
  Eigen::Quaterniond q;
  q.coeffs() = Eigen::Vector4d::Random();
  q.normalize();
  e.initial_state.base_pose.tail<4>() = q.coeffs();
  e.initial_state.base_twist = Eigen::Matrix<double, 6, 1>::Random();
  e.initial_state.joint_state.name = {"1", "2", "3", "4", "5", "6"};
  e.initial_state.joint_state.position =
      Eigen::VectorXd::Random(e.initial_state.joint_state.name.size());
  e.initial_state.joint_state.velocity =
      Eigen::VectorXd::Random(e.initial_state.joint_state.name.size());
  e.initial_state.joint_state.effort =
      Eigen::VectorXd::Random(e.initial_state.joint_state.name.size());
  e.feedback_gain = Eigen::MatrixXd::Random(8, 4);
  e.feedforward = Eigen::VectorXd::Random(8);

  etest.initial_state.base_pose.setZero();
  etest.initial_state.base_twist.setZero();
  etest.initial_state.joint_state.name = {"1", "2", "3", "4", "5", "6"};
  etest.initial_state.joint_state.position =
      Eigen::VectorXd::Zero(e.initial_state.joint_state.name.size());
  etest.initial_state.joint_state.velocity =
      Eigen::VectorXd::Zero(e.initial_state.joint_state.name.size());
  etest.initial_state.joint_state.effort =
      Eigen::VectorXd::Zero(e.initial_state.joint_state.name.size());
  etest.feedback_gain = Eigen::MatrixXd::Zero(8, 4);
  etest.feedforward = Eigen::VectorXd::Zero(8);

  lfc_msgs::controlEigenToMsg(e, m);
  lfc_msgs::controlMsgToEigen(m, etest);

  ASSERT_TRUE(
      e.initial_state.base_pose.isApprox(etest.initial_state.base_pose));
  ASSERT_EQ(e.initial_state.base_twist, etest.initial_state.base_twist);
  ASSERT_EQ(e.initial_state.joint_state.name,
            etest.initial_state.joint_state.name);
  ASSERT_EQ(e.initial_state.joint_state.position,
            etest.initial_state.joint_state.position);
  ASSERT_EQ(e.initial_state.joint_state.velocity,
            etest.initial_state.joint_state.velocity);
  ASSERT_EQ(e.initial_state.joint_state.effort,
            etest.initial_state.joint_state.effort);
  ASSERT_EQ(e.feedback_gain, etest.feedback_gain);
  ASSERT_EQ(e.feedforward, etest.feedforward);
}
