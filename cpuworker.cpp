#include "cpuworker.h"
namespace Benchmarking {

cpuWorker::cpuWorker() : QObject()
{

}

QString cpuWorker::runBenchmark()
{
    QString result;
    auto* bench1 = new benchmark::cpu::whetstoneCpuBenchmark();
    bench1->initialize(INT32_MAX);
    bench1->setNrThreads(16);
    bench1->runAbsolute(true);
    result += "Wetstone\n";
    result += "Multithreaded performace: ";
    result += QString::number(bench1->getResult());
    result += "\n";
    bench1->setNrThreads(1);
    bench1->runAbsolute(false);
    result += "Singlethreaded performance: ";
    result += QString::number(bench1->getResult());
    result += "\n\n";
    auto* bench2 = new bench::Cpu::CpuMatrixMultiplication();
    bench2->setMatrixSize(2000);
    bench2->setNumThreads(6);
    bench2->initialize();
    bench2->runAbsoluteBenchmark(true);
    result += "Matrix Multiplication\n";
    result += "Multithreaded performace: ";
    result += QString::number(bench2->getResult());
    result += "\n";
    result += "Singlethreaded performance: ";
    bench2->runAbsoluteBenchmark(false);
    result += QString::number(bench2->getResult());
    delete bench2;

    return result;
}

} // namespace Benchmarking
