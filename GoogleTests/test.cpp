#include "pch.h"
#include <thread>

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

// Test add and remove. Check add and remove order is correct
TEST_F(CircularBufferTest, IntAddRemove) {

	int head0 = cb1_.remove();
	EXPECT_EQ(head0, 12);

	cb1_.add(345);

	int head1 = cb1_.remove();
	EXPECT_EQ(head1, 1234);

	int head2 = cb1_.remove();
	EXPECT_EQ(head2, 123);

	int head3 = cb1_.remove();
	EXPECT_EQ(head3, 123456);

	cb1_.add(234);
	cb1_.add(23456);
	cb1_.add(23);

	int head4 = cb1_.remove();
	EXPECT_EQ(head4, 345);

	int head5 = cb1_.remove();
	EXPECT_EQ(head5, 234);

	int head6 = cb1_.remove();
	EXPECT_EQ(head6, 23456);

	int head7 = cb1_.remove();
	EXPECT_EQ(head7, 23);
}

// Multi-threading add into same CircularBuffer and check for consistency
void bufferAdd(CircularBuffer<int>* cb, int addValue)
{
	cb->add(addValue);
}
TEST_F(CircularBufferTest, TheadAdd) 
{
	std::thread threads[10];
	for (int i = 0; i < 10; i++)
		threads[i] = std::thread(bufferAdd, &cb0_, i + 1);

	for (auto& th : threads) th.join();

	EXPECT_EQ(cb0_.remove(), 1);
	EXPECT_EQ(cb0_.remove(), 2);
	EXPECT_EQ(cb0_.remove(), 3);
	EXPECT_EQ(cb0_.remove(), 4);
	EXPECT_EQ(cb0_.remove(), 5);
	EXPECT_EQ(cb0_.remove(), 6);
	EXPECT_EQ(cb0_.remove(), 7);
	EXPECT_EQ(cb0_.remove(), 8);
	EXPECT_EQ(cb0_.remove(), 9);
	EXPECT_EQ(cb0_.remove(), 10);

	EXPECT_EQ(cb0_.size(), 0);
}

// Test resizing of buffer when adding beyond capacity
TEST_F(CircularBufferTest, AddResize)
{
	cb1_.add(1);
	cb1_.add(2);
	cb1_.add(3);
	cb1_.add(4);
	cb1_.add(5);
	cb1_.add(6);

	EXPECT_EQ(cb1_.size(), 10);
	cb1_.add(7);
	cb1_.add(8);
	cb1_.add(9);
	cb1_.add(10);

	EXPECT_EQ(cb1_.size(), 14);
	EXPECT_EQ(cb1_.get(0), 12);
	EXPECT_EQ(cb1_.get(1), 1234);
	EXPECT_EQ(cb1_.get(2), 123);
	EXPECT_EQ(cb1_.get(3), 123456);
}

// Testing of invalidation from resize
TEST_F(CircularBufferTest, PointerAddResize)
{
	cb1_.add(1);
	cb1_.add(2);
	cb1_.add(3);
	cb1_.add(4);
	cb1_.add(5);
	cb1_.add(6);

	int* threePtr = &(cb1_.get(6)); // By reference
	int threeValue = (cb1_.get(6)); // By value
	EXPECT_EQ(cb1_.get(6), 3); // Validate object at index

	cb1_.add(7);
	cb1_.add(8);
	cb1_.add(9);
	cb1_.add(10);

	EXPECT_TRUE(*threePtr != 3); // Validate pointer to location is no longer valid
	EXPECT_TRUE(threeValue == 3); // Validate that return by value is unchanged
	threeValue = 1;
	EXPECT_EQ(cb1_.get(6), 3); // Validate object at index

}