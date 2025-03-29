#include <flagpp.hpp>
#include <iostream>

int main() {
  // Define some feature flags
  flagpp::flags::define("dark_mode", true, "Enable dark mode UI");
  flagpp::flags::define("max_connections", 100, "Maximum number of connections");
  flagpp::flags::define("api_endpoint", std::string("https://api.example.com"), 
                       "API endpoint URL");
  flagpp::flags::define("timeout_seconds", 30.5, "Connection timeout in seconds");

  // Check if a boolean flag is enabled
  if (flagpp::flags::is_enabled("dark_mode")) {
    std::cout << "Dark mode is enabled\n";
  } else {
    std::cout << "Dark mode is disabled\n";
  }

  // Get a numeric flag value
  auto max_conn = flagpp::flags::get_value<int>("max_connections");
  if (max_conn) {
    std::cout << "Max connections: " << *max_conn << "\n";
  }

  // Get a string flag value
  auto endpoint = flagpp::flags::get_value<std::string>("api_endpoint");
  if (endpoint) {
    std::cout << "API endpoint: " << *endpoint << "\n";
  }

  // Get a double flag value
  auto timeout = flagpp::flags::get_value<double>("timeout_seconds");
  if (timeout) {
    std::cout << "Timeout: " << *timeout << " seconds\n";
  }

  // Update a flag's value
  std::cout << "\nUpdating flags...\n\n";
  flagpp::flags::update("dark_mode", false);
  flagpp::flags::update("max_connections", 200);
  flagpp::flags::update("api_endpoint", std::string("https://api2.example.com"));
  flagpp::flags::update("timeout_seconds", 60.0);
  
  // Check the updated values
  std::cout << "Dark mode is " << (flagpp::flags::is_enabled("dark_mode") ? "enabled" : "disabled") << "\n";
  
  max_conn = flagpp::flags::get_value<int>("max_connections");
  if (max_conn) {
    std::cout << "Updated max connections: " << *max_conn << "\n";
  }
  
  endpoint = flagpp::flags::get_value<std::string>("api_endpoint");
  if (endpoint) {
    std::cout << "Updated API endpoint: " << *endpoint << "\n";
  }
  
  timeout = flagpp::flags::get_value<double>("timeout_seconds");
  if (timeout) {
    std::cout << "Updated timeout: " << *timeout << " seconds\n";
  }

  // List all flags
  std::cout << "\nAll registered flags:\n";
  for (const auto& flag : flagpp::flags::get_all()) {
    std::cout << "- " << flag->name() << ": ";
    
    if (auto val = flag->value().get<bool>()) {
      std::cout << (*val ? "true" : "false");
    } else if (auto val = flag->value().get<int>()) {
      std::cout << *val;
    } else if (auto val = flag->value().get<double>()) {
      std::cout << *val;
    } else if (auto val = flag->value().get<std::string>()) {
      std::cout << "\"" << *val << "\"";
    }
    
    if (!flag->description().empty()) {
      std::cout << " (" << flag->description() << ")";
    }
    
    std::cout << "\n";
  }

  return 0;
}
