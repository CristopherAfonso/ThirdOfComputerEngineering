#include <QCoreApplication>
#include <QVector>
#include <atomic>
#include <iostream>
#include <math.h>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <QElapsedTimer>
#include <QtCore/QReadWriteLock>

// https://stackoverflow.com/questions/7988486/how-do-you-calculate-the-variance-median-and-standard-deviation-in-c-or-java/7988556#7988556

QVector<float> d(60 * 60 * 24); ///< Here we save all the measurements of a single day
unsigned int total = 60 * 60 * 24 * 365; ///< sec * min * hour * days

std::mutex mtx; ///< This mutex will be used to protect the shared variable
std::condition_variable cv; ///< This condition variable will synchronize threads, preventing them from writing or reading when they shouldn't.

const int bufferSize{30}; ///< 30 positions because it's a month, one position per day
QVector<QVector<float>> bufferMonth(bufferSize);

int in{0}; ///< Write index in buffer
std::atomic<int> out{0}; ///< Read index in buffer


QElapsedTimer timer; ///< It will be our timer to count the time in each iteration of each method to solve the problem
qint64 time_serial = Q_INT64_C(0); ///< We set to zero
qint64 time_producer_consumer = Q_INT64_C(0); ///< We set to zero
qint64 time_reader_writer = Q_INT64_C(0); ///< We set to zeros
std::vector<std::pair<double, int>> acc_num_measurements = {std::pair<double, int>(0.0, 0), std::pair<double, int>(0.0, 0), std::pair<double, int>(0.0, 0)};

class Statistics { ///< This class allows us to save in a single vector, all the temperature measurements in each second of a processor and apply statistical calculations to those measurements.
 public:
  Statistics(const QVector<float>& data) {
    this->data = data;
    size = data.length();
  }

  double getMean(void) {
    double sum = 0.0;

    for(double a : data)
      sum += a;

    return sum / size;
  }

  double median(void) {
    //Arrays.sort(data);
    std::sort(data.begin(), data.end());

    if (data.length() % 2 == 0)
      return (data[(data.length() / 2) - 1] + data[data.length() / 2]) / 2.0;

    return data[data.length() / 2];
  }

 private:
  QVector<float> data; ///< In each position we store a temperature measurement
  int size; ///< We save the vector size to speed up operations
};

void serialMethod(void) {
  std::cout << "Serial mode start:" << std::endl;
  time_serial = timer.nsecsElapsed(); ///< We run the chronometer
  const int kSecondsInADay{24 * 3600};

  for(unsigned int i{0}; i <= total; ++i) { ///< We go through all the seconds that a year has
    d[i % (kSecondsInADay)] = (random() % 50 + 50); ///< We put a random temperature measurement in the position that corresponds to the vector

    if(i % (kSecondsInADay) == 0) { ///< If we fill in all the data for a day, we make the measurements.
      Statistics s(d);
      std::cout << i << " of " << total << " " << "average temperature " << s.getMean() << std::endl;
      std::cout << i << " of " << total << " " << "median " << s.median() << std::endl;
    }
  }

  time_serial = timer.nsecsElapsed() - time_serial;  ///< We stop the chronometer
  std::cout << "------------------------------------------------------------------------------\n";
  std::cout << "Done in serial mode\n\n";
}

void producerThreadMethod(void) {
  for(unsigned int i{0}; i <= total; ++i) { ///< We go through all the seconds that a year has
    d[i % (24 * 3600)] = (random() % 50 + 50); ///< We put a random temperature measurement at the end of the vector

    if(i % (24 * 3600) == 0) { ///< If we fill in all the data for a day, we make the measurements.
      std::unique_lock<std::mutex> lock(mtx); ///< We acquired the Mutex

      while (((in + 1) % bufferSize) == out) { ///< If the next position is being reading, we wait
        cv.wait(lock); ///< We wait for space in the buffer
      }

      bufferMonth[in] = d; ///< We add the data to the buffer
      in = (in + 1) % bufferSize; ///< We calculate the new index for the next iteration
      cv.notify_all(); ///< We tell the rest of the consumer threads that there is new data
    }
  }
}

