#ifndef CLOUD_SCHEDULER_EVALUATION_TASK_H
#define CLOUD_SCHEDULER_EVALUATION_TASK_H

struct Task {
    int id;
    double arrival_time;
    double cpu_required;
    double ram_required;
    double duration;
    double start_time = -1.0;
};

#endif  // CLOUD_SCHEDULER_EVALUATION_TASK_H
