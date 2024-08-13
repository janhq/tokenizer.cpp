# Makefile for Cortex llamacpp engine - Build, Lint, Test, and Clean
.PHONY: all build package

BUILD_DEPS_CMAKE_EXTRA_FLAGS ?= ""
CMAKE_EXTRA_FLAGS ?= ""
RUN_TESTS ?= false
CODE_SIGN ?= false
AZURE_KEY_VAULT_URI ?= xxxx
AZURE_CLIENT_ID ?= xxxx
AZURE_TENANT_ID ?= xxxx
AZURE_CLIENT_SECRET ?= xxxx
AZURE_CERT_NAME ?= xxxx
DEVELOPER_ID ?= xxxx

# Default target, does nothing
all:
	@echo "Specify a target to run"

# Build the library
build:
ifeq ($(OS),Windows_NT)
	@powershell -Command "mkdir -p build; cd build; cmake .. $(CMAKE_EXTRA_FLAGS); cmake --build . --config Release -j4;"
else ifeq ($(shell uname -s),Linux)
	@mkdir -p build && cd build; \
	cmake .. $(CMAKE_EXTRA_FLAGS); \
	make -j4;
else
	@mkdir -p build && cd build; \
	cmake .. $(CMAKE_EXTRA_FLAGS); \
	make -j4;
endif

# TODO(sang) header file
pre-package:
ifeq ($(OS),Windows_NT)
	@powershell -Command "mkdir -p tokenizer;"
	@powershell -Command "cp build\Release\tokenizer.lib .\tokenizer\;"
else ifeq ($(shell uname -s),Linux)
	@mkdir -p tokenizer; \
	cp build/libtokenizer.a tokenizer/;
else
	@mkdir -p tokenizer; \
	cp build/libtokenizer.a tokenizer/;
endif

codesign:
ifeq ($(CODE_SIGN),false)
	@echo "Skipping Code Sign"
	@exit 0
endif

ifeq ($(OS),Windows_NT)
	@powershell -Command "dotnet tool install --global AzureSignTool;"
	@powershell -Command 'azuresigntool.exe sign -kvu "$(AZURE_KEY_VAULT_URI)" -kvi "$(AZURE_CLIENT_ID)" -kvt "$(AZURE_TENANT_ID)" -kvs "$(AZURE_CLIENT_SECRET)" -kvc "$(AZURE_CERT_NAME)" -tr http://timestamp.globalsign.com/tsa/r6advanced1 -v ".\tokenizer\tokenizer.dll";'
else ifeq ($(shell uname -s),Linux)
	@echo "Skipping Code Sign for linux"
	@exit 0
else
	find "tokenizer" -type f -exec codesign --force -s "$(DEVELOPER_ID)" --options=runtime {} \;
endif

package:
ifeq ($(OS),Windows_NT)
	@powershell -Command "7z a -ttar temp.tar tokenizer\*; 7z a -tgzip tokenizer.tar.gz temp.tar;"
else ifeq ($(shell uname -s),Linux)
	tar -czvf tokenizer.tar.gz tokenizer;
else
	tar -czvf tokenizer.tar.gz tokenizer;
endif

clean:
ifeq ($(OS),Windows_NT)
	@powershell -Command "rm -rf build; rm -rf build-deps; rm -rf tokenizer; rm -rf tokenizer.tar.gz;"
else ifeq ($(shell uname -s),Linux)
	@rm -rf build; rm -rf build-deps; rm -rf tokenizer; rm -rf tokenizer.tar.gz;
else
	@rm -rf build; rm -rf build-deps; rm -rf tokenizer; rm -rf tokenizer.tar.gz;
endif