std::atomic<int> days_processed{0};
void consumerMeanAndMedianThreadMethod(const int& id) {
  for(days_processed; days_processed < 365; ++days_processed) { ///< We read a fact for each day of the year
    std::unique_lock<std::mutex> lock(mtx); ///< We acquired the Mutex

    while (in == out) { ///< If there is no data to read, we wait
      cv.wait(lock); ///< We must wait until data is available in the buffer
    }

    Statistics s(bufferMonth[out]); ///< We create the object that will make the calculations we need
    std::cout << "[" << id << "] " << (days_processed * 86400) << " of " << total << " average temperature " << s.getMean() << std::endl;
    std::cout << "[" << id << "] " << (days_processed * 86400) << " of " << total << " median " << s.median() << std::endl;
    out = (out + 1) % bufferSize; ///< We update the reading index
    cv.notify_all(); ///< Notify the producer that buffer space is available
  }
}

void producerConsumerMethod(void) {
  std::cout << "Producer and Consumer mode start:" << std::endl;
  time_producer_consumer = timer.nsecsElapsed(); ///< We run the chronometer
  std::thread producer(producerThreadMethod);
  std::thread consumer1(consumerMeanAndMedianThreadMethod, 1);
  std::thread consumer2(consumerMeanAndMedianThreadMethod, 2);
  producer.join();
  consumer1.join();
  consumer2.join();
  days_processed = 0; ///< If we want to use this mood again, we must reset the critical data.
  time_producer_consumer = timer.nsecsElapsed() - time_producer_consumer; ///< We stop the chronometer
  std::cout << "------------------------------------------------------------------------------\n";
  std::cout << "Done in Producer and Consumer mode\n\n";
}

std::mutex output_mutex;
QReadWriteLock rw_lock;
//std::shared_mutex shared_mutex;
//std::shared_lock<std::shared_mutex> shared_lock;
//std::condition_variable_any cond_var;
std::atomic<int> current_day{0};
std::atomic<bool> reader_work_finished[2] = {true, true};
void writerThreadMethod() {
  for (; current_day < 365; ++current_day) { ///during an entire year
    //rw_lock.lockForWrite();
    //std::lock_guard lock(shared_mutex);
    for (int day_temp = 0; day_temp < 86400; ++day_temp) {
      d[day_temp] = (random() % 50 + 50); ///< We put a random temperature measurement in the position of the vector that contains the measurements of the current day
    }

    //cond_var.notify_all();
    //cond_var.wait(lock);
    //reader_work_finished[0] = reader_work_finished[1] = false;
    //cv_b.notify_all();
    //cv_b.wait(lock);
    //if (current_day < 365) while (!reader_work_finished[0] || !reader_work_finished[1]); ///wait until both readers read the whole day
    //rw_lock.unlock();
    reader_work_finished[0] = reader_work_finished[1] = false;

    while (!(reader_work_finished[0] && reader_work_finished[1]));
  }
}

void readerMedianThreadMethod() {
  while(current_day < 365) { ///< We read a fact for each day of the year
    if (!reader_work_finished[0]) {
      //rw_lock.lockForRead();
      //while (reader_work_finished[0]) cv_b.wait(shared_lock); ///< If there is no data to read, we wait
      //std::shared_lock lock(shared_mutex, std::defer_lock);
      //cond_var.wait(lock);
      //lock.lock();
      Statistics s(d); ///< We create the object that will make the calculations we need
      auto median = s.median();
      auto day = (int)current_day; ///do this in order to prevent collisions with writer
      //reader_work_finished[0] = true;
      //lock.unlock();
      //rw_lock.unlock();
      reader_work_finished[0] = true;
      output_mutex.lock(); ///only block output
      std::cout << (day * 86400) << " of " << total << " median " << median << std::endl;
      output_mutex.unlock();
      //if (reader_work_finished[0] && reader_work_finished[1]) cv_b.notify_all();
    }
  }
}

void readerMeanThreadMethod() {
  while(current_day < 365) { ///< We read a fact for each day of the year
    if (!reader_work_finished[1]) {
      //rw_lock.lockForRead();
      //while (reader_work_finished[1]) cv_b.wait(shared_lock); ///< If there is no data to read, we wait
      //std::shared_lock lock(shared_mutex, std::defer_lock);
      //cond_var.wait(lock);
      //lock.lock();
      double sum = 0.0; ///We can skip one copy of the vector so that this solution to the problem is equal to the others

      for(int i = 0; i < d.size(); ++i) ///Just access vector d with the temperature data directly
        sum += d[i];

      double mean = sum / d.size();
      auto day = (int)current_day; ///do this in order to prevent collisions with writer
      //reader_work_finished[1] = true;
      //lock.unlock();
      //rw_lock.unlock();
      reader_work_finished[1] = true;
      output_mutex.lock(); ///only block output
      std::cout << (day * 86400) << " of " << total << " average temperature " << mean << std::endl;
      output_mutex.unlock();
      //if (reader_work_finished[0] && reader_work_finished[1]) cv_b.notify_all();
    }
  }
}

