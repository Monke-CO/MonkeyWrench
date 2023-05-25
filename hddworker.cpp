#include "hddworker.h"
#include "benchmark/hdd/hddWriteBenchmark.h"

namespace Benchmarking {

hddWorker::hddWorker() : QObject()
{

}

QString hddWorker::runBenchmark()
{
    QString result;
    auto* bench = new benchmark::hdd::HDDWriteSpeedBenchmark();
    bench->run("fs");
    result += QString::number(bench->getResult());
    return result;
}

} // namespace Benchmarking
