#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#if defined(ENABLE_FILE_INPUT_UNITTEST)
#include <fstream>
#endif

class BracketCalculator {
  public:
    BracketCalculator();
    ~BracketCalculator();
    void Calculate(const std::string& brackets);
    void PrintResults();

  private:
    int CalculationImpl();
    bool IsOpener(const char* bracket);
    bool IsMatching(const char* opener, const char* bracket);
    int ValueFrom(const char* bracket);

    int processing_index_;
    std::string processing_string_;
    std::vector<int> results_;
};

class InputStreamHelper {
  public:
    static std::unique_ptr<InputStreamHelper>
      GetInstance(int argc, char* argv[]);

    std::string ReadBracketString();
    int ReadIterationCount();

  private:
    InputStreamHelper(std::istream& istream);
    std::istream& input_stream_;
};

// Main Procedure
int main(int argc, char* argv[]) {
  std::unique_ptr<InputStreamHelper> istream_helper =
    InputStreamHelper::GetInstance(argc, argv);
  BracketCalculator bracket_calculator;

  int number_of_calculation = istream_helper->ReadIterationCount();

  for (int i = 0; i < number_of_calculation; i++) {
    std::string bracket_string = istream_helper->ReadBracketString();
    bracket_calculator.Calculate(bracket_string);
  }

  bracket_calculator.PrintResults();

  return 0;
}

///////////////////////////////////////////////////////
// BracketCalculator

BracketCalculator::BracketCalculator()
  : processing_index_(0),
    processing_string_() { }
BracketCalculator::~BracketCalculator() { }

void BracketCalculator::Calculate(std::string const& bracket_string) {
  processing_string_ = bracket_string;
  processing_index_ = 0;

  int result_for_this_calculation = 0;
  for (int end_index = processing_string_.length();
       processing_index_ < end_index;
       processing_index_++) {
    // CalculationImpl assume that processing_index_ indicate opener
    if (IsOpener(&processing_string_[processing_index_])) {
      int value_before_check = CalculationImpl();
      if (value_before_check == 0) {
        result_for_this_calculation = 0;
        break;
      } else {
        result_for_this_calculation
          = (result_for_this_calculation + value_before_check) % 100000000;
      }
    } else { // exception
      const char* exception_char = &processing_string_[processing_index_];
      // Check for Windows linefeed: if it is, actually this is not exception
      if (*exception_char != '\r')
        result_for_this_calculation = 0;
      break;
    }
  }

  results_.push_back(result_for_this_calculation);
}

void BracketCalculator::PrintResults() {
  for (int result : results_)
    std::cout << result << std::endl;
}

// private
int BracketCalculator::CalculationImpl() {
  const char* opener = &processing_string_[processing_index_];
  int value_for_this_level  = ValueFrom(opener);

  for (int value_for_inside = 0; true;) {
    const char* next_bracket = &processing_string_[++processing_index_];
    if (IsMatching(opener, next_bracket)) { // Close bracket (case: (.*), [.*], {.*})
      if (value_for_inside == 0) // atomic bracket
        return value_for_this_level;
      return (value_for_this_level * value_for_inside) % 100000000;
    } else if (IsOpener(next_bracket)) { // Another opener, do CalculationImpl again
      int value_before_check = CalculationImpl();
      if (value_before_check == 0) // Exception occur inside
        return 0;
      value_for_inside = (value_for_inside + value_before_check) % 100000000;
    } else { // Exception, failed bracket matching
      return 0;
    }
  }
}

bool BracketCalculator::IsOpener(const char* bracket) {
  return (*bracket == '(' | *bracket == '{' | *bracket == '[');
}

bool BracketCalculator::IsMatching(const char* opener, const char* bracket) {
  return (*opener + 1 == *bracket) | (*opener + 2 == *bracket);
}

int BracketCalculator::ValueFrom(const char* bracket) {
  switch(*bracket) {
    case '(':
      return 1;
      break;
    case '{':
      return 2;
      break;
    case '[':
      return 3;
      break;
    default:
      assert(false); // make sure opener
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

int InputStreamHelper::ReadIterationCount() {
  int number_of_calculation = 0;
  input_stream_ >> number_of_calculation;
  std::string dummy_string; // For eating linefeed characters
  std::getline(input_stream_, dummy_string);
  return number_of_calculation;
}

std::string InputStreamHelper::ReadBracketString() {
  std::string bracket_string;
  std::getline(input_stream_, bracket_string);
  return bracket_string;
}

// private
InputStreamHelper::InputStreamHelper(std::istream& istream) 
: input_stream_(istream) { }
