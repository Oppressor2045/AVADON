/*
 * test_main.cpp — Test suite entry point
 * AVADON Network Reconnaissance Framework
 *
 * Build:
 *   g++ -std=c++17 -Iinclude -Itests \
 *       tests/test_main.cpp src/scanner.cpp \
 *       -lpthread -o run_tests
 *
 * Run:
 *   ./run_tests
 */

#include "test_framework.hpp"

// Forward declarations
void test_argparser();
void test_port_parser();
void test_cidr_expand();
void test_service_map();

REGISTER_TEST(test_argparser);
REGISTER_TEST(test_port_parser);
REGISTER_TEST(test_cidr_expand);
REGISTER_TEST(test_service_map);

int main() {
    return run_all_tests();
}
