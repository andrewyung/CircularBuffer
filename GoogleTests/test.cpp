#include "pch.h"

class CircularBufferTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		cb1_.add(12);
		cb1_.add(1234);
		cb1_.add(123);
		cb1_.add(123456);
	}

	CircularBuffer<int> cb0_;
	CircularBuffer<int> cb1_;
};

// Test add and check size
TEST_F(CircularBufferTest, IntAddSize) {
	
	cb0_.add(0);
	EXPECT_EQ(cb0_.size(), 1);

	cb0_.add(10);
	EXPECT_EQ(cb0_.size(), 2);

	cb0_.add(2);
	cb0_.add(3);
	EXPECT_EQ(cb0_.size(), 4);
}

// Test get
TEST_F(CircularBufferTest, IntGet) {

	EXPECT_EQ(cb1_.get(0), 12);
	EXPECT_EQ(cb1_.get(1), 1234);
	EXPECT_EQ(cb1_.get(2), 123);
	EXPECT_EQ(cb1_.get(3), 123456);
}

// Test insert (at beginning, middle and end)
TEST_F(CircularBufferTest, IntInsertGet) {

	// Middle
	cb1_.insert(321, 2);
	EXPECT_EQ(cb1_.get(2), 321);
	// Beginning
	cb1_.insert(32, 0);
	EXPECT_EQ(cb1_.get(0), 32);
	// End
	cb1_.insert(12121, 6);
	EXPECT_EQ(cb1_.get(6), 12121);
}

// Test remove then reinsert into beginning
TEST_F(CircularBufferTest, IntRemoveGet) {

	int head0 = cb1_.remove();
	EXPECT_EQ(head0, 12);
	int head1 = cb1_.remove();
	EXPECT_EQ(head1, 1234);
	EXPECT_EQ(cb1_.get(0), 123);
	EXPECT_EQ(cb1_.get(1), 123456);

	cb1_.insert(111, 0);
	EXPECT_EQ(cb1_.get(0), 111);
	EXPECT_EQ(cb1_.get(1), 123);
	EXPECT_EQ(cb1_.get(2), 123456);
}