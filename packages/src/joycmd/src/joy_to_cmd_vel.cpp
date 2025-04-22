#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"

class JoyToCmdVel : public rclcpp::Node {
  public:
    JoyToCmdVel() : Node("joy_to_cmd_vel") {
        joy_sub_ = this->create_subscription<sensor_msgs::msg::Joy>(
            "/joy", 10, std::bind(&JoyToCmdVel::joy_callback, this, std::placeholders::_1));

        cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

        // Optional parameters for scaling
        this->declare_parameter("linear_scale", 1.0);
        this->declare_parameter("angular_scale", 1.0);

        this->get_parameter("linear_scale", linear_scale_);
        this->get_parameter("angular_scale", angular_scale_);
    }

  private:
    void joy_callback(const sensor_msgs::msg::Joy::SharedPtr joy_msg) {
        auto twist_msg = geometry_msgs::msg::Twist();

        twist_msg.linear.x = linear_scale_ * joy_msg->axes[1];   // Forward/backward
        twist_msg.angular.z = angular_scale_ * joy_msg->axes[0]; // Left/right

        cmd_vel_pub_->publish(twist_msg);
    }

    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joy_sub_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;

    double linear_scale_;
    double angular_scale_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JoyToCmdVel>());
    rclcpp::shutdown();
    return 0;
}
