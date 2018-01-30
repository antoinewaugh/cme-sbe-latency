
#include "DepthList.h"
#include <gtest/gtest.h>


class DepthListTest : public testing::Test {
public:
  DepthList list;

  DepthListTest()
      : list(true, 5)
  {
  }

  void SetUp()
  {
    list = DepthList(true, 5);
  }
};

TEST_F(DepthListTest, insert)
{
list.Add(1, 1300.25, 100);

ASSERT_EQ(list.Size(), 1);
ASSERT_EQ(list.Get(0).price, 1300.25);
ASSERT_EQ(list.Get(0).quantity, 100);
}


TEST_F(DepthListTest, modifyValidIndex)
{
list.Add(1,1300.25, 100);
list.Update(1, 1300.25, 1012);

ASSERT_EQ(list.Size(), 1);
ASSERT_EQ(list.Get(0).price, 1300.25);
ASSERT_EQ(list.Get(0).quantity, 1012);
}

TEST_F(DepthListTest, modifyInvalidIndex)
{
testing::internal::CaptureStdout();
list.Update(1,1300.25, 100);
ASSERT_EQ("Update bid level out of range : 1 > 0\n", testing::internal::GetCapturedStdout());
}

TEST_F(DepthListTest, deleteFrom)
{
  list.Add(1,1300.25, 89);
  list.Add(2,1300.5, 111);
  list.Add(3,1300.75, 56);
  list.Add(4,1301.0, 2);

  list.DeleteFrom(2);
  ASSERT_EQ(list.Size(), 2);
  ASSERT_EQ(list.Get(0).price, 1300.75);
  ASSERT_EQ(list.Get(0).quantity, 56);
  ASSERT_EQ(list.Get(1).price, 1301.0);
  ASSERT_EQ(list.Get(1).quantity, 2);
}

TEST_F(DepthListTest, deleteThru)
{
  list.Add(1,1300.25, 89);
  list.Add(2,1300.5, 111);
  list.Add(3,1300.75, 56);
  list.Add(4,1301.0, 2);

  list.DeleteThru(2);
  ASSERT_EQ(list.Size(), 1);
  ASSERT_EQ(list.Get(0).price, 1300.25);
  ASSERT_EQ(list.Get(0).quantity, 89);
}


TEST_F(DepthListTest, removeValidIndex)
{
list.Add(1,1300.25, 100);
list.Delete(1,1300.25);
ASSERT_EQ(list.Size(), 0);
}

TEST_F(DepthListTest, removeInvalidPrice)
{
testing::internal::CaptureStdout();

list.Add(1,1300.25, 100);
list.Delete(1,1300.5);

ASSERT_EQ("Price mismatch on delete request: 1300.5 != 1300.25\n", testing::internal::GetCapturedStdout());
}

TEST_F(DepthListTest, removeInvalidIndex)
{
testing::internal::CaptureStdout();
list.Delete(1,1300.25);
ASSERT_EQ("Delete level out of range : 1 > 0\n", testing::internal::GetCapturedStdout());
}

TEST_F(DepthListTest, copyConstructor) {
  DepthList copy(list);
  list.Add(1, 1300.25, 100);

  ASSERT_EQ(list.Size(), 1);
  ASSERT_EQ(copy.Size(), 0);
}

