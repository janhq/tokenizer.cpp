#include "tokenizer.h"

namespace tokenizer_cpp {
SentencePieceTokenizer::SentencePieceTokenizer(const std::string& model_path) {
  auto status = processor_.Load(model_path);
  if (!status.ok()) {
    std::cerr << status.ToString() << std::endl;
  }
  std::cout << "Successully loaded the tokenizer" << std::endl;
}

std::string SentencePieceTokenizer::DecodeWithSpace(const int id) {
  std::string text = processor_.IdToPiece(id);
  ReplaceSubstring(text, "▁", " ");
  return text;
}

std::string SentencePieceTokenizer::Decode(const std::vector<int32_t> ids) {
  std::string text = processor_.DecodeIds(ids);
  return text;
}

std::vector<int> SentencePieceTokenizer::Encode(const std::string& input) {
  std::vector<int> ids;
  processor_.Encode(input, &ids);
  return ids;
}

void SentencePieceTokenizer::ReplaceSubstring(std::string& base,
                                              const std::string& from,
                                              const std::string& to) {
  size_t start_pos = 0;
  while ((start_pos = base.find(from, start_pos)) != std::string::npos) {
    base.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
}

class TFilePathResourceReader : public IResourceReader {
 public:
  TFilePathResourceReader(const std::string& path) : path_(path) {}

  std::vector<std::string> readLines() override {
    std::ifstream file(path_);
    if (!file.is_open()) {
      throw std::runtime_error("Embedded resource '" + path_ + "' not found.");
    }

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
      lines.push_back(line);
    }

    return lines;
  }

 private:
  std::string path_;
};

TiktokenTokenizer::TiktokenTokenizer(const std::string& model_path) {
  TFilePathResourceReader reader(model_path);
  encoder_ =
      GptEncoding::get_encoding_llama3(LanguageModel::CL100K_BASE, &reader);
  std::cout << "Successully loaded the tokenizer" << std::endl;
}

std::string TiktokenTokenizer::Decode(const std::vector<int32_t> ids) {
  std::string text = encoder_->decode(ids);
  return text;
}

std::vector<int> TiktokenTokenizer::Encode(const std::string& input) {
  std::vector<int> ids = encoder_->encode(input);
  return ids;
}

}  // namespace tokenizer_cpp