#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "flagpp.hpp"
#include <thread>
#include <vector>

TEST_CASE("Flag creation and retrieval") {
  // Clear any existing flags from other tests
  // Note: In a real implementation, you might want to add a clear() method to FlagRegistry
  
  SUBCASE("Define and retrieve boolean flag") {
    auto flag = flagpp::flags::define("test_bool", true, "Test boolean flag");
    CHECK(flag != nullptr);
    CHECK(flag->name() == "test_bool");
    CHECK(flag->description() == "Test boolean flag");
    CHECK(static_cast<bool>(flag->value()) == true);
    
    auto retrieved = flagpp::flags::get("test_bool");
    CHECK(retrieved != nullptr);
    CHECK(retrieved->name() == "test_bool");
    CHECK(static_cast<bool>(retrieved->value()) == true);
  }
  
  SUBCASE("Define and retrieve integer flag") {
    auto flag = flagpp::flags::define("test_int", 42, "Test integer flag");
    CHECK(flag != nullptr);
    CHECK(flag->name() == "test_int");
    CHECK(flag->description() == "Test integer flag");
    CHECK(static_cast<int>(flag->value()) == 42);
    
    auto retrieved = flagpp::flags::get("test_int");
    CHECK(retrieved != nullptr);
    CHECK(retrieved->name() == "test_int");
    CHECK(static_cast<int>(retrieved->value()) == 42);
  }
  
  SUBCASE("Define and retrieve double flag") {
    auto flag = flagpp::flags::define("test_double", 3.14, "Test double flag");
    CHECK(flag != nullptr);
    CHECK(flag->name() == "test_double");
    CHECK(flag->description() == "Test double flag");
    CHECK(static_cast<double>(flag->value()) == doctest::Approx(3.14));
    
    auto retrieved = flagpp::flags::get("test_double");
    CHECK(retrieved != nullptr);
    CHECK(retrieved->name() == "test_double");
    CHECK(static_cast<double>(retrieved->value()) == doctest::Approx(3.14));
  }
  
  SUBCASE("Define and retrieve string flag") {
    auto flag = flagpp::flags::define("test_string", std::string("hello"), "Test string flag");
    CHECK(flag != nullptr);
    CHECK(flag->name() == "test_string");
    CHECK(flag->description() == "Test string flag");
    CHECK(static_cast<std::string>(flag->value()) == "hello");
    
    auto retrieved = flagpp::flags::get("test_string");
    CHECK(retrieved != nullptr);
    CHECK(retrieved->name() == "test_string");
    CHECK(static_cast<std::string>(retrieved->value()) == "hello");
  }
  
  SUBCASE("Retrieve non-existent flag") {
    auto flag = flagpp::flags::get("non_existent");
    CHECK(flag == nullptr);
  }
}

TEST_CASE("Flag value type checking") {
  SUBCASE("Boolean flag") {
    flagpp::flags::define("type_bool", true);
    
    CHECK(flagpp::flags::get_value<bool>("type_bool").has_value());
    CHECK(*flagpp::flags::get_value<bool>("type_bool") == true);
    
    CHECK_FALSE(flagpp::flags::get_value<int>("type_bool").has_value());
    CHECK_FALSE(flagpp::flags::get_value<double>("type_bool").has_value());
    CHECK_FALSE(flagpp::flags::get_value<std::string>("type_bool").has_value());
  }
  
  SUBCASE("Integer flag") {
    flagpp::flags::define("type_int", 42);
    
    CHECK(flagpp::flags::get_value<int>("type_int").has_value());
    CHECK(*flagpp::flags::get_value<int>("type_int") == 42);
    
    CHECK_FALSE(flagpp::flags::get_value<bool>("type_int").has_value());
    CHECK_FALSE(flagpp::flags::get_value<double>("type_int").has_value());
    CHECK_FALSE(flagpp::flags::get_value<std::string>("type_int").has_value());
  }
  
  SUBCASE("Double flag") {
    flagpp::flags::define("type_double", 3.14);
    
    CHECK(flagpp::flags::get_value<double>("type_double").has_value());
    CHECK(*flagpp::flags::get_value<double>("type_double") == doctest::Approx(3.14));
    
    CHECK_FALSE(flagpp::flags::get_value<bool>("type_double").has_value());
    CHECK_FALSE(flagpp::flags::get_value<int>("type_double").has_value());
    CHECK_FALSE(flagpp::flags::get_value<std::string>("type_double").has_value());
  }
  
  SUBCASE("String flag") {
    flagpp::flags::define("type_string", std::string("hello"));
    
    CHECK(flagpp::flags::get_value<std::string>("type_string").has_value());
    CHECK(*flagpp::flags::get_value<std::string>("type_string") == "hello");
    
    CHECK_FALSE(flagpp::flags::get_value<bool>("type_string").has_value());
    CHECK_FALSE(flagpp::flags::get_value<int>("type_string").has_value());
    CHECK_FALSE(flagpp::flags::get_value<double>("type_string").has_value());
  }
}

