// Copyright 2021 GHA Test Team

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <chrono>
#include <thread>

#include "TimedDoor.h"


class MockTimer : public Timer {
public:
    MOCK_METHOD(void, tregister, (int, TimerClient*));
};

class MockTimerClient : public TimerClient {
public:
    MOCK_METHOD(void, Timeout, (), (override));
};

class TimedDoorTest : public ::testing::Test {
protected:
    TimedDoor door;
    MockTimerClient *mockClient;
    Timer timer;
public:
    TimedDoorTest() : door(10), timer() {}
protected:
    void SetUp() override {
        mockClient = new MockTimerClient();
    }
    void TearDown() override {
        delete mockClient;
    }
};

TEST_F(TimedDoorTest, InitialState) {
    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, UnlockDoor) {
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, LockDoor) {
    door.lock();
    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, LockAfterUnlockClosesDoor) {
    door.unlock();
    door.lock();
    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, UnlockAlreadyOpenedDoor) {
    door.unlock();
    EXPECT_THROW(door.unlock(), std::logic_error);
}

TEST_F(TimedDoorTest, LockAlreadyOpenedDoor) {
    EXPECT_THROW(door.lock(), std::logic_error);
}

TEST_F(TimedDoorTest, ExceptionThrownOn) {
    door.unlock();
    ASSERT_THROW(door.lock(), std::runtime_error);
}

TEST_F(TimedDoorTest, CLOSED_BEFORE_TIMEOUT_NO_THROW) {
    EXPECT_NO_THROW(door.throwState());
}

TEST_F(TimedDoorTest, DoorRemainsOpenAfterTimeout) {
    door.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(11)); 
    ASSERT_TRUE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, NoExceptionThrownOnTimeoutWithOpenedDoor) {
    door.unlock();
    door.unlock(); 
    std::this_thread::sleep_for(std::chrono::seconds(11)); 
    ASSERT_NO_THROW(door.lock());
}
