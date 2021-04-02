#include <exception>
#include <iostream>
#include <memory>

#include "SignalProcessor.h"
#include "SpikesObserver.h"
#include "catch.hpp"

// TEST_CASE("hello says hello")
// {
//   std::ostringstream out;
//   std::streambuf* coutbuf = std::cout.rdbuf();

//   std::cout.rdbuf(out.rdbuf()); // redirect cout to out
//   hello("tester");
//   hello("you");
//   std::cout.rdbuf(coutbuf);

//   REQUIRE(out.str() == "Hello, tester!\nHello, you!\n");
// }

class SignalProcessorTester : public SpikesObserver {
 private:
  const int BIT_ERROR = -1;
  const int BIT_END = -2;

  int _lastBitNum = -1;
  int _nBits = 0;
  int _spkErrors = 0;

 public:
  SignalProcessorTester(SignalProcessor& mod, double binTime)
      : SpikesObserver(mod, binTime) {}

  void update(double time) {
    if (floor(time / _binTime) < _lastBitNum) {
      throw -1;  // std::logic_error;
    }

    if (floor(time / _binTime) == _lastBitNum) {
      _spkErrors++;
    }

    if (floor(time / _binTime) > _lastBitNum) {
      // notify(_lastBit); // notify 1 to BitDetector(s)
      _nBits++;

      for (int i = 1; i < (floor(time / _binTime) - _lastBitNum); i++)
        // notify(0); // notify (several) 0s to BitDetector(s)
        _nBits++;

      _lastBitNum = floor(time / _binTime);
    }
  }

  int getNBits() { return _nBits; }

  int getSpkErrors() { return _spkErrors; }
};

TEST_CASE("Detect n spikes in SignalProcessor") {
  double binTime = 80.0;
  std::string testDataFilename = "files/test.dat";
  std::string testOutpFilename = "files/out.h5";
  std::string configFilename = "files/config.yaml";

  std::ifstream testFile(testDataFilename);
  REQUIRE(testFile.good());

  std::ifstream configFile(configFilename);
  REQUIRE(configFile.good());

  std::shared_ptr<ProblemConfig> config(
      new ProblemConfig(configFilename, testDataFilename, testOutpFilename));

  SignalProcessor sp(config);
  std::shared_ptr<SignalProcessorTester> tester(
      new SignalProcessorTester(sp, binTime));

  sp.attach(tester);
  sp.run();

  REQUIRE(tester->getSpkErrors() == 0);
  REQUIRE(tester->getSpkErrors() == 5);
}
