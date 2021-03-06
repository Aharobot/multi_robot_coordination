/*
 * Copyright (c) 2020, Vision for Robotics Lab
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * * Neither the name of the Vision for Robotics Lab, ETH Zurich nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * pointcloud_filter.h
 * @author Luca Bartolomei, V4RL
 * @brief  Class to filter out the points inside a box in a pointcloud
 * @date   22.08.2019
 */

#pragma once

#include <eigen_conversions/eigen_msg.h>
#include <nav_msgs/Odometry.h>
#include <pcl/common/transforms.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf/transform_listener.h>

namespace pointcloud_filter {

class PointcloudFilter {
 public:
  typedef sensor_msgs::PointCloud2 PointcloudROS;
  typedef pcl::PointCloud<pcl::PointXYZRGB> PointcloudPCL;

  /**
   * @brief Class constructor
   * @param[in] nh : ROS node handle
   * @param[in] nh_private : Private ROS node handle
   * @param[in] agent_id : ID of current agent
   */
  PointcloudFilter(const ros::NodeHandle &nh, const ros::NodeHandle &nh_private,
                   const int agent_id);

  /**
   * @brief Destructor
   */
  ~PointcloudFilter();

 private:
  /**
   * @brief Subscriber for the dense stereo pointcloud for an agent
   * @param[in] pcl_msg : pointcloud in ROS msg format
   */
  void pointCloudCallback(const PointcloudROS::ConstPtr &pcl_msg);

  /**
   * @brief Callback for odometry. It stores the odometry for an agent in an
   *        homogeneous transformation
   * @param[in] odom_msg : odometry message to process
   * @param[in] agent_id : agent id the odometry corresponds to
   */
  void odometryCallback(const nav_msgs::OdometryConstPtr &odom_msg,
                        const uint64_t agent_id);

  /**
   * @brief Callback to get the transformation from odometry to map for an
   *        agent.
   * @param[in] transf_msg : the transformation message
   * @param[in[ agent_id : the id of the agent the transformation belongs to
   */
  void odomToMapCallback(
      const geometry_msgs::TransformStampedConstPtr &transf_odom_map_msg,
      const uint64_t agent_id);

  /**
   * @brief Callback to get the transformation from map to world for an agent.
   * @param[in] transf_msg : the transformation message
   * @param[in[ agent_id : the id of the agent the transformation belongs to
   */
  void mapToWorldCallback(
      const geometry_msgs::TransformStampedConstPtr &transf_map_world_msg,
      const uint64_t agent_id);

  /**
   * @brief Method to get the position of an agent from the tf tree
   * @param[id] agent_id : id of the agent to locate
   * @param[out] position : position of the agent in the world frame
   * @return True if query of position was successfull
   */
  bool getPositionAgentId(const int agent_id, Eigen::Vector3d &position) const;

 protected:
  ros::NodeHandle nh_;
  ros::NodeHandle nh_private_;

  ros::Subscriber pcl_sub_;
  std::vector<ros::Subscriber> odometry_subs_;
  std::vector<ros::Subscriber> transf_odom_map_subs_;
  std::vector<ros::Subscriber> transf_map_world_subs_;
  ros::Publisher pcl_pub_;

  tf::TransformListener tf_listener_;

  int num_agents_;
  int agent_id_;
  double ball_radius_;
  std::vector<Eigen::Vector3d> agents_positions_;
  std::string world_frame_;
  std::string agent_frame_;
  std::string pointcloud_topic_;

  // Storages for the transformations
  // Reference frame convention for each agent:
  // - W : world
  // - M : map
  // - O : odom
  // - A : agent
  // Convention: T_B_A --> from A to B
  std::vector<Eigen::Matrix4d> T_O_A_;  // this is from odometry information
  std::vector<Eigen::Matrix4d> T_W_M_;  // this is from pose graph backend
  std::vector<Eigen::Matrix4d> T_M_O_;  // this is from pose graph backend

  std::vector<bool> transformations_initialized_O_A_;
  std::vector<bool> transformations_initialized_M_O_;
  std::vector<bool> transformations_initialized_W_M_;

};  // end class pointcloud filter

}  // end namespace pointcloud_filter
