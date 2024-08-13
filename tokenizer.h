#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include "cpp-tiktoken/emdedded_resource_reader.h"
#include "cpp-tiktoken/encoding.h"
#include "sentencepiece/src/sentencepiece_processor.h"

namespace tokenizer_cpp {
// Class Tokenizer is not thread-safe
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

class Tokenizer {

 public:
  Tokenizer() {}
  virtual ~Tokenizer() {}

  virtual std::string DecodeWithSpace(const int id) { return ""; }

  virtual std::string Decode(const std::vector<int32_t> ids) = 0;

  virtual std::vector<int> Encode(const std::string& input) = 0;
};

class SentencePieceTokenizer : public Tokenizer {

 public:
  SentencePieceTokenizer(const std::string& model_path) : Tokenizer() {
    auto status = processor.Load(model_path);
    if (!status.ok()) {
      std::cerr << status.ToString() << std::endl;
    }
    std::cout << "Successully loaded the tokenizer" << std::endl;
  }

  std::string DecodeWithSpace(const int id) override {
    std::string text = processor.IdToPiece(id);
    ReplaceSubstring(text, "▁", " ");
    return text;
  }

  std::string Decode(const std::vector<int32_t> ids) override {
    std::string text = processor.DecodeIds(ids);
    return text;
  }

  std::vector<int> Encode(const std::string& input) override {
    std::vector<int> ids;
    processor.Encode(input, &ids);
    return ids;
  }

 private:
  void ReplaceSubstring(std::string& base, const std::string& from,
                        const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = base.find(from, start_pos)) != std::string::npos) {
      base.replace(start_pos, from.length(), to);
      start_pos += to.length();
    }
  }

 private:
  sentencepiece::SentencePieceProcessor processor;
};

class TiktokenTokenizer : public Tokenizer {
 public:
  TiktokenTokenizer(const std::string& model_path) : Tokenizer() {
    TFilePathResourceReader reader(model_path);
    encoder =
        GptEncoding::get_encoding_llama3(LanguageModel::CL100K_BASE, &reader);
    std::cout << "Successully loaded the tokenizer" << std::endl;
  }

  std::string Decode(const std::vector<int32_t> ids) override {
    std::string text = encoder->decode(ids);
    return text;
  }

  std::vector<int> Encode(const std::string& input) override {
    std::vector<int> ids = encoder->encode(input);
    return ids;
  }

 private:
  std::shared_ptr<GptEncoding> encoder;
};

}  // namespace tokenizer_cpp