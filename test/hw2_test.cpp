#include <gtest/gtest.h>

extern "C" {
    #include "k_means.h"
}

TEST (evil_test, evil_test) {
  K_means* k_means = NULL;
  EXPECT_EQ (0, create_points(&k_means, 3333333, 33));
  EXPECT_EQ (0, proceed_algorithm(k_means));
  EXPECT_EQ (0, delete_points (&k_means));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}