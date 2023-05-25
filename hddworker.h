#ifndef BENCHMARKING_HDDWORKER_H
#define BENCHMARKING_HDDWORKER_H

#include <QObject>
#include "benchmarks/include/benchmark/cpu/whetstoneCpuBenchmark.h"
#include "benchmarks/include/benchmark/cpu/CpuMatrixMultiplication.h"

namespace Benchmarking {

class hddWorker : public QObject
{
public:
    hddWorker();
    QString runBenchmark();
};

} // namespace Benchmarking

#endif // BENCHMARKING_HDDWORKER_H
