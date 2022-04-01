#include <gtest/gtest.h>

extern "C" {
    #include "../k_means_classic/k_means.h"
}

TEST (main_test, main_test) {
    K_means* k_means = NULL;
    EXPECT_EQ (0, create_points(&k_means));
    EXPECT_EQ (0, proceed_algorithm(k_means));
    EXPECT_EQ (0, delete_points (&k_means));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}