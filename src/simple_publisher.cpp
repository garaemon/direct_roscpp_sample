#include <ros/ros.h>
#include <ros/transport_subscriber_link.h>
#include <ros/connection.h>
#include <ros/connection_manager.h>
#include <ros/transport/transport_udp.h>
#include <ros/poll_manager.h>
#include <std_msgs/String.h>

// ros::NodeHandle.advertise
// -> TopicManager->advertise
//   -> Publisher
// ros::Publisher::publish
//   -> TopicManager::publish
//      -> Publication::publish
//        -> Publication::enqueueMessage
//          -> SubscriberLink::enqueueMessage
//          -> TransportSubscriberLink::enqueueMessage
// TransportSubscriberLink
//   <- ConnectionManager::onConnectionHeaderReceived
// PollManager::addPollThreadListener
// -> TopicManager::processPublishQueues
//   -> Publication::processPublishQueue

// TopicManager::requestTopic
//   --> 
int main(int argc, char** argv)
{
  std_msgs::String string_message;
  string_message.data = "Hello World";
  boost::shared_ptr<ros::TransportSubscriberLink> subscriber_link
    (new ros::TransportSubscriberLink);
  ros::TransportUDPPtr transport_server(new ros::TransportUDP(&ros::PollManager::instance()->getPollSet()));
  bool success = transport_server->createIncoming(0, true);
  boost::shared_ptr<ros::Connection> connection(new ros::Connection);
  
  ros::TransportUDPPtr transport = transport_server->createOutgoing("localhost", 2014, 0, 1);
  connection->initialize(transport,false, NULL);
  subscriber_link->initialize(connection);
  ros::SerializedMessage m;
  m.type_info = &typeid(std_msgs::String);
  m.message = boost::make_shared<std_msgs::String>(string_message);
  while (true) {
    std::cerr << "send" << std::endl;
    subscriber_link->enqueueMessage(m, false, false);
    sleep(1);
  }
  // ros::ConnectionManagerPtr connection_manager = ros::ConnectionManager::instance();
  // //connection_manager->start();
  // connection_manager->getUDPServerTransport();
  // ros::TransportUDPPtr transport = connection_manager->getUDPServerTransport()->createOutgoing("localhost", 1024, 0, 1000);
  //ros::Header h;
  //connection->writeTransport();
  // ros::AdvertiseOptions ops;
  // ops.template init<std_msgs::String>("dummy_topic", 1);
  // ops.latch = false;
  std::cout << "done" << std::endl;
  return 0;
}
