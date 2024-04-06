// Copyright 2024 

#include "TimedDoor.h"
#include <thread>
#include <chrono>
#include <stdexcept>

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& door) : door(door) {}

void DoorTimerAdapter::Timeout() {
    if (door.isDoorOpened()) {
        throw std::runtime_error("Door is still open after timeout!");
    }
}

TimedDoor::TimedDoor(int timeout) : adapter(new DoorTimerAdapter(*this)), iTimeout(timeout), isOpened(false) {}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    isOpened = true;
    adapter->Timeout(); // Запускаем таймер сразу при открытии
}

void TimedDoor::lock() {
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    if (isOpened) {
        throw std::runtime_error("Door is open!");
    }
}

void Timer::tregister(int timeout, TimerClient* client) {
    this->client = client;
    sleep(timeout);
    client->Timeout();
}

void Timer::sleep(int timeout) {
    std::this_thread::sleep_for(std::chrono::seconds(timeout));
}