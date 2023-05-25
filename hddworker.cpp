#include "hddworker.h"
#include "benchmark/hdd/hddWriteBenchmark.h"
#include "benchmark/hdd/hddReadBenchmark.h"

namespace Benchmarking {

hddWorker::hddWorker() : QObject()
{

}

QString hddWorker::runBenchmark()
{
    QString result;
    auto* bench1 = new benchmark::hdd::HDDWriteSpeedBenchmark;
    bench1->setMinIndex(1);
    bench1->setMaxIndex(8);
    bench1->setPrefix("data-files/data");
    bench1->setSuffix(".dat");
    bench1->run("fs");
    result+= "Fixed size write speed: ";
    result += QString::number(bench1->getResult()) + " MB/s\n";
    result += "\n";
    bench1->run("fb");
    result+= "Fixed buffer write speed: ";
    result += QString::number(bench1->getResult()) + " MB/s\n";
    result += "\n";
    delete bench1;
    auto* bench2 = new Bench::HDD::HddReadBenchmark;
    bench2->initialize(1024LL*1024LL*1024LL*16);
    bench2->setItterations(1);
    bench2->setBufferSize(1024*4);
    bench2->setFileName("read-data.dat");
    bench2->run();
    result += "Read speed: ";
    result += QString::number(bench2->getResult()) + " MB/s\n";
    delete bench2;
    return result;
}

} // namespace Benchmarking
