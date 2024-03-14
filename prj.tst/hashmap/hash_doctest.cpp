#include "Hashmap/hash_new.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <string>
#define HASH_SIZE 10000

using hash_string = lab618::CHash<std::string, lab618::stringHashFunction, lab618::stringComparator>;

TEST_CASE("[hash] -- comparator") {
    std::string str_1 = "aaa";
    std::string str_2 = "aaa";
    std::string str_3 = "bbb";
    CHECK(lab618::stringComparator(&str_1, &str_2) == 0);
    CHECK(lab618::stringComparator(&str_1, &str_3) == 1);
}

TEST_CASE("[hash] -- insert delete ") {
    hash_string hash(HASH_SIZE);

    std::string str_1 = "aaa";
    std::string str_2 = "bbb";

    hash.add(&str_1);
    hash.add(&str_2);
    CHECK(*hash.find(str_1) == str_1);
    CHECK(*hash.find(str_2) == str_2);

    str_2 = "ccc";
    hash.update(&str_2);
    CHECK(*hash.find(str_2) == str_2);
}