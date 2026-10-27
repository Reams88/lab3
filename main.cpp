#include <iostream>
#include <thread>
#include <latch>
#include <syncstream>
#include <chrono>
#include <format>

using namespace std;
using namespace std::chrono_literals;

constexpr int nt = 5;

void f(char x, int i)
{
  this_thread::sleep_for(1s);
  osyncstream(cout) << format("З набору {} виконано дію {}.\n", x, i);
}

latch latch_c(nt);
latch latch_d(nt);
latch latch_top(nt);
latch latch_bot(nt);

void w1()
{
  f('c', 1);
  latch_c.count_down();

  f('d', 1);
  f('d', 6);
  latch_d.arrive_and_wait();

  f('g', 1);
  f('g', 6);
  f('a', 3);
  latch_top.count_down();

  f('b', 2);

  latch_c.wait();
  f('h', 1);
  f('h', 6);
  f('i', 5);
  latch_bot.arrive_and_wait();

  f('k', 1);

  latch_top.wait();
  f('j', 5);
}

void w2()
{
  f('c', 2);
  latch_c.count_down();

  f('d', 2);
  latch_d.count_down();

  latch_c.wait();
  f('e', 1);

  latch_d.wait();
  f('g', 2);
  f('g', 7);
  f('a', 4);
  latch_top.count_down();

  f('b', 3);
  f('h', 2);
  f('i', 1);
  f('i', 6);
  latch_bot.arrive_and_wait();

  f('k', 2);

  latch_top.wait();
  f('j', 6);
}

void w3()
{
  f('c', 3);
  latch_c.count_down();

  f('d', 3);
  latch_d.count_down();

  latch_c.wait();
  f('e', 2);

  latch_d.wait();
  f('g', 3);
  f('g', 8);
  f('a', 5);
  latch_top.count_down();

  f('b', 4);
  f('h', 3);
  f('i', 2);
  latch_bot.count_down();

  latch_top.wait();
  f('j', 1);

  latch_bot.wait();
  f('k', 3);
}

void w4()
{
  f('c', 4);
  latch_c.count_down();

  f('d', 4);
  latch_d.count_down();

  latch_c.wait();
  f('e', 3);

  latch_d.wait();
  f('g', 4);
  f('a', 1);
  f('a', 6);
  latch_top.count_down();

  f('b', 5);
  f('h', 4);
  f('i', 3);
  latch_bot.count_down();

  latch_top.wait();
  f('j', 2);

  latch_bot.wait();
  f('k', 4);
}

void w5()
{
  f('c', 5);
  latch_c.count_down();

  f('d', 5);
  latch_d.count_down();

  latch_c.wait();
  f('e', 4);

  latch_d.wait();
  f('g', 5);
  f('a', 2);
  latch_top.count_down();

  f('b', 1);
  f('b', 6);
  f('h', 5);
  f('i', 4);
  latch_bot.count_down();

  latch_top.wait();
  f('j', 3);
  f('j', 4);
}

int main()
{
  osyncstream(cout) << "Обчислення розпочато.\n";

  auto start_time = chrono::steady_clock::now();

  {
    jthread t1(w1);
    jthread t2(w2);
    jthread t3(w3);
    jthread t4(w4);
    jthread t5(w5);
  }

  auto end_time = chrono::steady_clock::now();
  auto duration_sec = chrono::duration<double>(end_time - start_time).count();

  osyncstream(cout) << format("Обчислення завершено. Тривалість обчислення {:.3f} секунд.\n", duration_sec);

  return 0;
}
