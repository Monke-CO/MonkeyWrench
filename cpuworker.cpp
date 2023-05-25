#include "cpuworker.h"
namespace Benchmarking {

cpuWorker::cpuWorker() : QObject()
{

}

QString cpuWorker::runBenchmark()
{
    QString result;
    auto* bench = new benchmark::cpu::whetstoneCpuBenchmark();
    bench->initialize(INT32_MAX);
    bench->setNrThreads(16);
    bench->runAbsolute(true);
    result += "Multithreaded performace: ";
    result += QString::number(bench->getResult());
    result += "\n";
    bench->setNrThreads(1);
    bench->runAbsolute(false);
    result += "Singlethreaded performance: ";
    result += QString::number(bench->getResult());

    return result;
}

} // namespace Benchmarking
