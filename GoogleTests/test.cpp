#include "pch.h"

class CircularBufferTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
	}

	CircularBuffer<int> cb0_;
};

TEST_F(CircularBufferTest, IntAddSize) {
	
	cb0_.add(0);
	EXPECT_EQ(cb0_.size(), 1);

	cb0_.add(10);
	EXPECT_EQ(cb0_.size(), 2);

	cb0_.add(2);
	cb0_.add(3);
	EXPECT_EQ(cb0_.size(), 4);
}

TEST_F(CircularBufferTest, IntGet) {

	cb0_.add(12);
	cb0_.add(1234);
	cb0_.add(123);
	cb0_.add(123456);
	EXPECT_EQ(cb0_.get(0), 12);
	EXPECT_EQ(cb0_.get(1), 1234);
	EXPECT_EQ(cb0_.get(2), 123);
	EXPECT_EQ(cb0_.get(3), 123456);
}

TEST_F(CircularBufferTest, IntInsertGet) {

	cb0_.add(12);
	cb0_.add(1234);
	cb0_.add(123);
	cb0_.add(123456);
	cb0_.insert(321, 2);
	EXPECT_EQ(cb0_.get(0), 12);
	EXPECT_EQ(cb0_.get(1), 1234);
	EXPECT_EQ(cb0_.get(2), 321);
	EXPECT_EQ(cb0_.get(3), 123);
	EXPECT_EQ(cb0_.get(4), 123456);
}