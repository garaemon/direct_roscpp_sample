#include <ros/ros.h>
#include <ros/transport_subscriber_link.h>
#include <ros/connection.h>
#include <ros/connection_manager.h>
#include <ros/transport/transport_udp.h>
#include <ros/poll_manager.h>
#include <std_msgs/String.h>

// ros::NodeHandle::subscribe
//   -> TopicManager::subscribe

int main(int argc, char** argv)
{
  boost::shared_ptr<ros::Connection> connection(new ros::Connection);
  ros::TransportUDPPtr transport = ros::ConnectionManager::instance()->getUDPServerTransport()->getServerPort();
  connection->initialize(transport, true, NULL);
  std::cout << "done" << std::endl;
  return 0;
}
