#include <flagpp.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

// Shared counter for tracking operations
std::atomic<int> counter{0};

void reader_thread(int id, int iterations) {
  for (int i = 0; i < iterations; ++i) {
    // Read various flags
    bool dark_mode = flagpp::flags::is_enabled("dark_mode");
    auto max_conn = flagpp::flags::get_value<int>("max_connections");
    auto endpoint = flagpp::flags::get_value<std::string>("api_endpoint");
    
    // Just to prevent compiler optimization
    if (dark_mode && max_conn && endpoint) {
      counter.fetch_add(1, std::memory_order_relaxed);
    }
    
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
  
  std::cout << "Reader thread " << id << " completed\n";
}

void writer_thread(int id, int iterations) {
  for (int i = 0; i < iterations; ++i) {
    // Update flags with new values
    flagpp::flags::update("dark_mode", i % 2 == 0);
    flagpp::flags::update("max_connections", 100 + i);
    flagpp::flags::update("api_endpoint", 
                         std::string("https://api") + std::to_string(i) + ".example.com");
    
    counter.fetch_add(1, std::memory_order_relaxed);
    
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  
  std::cout << "Writer thread " << id << " completed\n";
}

int main() {
  // Define initial flags
  flagpp::flags::define("dark_mode", false, "Enable dark mode UI");
  flagpp::flags::define("max_connections", 100, "Maximum number of connections");
  flagpp::flags::define("api_endpoint", std::string("https://api.example.com"), 
                       "API endpoint URL");

  // Create multiple reader and writer threads
  std::vector<std::thread> threads;
  
  // Start reader threads (more readers than writers)
  for (int i = 0; i < 8; ++i) {
    threads.emplace_back(reader_thread, i, 1000);
  }
  
  // Start writer threads
  for (int i = 0; i < 2; ++i) {
    threads.emplace_back(writer_thread, i, 100);
  }
  
  // Wait for all threads to complete
  for (auto& t : threads) {
    t.join();
  }
  
  // Print final flag values
  std::cout << "\nFinal flag values:\n";
  std::cout << "dark_mode: " << (flagpp::flags::is_enabled("dark_mode") ? "true" : "false") << "\n";
  
  auto max_conn = flagpp::flags::get_value<int>("max_connections");
  if (max_conn) {
    std::cout << "max_connections: " << *max_conn << "\n";
  }
  
  auto endpoint = flagpp::flags::get_value<std::string>("api_endpoint");
  if (endpoint) {
    std::cout << "api_endpoint: " << *endpoint << "\n";
  }
  
  std::cout << "Total operations: " << counter.load() << "\n";
  
  return 0;
}