void readersWritersMethod(void) {
  std::cout << "Readers and Writers mode start:" << std::endl;
  time_reader_writer = timer.nsecsElapsed(); ///< We run the chronometer
  current_day = 0;
  reader_work_finished[0] = reader_work_finished[1] = true;
  std::thread writer(writerThreadMethod);
  std::thread reader_median(readerMedianThreadMethod);
  std::thread reader_mean(readerMeanThreadMethod);
  writer.join();
  reader_median.join();
  reader_mean.join();
  days_processed = 0; ///< If we want to use this mood again, we must reset the critical data.
  time_reader_writer = timer.nsecsElapsed() - time_reader_writer; ///< We stop the chronometer
  std::cout << "------------------------------------------------------------------------------\n";
  std::cout << "Done in Readers and Writers mode\n\n";
}

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  std::string election = "0"; ///< How to run the program

  while(true) {
    in = 0; ///< We set the index to a start position
    out = 0; ///< We set the index to a start position
    std::cout << "How do you want to run the program?" << std::endl;
    std::cout << "Exit(0), Sequential(1), Producers/Consumers(2), Readers/Writers(3)" << std::endl;
    std::cout << "Answer? ";
    std::cin >> election;
    std::cout << std::endl;

    /// If we choose a invalid option, the programme ask to us again
    while(election != "0" && election != "1" && election != "2" && election != "3") {
      std::cout << "Invalid option, try again!" << std::endl;
      std::cout << "Answer? ";
      std::cin >> election;
      std::cout << std::endl;
    }

    if(election == "0") break; ///< We stop the programme

    int num_exec{0}; ///< Times to execute the selected mode

    do {
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
      }

      std::cout << "Enter number of times this method should be executed: ";
    } while (!(std::cin >> num_exec) || num_exec <= 0); ///< If the times to execute is wrong, we ask it again

    std::cout << std::endl; ///< It is to the terminal looks better :)

    if(election == "1") {
      for (int i{0}; i < num_exec; ++i) {
        serialMethod(); ///< We execute the selected mode going to the concret function
        acc_num_measurements[0].first += double(time_serial) / 1000000000; ///< We get the all measurements in a variable in seconds
        ++acc_num_measurements[0].second; ///< We store the times to this mode has been used
      }
    }

    if(election == "2") {
      for (int i{0}; i < num_exec; ++i) {
        producerConsumerMethod();  ///< We execute the selected mode going to the concret function
        acc_num_measurements[1].first += double(time_producer_consumer) / 1000000000; ///< We get the all measurements in a variable in seconds
        ++acc_num_measurements[1].second; ///< We store the times to this mode has been used
      }
    }

    if(election == "3") {
      for (int i{0}; i < num_exec; ++i) {
        readersWritersMethod();  ///< We execute the selected mode going to the concret function
        acc_num_measurements[2].first += double(time_reader_writer) / 1000000000; ///< We get the all measurements in a variable in seconds
        ++acc_num_measurements[2].second; ///< We store the times to this mode has been used
      }
    }

    election = "0";
    /// We show the relevant data of serial mode
    std::cout << "Last Time Serial: " << (double(time_serial) / 1000000000) << " seconds - Average: "
              << ((acc_num_measurements[0].second != 0) ? acc_num_measurements[0].first / acc_num_measurements[0].second : 0.0)
              << ", Times Executed: " << acc_num_measurements[0].second << std::endl;
    /// We show the relevant data of producer and consumer mode
    std::cout << "Last Time P/C:    " << (double(time_producer_consumer) / 1000000000) << " seconds - Average: "
              << ((acc_num_measurements[1].second != 0) ? acc_num_measurements[1].first / acc_num_measurements[1].second : 0.0)
              << ", Times Executed: " << acc_num_measurements[1].second  << std::endl;
    /// We show the relevant data of reader and writer mode
    std::cout << "Last Time R/W:    " << (double(time_reader_writer) / 1000000000) << " seconds - Average: "
              << ((acc_num_measurements[2].second != 0) ? acc_num_measurements[2].first / acc_num_measurements[2].second : 0.0)
              << ", Times Executed: " << acc_num_measurements[2].second  << std::endl << std::endl;
  }

  return 0;
}