TEST_CASE("Flag updating") {
  SUBCASE("Update boolean flag") {
    flagpp::flags::define("update_bool", false);
    CHECK(flagpp::flags::is_enabled("update_bool") == false);
    
    flagpp::flags::update("update_bool", true);
    CHECK(flagpp::flags::is_enabled("update_bool") == true);
  }
  
  SUBCASE("Update integer flag") {
    flagpp::flags::define("update_int", 100);
    CHECK(*flagpp::flags::get_value<int>("update_int") == 100);
    
    flagpp::flags::update("update_int", 200);
    CHECK(*flagpp::flags::get_value<int>("update_int") == 200);
  }
  
  SUBCASE("Update double flag") {
    flagpp::flags::define("update_double", 1.5);
    CHECK(*flagpp::flags::get_value<double>("update_double") == doctest::Approx(1.5));
    
    flagpp::flags::update("update_double", 2.5);
    CHECK(*flagpp::flags::get_value<double>("update_double") == doctest::Approx(2.5));
  }
  
  SUBCASE("Update string flag") {
    flagpp::flags::define("update_string", std::string("old"));
    CHECK(*flagpp::flags::get_value<std::string>("update_string") == "old");
    
    flagpp::flags::update("update_string", std::string("new"));
    CHECK(*flagpp::flags::get_value<std::string>("update_string") == "new");
  }
  
  SUBCASE("Update non-existent flag") {
    bool result = flagpp::flags::update("non_existent", true);
    CHECK(result == false);
  }
}

TEST_CASE("Flag existence checking") {
  SUBCASE("Check existing flag") {
    flagpp::flags::define("exists_flag", true);
    CHECK(flagpp::flags::exists("exists_flag") == true);
  }
  
  SUBCASE("Check non-existent flag") {
    CHECK(flagpp::flags::exists("non_existent_flag") == false);
  }
}

TEST_CASE("Get all flags") {
  // Define some flags
  flagpp::flags::define("all_test_1", true);
  flagpp::flags::define("all_test_2", 42);
  flagpp::flags::define("all_test_3", std::string("hello"));
  
  auto all_flags = flagpp::flags::get_all();
  
  // Check that we have at least the three flags we just defined
  // (there might be more from other tests)
  CHECK(all_flags.size() >= 3);
  
  // Check that our flags are in the list
  bool found1 = false, found2 = false, found3 = false;
  
  for (const auto& flag : all_flags) {
    if (flag->name() == "all_test_1") found1 = true;
    if (flag->name() == "all_test_2") found2 = true;
    if (flag->name() == "all_test_3") found3 = true;
  }
  
  CHECK(found1);
  CHECK(found2);
  CHECK(found3);
}

TEST_CASE("Thread safety") {
  // Define some flags
  flagpp::flags::define("thread_test_bool", false);
  flagpp::flags::define("thread_test_int", 0);
  flagpp::flags::define("thread_test_string", std::string(""));
  
  // Create threads that update and read flags concurrently
  std::vector<std::thread> threads;
  
  // Writer threads
  for (int i = 0; i < 4; ++i) {
    threads.emplace_back([i]() {
      for (int j = 0; j < 100; ++j) {
        flagpp::flags::update("thread_test_bool", j % 2 == 0);
        flagpp::flags::update("thread_test_int", i * 1000 + j);
        flagpp::flags::update("thread_test_string", 
                             std::string("thread") + std::to_string(i) + 
                             "_" + std::to_string(j));
      }
    });
  }
  
  // Reader threads
  for (int i = 0; i < 4; ++i) {
    threads.emplace_back([]() {
      for (int j = 0; j < 100; ++j) {
        // Just read the flags
        flagpp::flags::is_enabled("thread_test_bool");
        flagpp::flags::get_value<int>("thread_test_int");
        flagpp::flags::get_value<std::string>("thread_test_string");
      }
    });
  }
  
  // Wait for all threads to complete
  for (auto& t : threads) {
    t.join();
  }
  
  // If we got here without crashes or deadlocks, the test passes
  CHECK(true);
}
