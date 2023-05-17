#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <string>
#include "fifo.h"
#include "sjf.h"
#include "stcf.h"
#include "rr.h"

using namespace std;

TEST(HomeworkTest, FIFO_workload01)
{
  FIFO fifo("workloads/workload_01.txt", 1);
  fifo.print_metrics();
  Metrics metrics = fifo.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 20);
  EXPECT_EQ(metrics.avg_response, 10);
}

TEST(HomeworkTest, FIFO_workload02)
{
  FIFO fifo("workloads/workload_02.txt", 1);
  fifo.print_metrics();
  Metrics metrics = fifo.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 50);
  EXPECT_EQ(metrics.avg_response, 10);
}

TEST(HomeworkTest, FIFO_workload03)
{
  FIFO fifo("workloads/workload_03.txt", 1);
  fifo.print_metrics();
  Metrics metrics = fifo.get_metrics();
  EXPECT_NEAR(metrics.avg_response, 63.3, 0.1);
  EXPECT_NEAR(metrics.avg_turnaround, 103.3, 0.1);
}

TEST(HomeworkTest, FIFO_workload04)
{
  FIFO fifo("workloads/workload_04.txt", 1);
  fifo.print_metrics();
  Metrics metrics = fifo.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 10);
  EXPECT_EQ(metrics.avg_response, 5);
}

TEST(HomeworkTest, SJF_workload01)
{
  SJF sjf("workloads/workload_01.txt", 1);
  sjf.print_metrics();
  Metrics metrics = sjf.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 20);
  EXPECT_EQ(metrics.avg_response, 10);
}

TEST(HomeworkTest, SJF_workload02)
{
  SJF sjf("workloads/workload_02.txt", 1);
  sjf.print_metrics();
  Metrics metrics = sjf.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 50);
  EXPECT_EQ(metrics.avg_response, 10);
}

TEST(HomeworkTest, SJF_workload03)
{
  SJF sjf("workloads/workload_03.txt", 1);
  sjf.print_metrics();
  Metrics metrics = sjf.get_metrics();
  EXPECT_NEAR(metrics.avg_turnaround, 103.3, 0.1);
  EXPECT_NEAR(metrics.avg_response, 63.3, 0.1);
}

TEST(HomeworkTest, SJF_workload04)
{
  SJF sjf("workloads/workload_04.txt", 1);
  sjf.print_metrics();
  Metrics metrics = sjf.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 10);
  EXPECT_EQ(metrics.avg_response, 5);
}

TEST(HomeworkTest, STCF_workload01)
{
  STCF stcf("workloads/workload_01.txt", 1);
  stcf.print_metrics();
  Metrics metrics = stcf.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 20);
  EXPECT_EQ(metrics.avg_response, 10);
}

TEST(HomeworkTest, STCF_workload02)
{
  STCF stcf("workloads/workload_02.txt", 1);
  stcf.print_metrics();
  Metrics metrics = stcf.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 50);
  EXPECT_EQ(metrics.avg_response, 10);
}

TEST(HomeworkTest, STCF_workload03)
{
  STCF stcf("workloads/workload_03.txt", 1);
  stcf.print_metrics();
  Metrics metrics = stcf.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 50);
  EXPECT_NEAR(metrics.avg_response, 3.3, 0.1);
}

TEST(HomeworkTest, STCF_workload04)
{
  STCF stcf("workloads/workload_04.txt", 1);
  stcf.print_metrics();
  Metrics metrics = stcf.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 10);
  EXPECT_EQ(metrics.avg_response, 5);
}

TEST(HomeworkTest, RR_workload01)
{
  RR rr("workloads/workload_01.txt", 1);
  rr.print_metrics();
  Metrics metrics = rr.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 29);
  EXPECT_EQ(metrics.avg_response, 1);
}

TEST(HomeworkTest, RR_workload02)
{
  RR rr("workloads/workload_02.txt", 1);
  rr.print_metrics();
  Metrics metrics = rr.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 59);
  EXPECT_EQ(metrics.avg_response, 1);
}

TEST(HomeworkTest, RR_workload03)
{
  RR rr("workloads/workload_03.txt", 1);
  rr.print_metrics();
  Metrics metrics = rr.get_metrics();
  EXPECT_NEAR(metrics.avg_turnaround, 59.6, 0.1);
  EXPECT_EQ(metrics.avg_response, 1);
}

TEST(HomeworkTest, RR_workload04)
{
  RR rr("workloads/workload_04.txt", 1);
  rr.print_metrics();
  Metrics metrics = rr.get_metrics();
  EXPECT_EQ(metrics.avg_turnaround, 14);
  EXPECT_EQ(metrics.avg_response, 1);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
