#include "cpuworker.h"
namespace Benchmarking {

cpuWorker::cpuWorker() : QObject()
{

}

QString cpuWorker::runBenchmark()
{
    QString result;
    auto* bench = new benchmark::cpu::whetstoneCpuBenchmark();
    bench->initialize(4);
    bench->setNrThreads(4);
    bench->runAbsolute(false);
    result += QString::number(bench->getResult());
    return result;
}

} // namespace Benchmarking
