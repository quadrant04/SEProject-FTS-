class Frame {
public: 
    int fSlime;
    int fTower;
    double delay;

    Frame() {
        fSlime = 0;
        fTower = 0;
        delay = 0.25;
    }
};

class Timers {
public:
    double physicsRate;
    double oobillion;
    struct timespec timeStart, timeEnd, timeCurrent;
    struct timespec slimeTimer;
    struct timespec towerTimer;
    Timers() {
        physicsRate = 1.0 / 30.0;
        oobillion = 1.0 / 1e9;
    }
    double timeDiff(struct timespec *start, struct timespec *end) {
        return (double)(end->tv_sec - start->tv_sec ) +
                (double)(end->tv_nsec - start->tv_nsec) * oobillion;
    }
    void timeCopy(struct timespec *dest, struct timespec *source) {
        memcpy(dest, source, sizeof(struct timespec));
    }
    void recordTime(struct timespec *t) {
        clock_gettime(CLOCK_REALTIME, t);
    }
} timers;
