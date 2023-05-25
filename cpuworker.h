#ifndef BENCHMARKING_CPUWORKER_H
#define BENCHMARKING_CPUWORKER_H

#include <QObject>
#include "benchmarks/include/benchmark/cpu/whetstoneCpuBenchmark.h"
#include "benchmarks/include/benchmark/cpu/CpuMatrixMultiplication.h"

namespace Benchmarking {

class cpuWorker : public QObject
{
    Q_OBJECT
public:
    cpuWorker();
    QString runBenchmark();
};

} // namespace Benchmarking

#endif // BENCHMARKING_CPUWORKER_H
