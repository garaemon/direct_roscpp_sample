#include <ros/ros.h>
#include <ros/console.h>
#include <ros/transport_subscriber_link.h>
#include <ros/connection.h>
#include <ros/connection_manager.h>
#include <ros/transport/transport_udp.h>
#include <ros/poll_manager.h>
#include <ros/serialization.h>
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

void emptyFinishFunc(const ros::ConnectionPtr&)
{

}

int main(int argc, char** argv)
{
  if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
    ros::console::notifyLoggerLevelsChanged();
  }

  
  boost::shared_ptr<ros::TransportSubscriberLink> subscriber_link
    (new ros::TransportSubscriberLink);
  ros::TransportUDPPtr transport_server(new ros::TransportUDP(&ros::PollManager::instance()->getPollSet()));
  bool success = transport_server->createIncoming(0, true);
  boost::shared_ptr<ros::Connection> connection(new ros::Connection);
  
  
  while (true) {
    ros::TransportUDPPtr transport = transport_server->createOutgoing("localhost", 2014, 0, 1000);
    std_msgs::String string_message;
    string_message.data = "Hello World";
    connection->initialize(transport,false, NULL);
    std::cerr << "send" << std::endl;
    ros::SerializedMessage m2 = ros::serialization::serializeMessage<std_msgs::String>(string_message);
    connection->write(m2.buf, m2.num_bytes, boost::bind(&emptyFinishFunc, _1),
                      true);
    std::cerr << "ISdROP" << connection->isDropped() << std::endl;
    sleep(1);
  }
  std::cout << "done" << std::endl;
  return 0;
}
