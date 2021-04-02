//      pexHist.cpp
//
//      Copyright 2015 Ángel Lareo <angel.lareo@gmail.com>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ErrorFilter.h"
#include "HDF5HistWriter.h"
#include "HDF5TransitionsWriter.h"
#include "SignalProcessor.h"
#include "WordHistGenerator.h"

using namespace std;

static void show_usage(string name) {
  std::cerr << "Usage: " << name << " <option(s)> SOURCES" << endl
            << "Options:\n"
            << "\t-c,--config\t\tConfig file.\n"
            << "\t-h,--help\t\tShow this message.\n"
            << "\t-i,--input\t\tInput file.\n"
            << "\t-o,--output\t\tOutput file\n"
            << endl;
}

int main(int argc, char* argv[]) {
  int c;
  string configFilename, inputFilename, outputFilename;
  vector<shared_ptr<ErrorFilter> > EFilters;
  vector<shared_ptr<BinSignalGenerator> > BinSignalGens;
  vector<shared_ptr<WordHistGenerator> > HistGens;
  vector<float> binTimes;
  shared_ptr<ProblemConfig> info;

  while ((c = getopt(argc, argv, "hc:i:o:")) != -1) {  //@todo: use long options
    switch (c) {
      case 'h':
        show_usage(argv[0]);
        break;
      case 'c':
        configFilename += optarg;
        break;
      case 'i':
        inputFilename += optarg;
        break;
      case 'o':
        outputFilename += optarg;
        break;
    }
  }

  if (configFilename.empty()) {
    cerr << "Error: Required CONFIG file" << endl;
    show_usage(argv[0]);
  }

  ifstream test(configFilename);
  if (!test.good()) {
    cerr << "Error: Cannot find CONFIG file" << endl;
    return 0;
  }
  test.close();
  test.open(inputFilename);
  if (!test.good()) {
    cerr << "Error: Cannot find INPUT file" << endl;
    return 0;
  }

  //@todo: detect if outputFile exists
  //@todo: if outputFile exists -> rename

  if (outputFilename.empty()) {
    info = make_shared<ProblemConfig>(ProblemConfig(configFilename));
  } else {
    info = make_shared<ProblemConfig>(
        ProblemConfig(configFilename, inputFilename, outputFilename));
  }

  HDF5HistWriter H5FileWriter(info->getOutputFileName(), info->getWordLengths(),
                              info->getBinTimes());

  H5FileWriter.writeProblemInfo(info);

  SignalProcessor sp(info);

  for (double bT : info->getBinTimes()) {
    shared_ptr<ErrorFilter> errorFilter =
        make_shared<ErrorFilter>(bT, info->getTotalTime(), sp);
    sp.attach(errorFilter);
    EFilters.push_back(errorFilter);
    for (int wL : info->getWordLengths()) {
      shared_ptr<BinSignalGenerator> binSignalGen =
          make_shared<BinSignalGenerator>(wL, errorFilter);
      errorFilter->attach(binSignalGen);
      BinSignalGens.push_back(binSignalGen);
      shared_ptr<WordHistGenerator> wordHistGen =
          make_shared<WordHistGenerator>(binSignalGen);
      binSignalGen->attach(wordHistGen);
      HistGens.push_back(wordHistGen);
    }
  }

  sp.run();

  for (auto histGen : HistGens) {
    int nBits = histGen->getSubject()->getWordLength();
    float binTime = histGen->getSubject()->getBinTime();
    auto hist = histGen->getHist();

    hsize_t histDims[2];
    histDims[0] = histGen->getSubject()->getMaxWords();
    histDims[1] = 2;

    pair<float, int> par(binTime, nBits);

    H5FileWriter.writeHistData(hist, par, histDims);
    H5FileWriter.writeEntropy(histGen->getEntropy() / nBits, binTime, nBits);
    // H5FileWriter.writeBias(histGen->getBias(),
    // histGen->getBinTime(), histGen->getSubject()->getWordLength());
    // H5FileWriter.writeCorrectedEntropy(histGen->getCorrectedEntropy(),
    // histGen->getBinTime(), histGen->getSubject()->getWordLength());
  }

  for (auto eFilter : EFilters) {
    H5FileWriter.writeErrors(eFilter->getSpkErrors(), eFilter->getBitErrors(),
                             eFilter->getBinTime());
  }

  return 0;
}
