#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>

#if defined(ENABLE_FILE_INPUT_UNITTEST)
#include <fstream>
#endif

// VoteTracker is for:
//   Tracking most popular(=voted) people, and print who they are
//
// Implementation:
//   For performance on large-scale input, unordered_map is used
//   to store vote number for given name(=string)
//
// Note 1, Do not data iteration after votes ends
//   Continuously update most popular people list during voting

class VoteTracker {
  public:
    VoteTracker();
    ~VoteTracker() {}

    void AddVote(std::string const& name);
    void PrintMostVotedPeople() const;

  private:
    void UpdateVoteStatus(std::string const& name, int vote);

    int most_voted_value_;
    std::set<std::string> most_voted_list_;
    std::unordered_map<std::string, int> name_to_vote_map_;
};

class InputStreamHelper {
  public:
    static std::unique_ptr<InputStreamHelper>
      GetInstance(int argc, char* argv[]);
    std::string ReadLine();

  private:
    InputStreamHelper(std::istream& istream);
    std::istream& input_stream_;
};

// Main Procedure
int main(int argc, char* argv[]) {
  std::unique_ptr<InputStreamHelper> istream_helper =
    InputStreamHelper::GetInstance(argc, argv);
  VoteTracker vote_tracker;

  std::string line = istream_helper->ReadLine();
  while (!line.empty()) { // Accept input until EOF (== Ctrl+D) or File end
    vote_tracker.AddVote(line);
    line = istream_helper->ReadLine();
  }
  vote_tracker.PrintMostVotedPeople();

  return 0;
}


///////////////////////////////////////////////////////
// VoteTracker 
VoteTracker::VoteTracker()
  : most_voted_value_(0) { }

void VoteTracker::AddVote(std::string const& name) { 
  std::unordered_map<std::string, int>::iterator
    found = name_to_vote_map_.find(name);

  if (found == name_to_vote_map_.end()) { // Never Voted
    name_to_vote_map_.insert(std::make_pair(name, 1));
//    std::cout << "\t>> New: " << name << std::endl;
    if (most_voted_value_ <= 1) // TODO(jongmin): consider optimizing, 
      UpdateVoteStatus(name, 1);
  } else {
//    std::cout << "\t>> Voted: " << name << ", current: " << found->second+1 << std::endl;
    found->second++; // Increment vote count
    if (found->second >= most_voted_value_)
      UpdateVoteStatus(found->first, found->second);
  }
}

void VoteTracker::PrintMostVotedPeople() const {
  for (auto& name : most_voted_list_)
    std::cout << name << std::endl;
}

// private
void VoteTracker::UpdateVoteStatus(std::string const& name, int vote) {
  assert(vote >= most_voted_value_);

  if (vote > most_voted_value_) {
    most_voted_value_ = vote;
    most_voted_list_.clear();
    most_voted_list_.insert(name);
  } else { // make sure: vote == most_voted_value_
    most_voted_list_.insert(name);
  }
}

///////////////////////////////////////////////////////
// InputStreamHelper 

std::unique_ptr<InputStreamHelper>
  InputStreamHelper::GetInstance(int argc, char* argv[]) {
#if !defined(ENABLE_FILE_INPUT_UNITTEST)
  std::unique_ptr<InputStreamHelper>
    istream_helper(new InputStreamHelper(std::cin));
  return std::move(istream_helper);
#else
  assert(argc >= 2); // make sure there should be filename
  std::string file_path(argv[1]);

  // TODO(jongmin): in this mode, below makes memory leaks until program end
  std::ifstream* input_stream = new std::ifstream(file_path);
  std::unique_ptr<InputStreamHelper>
    istream_helper(new InputStreamHelper(*input_stream));
  return std::move(istream_helper);
#endif
}

std::string InputStreamHelper::ReadLine() {
  std::string line;
  std::getline(input_stream_, line);
  return line;
}

// private
InputStreamHelper::InputStreamHelper(std::istream& istream) 
: input_stream_(istream) { }
