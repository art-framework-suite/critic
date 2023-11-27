#include "TFile.h"
#include "TTree.h"

#include <cassert>
#include <memory>

int main(int argc, char* argv[])
{
  assert(argc == 4);
  constexpr int threshold{500};
  std::unique_ptr<TFile> first{TFile::Open(argv[1])};
  std::unique_ptr<TFile> second{TFile::Open(argv[2])};
  std::unique_ptr<TFile> third{TFile::Open(argv[3])};
  assert(first->GetSize() >= threshold);
  assert(second->GetSize() >= threshold);

  // Check that the right number of aggregated events have been written across files.
  unsigned nevents{};
  nevents += first->Get<TTree>("Events")->GetEntries();
  nevents += second->Get<TTree>("Events")->GetEntries();
  nevents += third->Get<TTree>("Events")->GetEntries();
  assert(nevents == 4000);
}
