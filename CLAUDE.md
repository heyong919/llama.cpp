# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

llama.cpp is a C/C++ implementation for running Large Language Models (LLMs) locally with minimal setup and state-of-the-art performance. The project aims to enable LLM inference on a wide range of hardware with plain C/C++ implementation and no external dependencies.

## Key Architecture Components

### Core Libraries
- **ggml/**: Core tensor operations library that provides the computational foundation
  - `ggml/include/ggml.h`: Main tensor library interface with automatic differentiation
  - `ggml/src/ggml-cpu/`: CPU backend implementation
  - `ggml/src/ggml-cuda/`: CUDA backend for NVIDIA GPUs
  - `ggml/src/ggml-metal/`: Metal backend for Apple Silicon
  - `ggml/src/ggml-vulkan/`: Vulkan backend for cross-platform GPU support
  - `ggml/src/ggml-mlu/`: MLU backend for Cambricon devices (recently added)

- **src/**: Main llama.cpp library implementation
  - `src/llama.cpp`: Main library implementation
  - `src/llama-*.cpp`: Modular components (vocab, model, sampling, etc.)
  - `include/llama.h`: C API interface for the library

- **common/**: Shared utilities and helper functions
  - `common/common.h`: Common parameters and utilities
  - `common/sampling.cpp`: Sampling algorithms
  - `common/chat.cpp`: Chat template handling

### Tools and Examples
- **tools/**: Production-ready tools
  - `tools/main/`: llama-cli (main inference tool)
  - `tools/server/`: llama-server (OpenAI-compatible HTTP server)
  - `tools/quantize/`: Model quantization tool
  - `tools/perplexity/`: Perplexity evaluation tool

- **examples/**: Example applications demonstrating various use cases
  - `examples/simple/`: Minimal example for developers
  - `examples/batched/`: Batched inference example
  - `examples/embedding/`: Text embedding example

## Build System

### CMake (Primary Build System)
The project uses CMake as the primary build system with extensive configuration options.

#### Basic Build Commands
```bash
# CPU-only build
cmake -B build
cmake --build build --config Release

# CUDA build
cmake -B build -DGGML_CUDA=ON
cmake --build build --config Release

# Metal build (macOS)
cmake -B build -DGGML_METAL=ON
cmake --build build --config Release

# Debug build
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

#### Key CMake Options
- `GGML_CUDA`: Enable CUDA support
- `GGML_METAL`: Enable Metal support (macOS)
- `GGML_VULKAN`: Enable Vulkan support
- `GGML_MLU`: Enable MLU support (Cambricon)
- `GGML_OPENCL`: Enable OpenCL support
- `LLAMA_BUILD_TESTS`: Build test suite
- `LLAMA_BUILD_TOOLS`: Build tools
- `LLAMA_BUILD_EXAMPLES`: Build examples
- `BUILD_SHARED_LIBS`: Build shared libraries

### Makefile (Legacy, Deprecated)
The Makefile build system is deprecated. Use CMake instead.

## Testing Infrastructure

### Test Organization
Tests are organized in the `tests/` directory with different categories:
- **Unit tests**: `test-*.cpp` files testing specific components
- **Integration tests**: Testing full workflows
- **Backend tests**: Testing specific backend implementations (e.g., MLU tests)
- **Tokenizer tests**: Comprehensive tokenization testing

### Running Tests
```bash
# Build and run all tests
cmake -B build -DLLAMA_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build

# Run specific test
./build/bin/test-tokenizer-0 models/ggml-vocab-llama-bpe.gguf

# Run tests with specific label
ctest --test-dir build -L "main"
```

### Test Categories
- **main**: Core functionality tests
- **model**: Model loading and management tests
- **mlu**: MLU backend specific tests (when GGML_MLU is enabled)

## Common Development Workflows

### Adding New Backend Support
1. Create backend directory in `ggml/src/ggml-{backend}/`
2. Implement backend interface following existing patterns
3. Add CMake configuration in `ggml/src/ggml-{backend}/CMakeLists.txt`
4. Add backend option to main CMakeLists.txt
5. Add backend tests in `tests/test-{backend}-*.cpp`

### Model Format Support
- Models must be in GGUF format
- Use conversion scripts like `convert_hf_to_gguf.py` for HuggingFace models
- Model loading logic is in `src/llama-model-loader.cpp`

### Performance Optimization
- Use quantization for reduced memory usage (`tools/quantize/`)
- Enable appropriate backend for hardware acceleration
- Tune context length and batch sizes for specific use cases
- Use imatrix for improved quantization quality (`tools/imatrix/`)

## Key File Locations

### Core Implementation
- `include/llama.h`: Main C API
- `src/llama.cpp`: Core library implementation
- `ggml/include/ggml.h`: Tensor operations API

### Configuration
- `CMakeLists.txt`: Main build configuration
- `cmake/`: CMake modules and configuration helpers

### Documentation
- `README.md`: Main project documentation
- `docs/build.md`: Detailed build instructions
- `docs/backend/`: Backend-specific documentation

### Models and Vocabularies
- `models/`: Directory for model files and vocabulary files
- `convert_hf_to_gguf.py`: Model conversion utility

## Backend-Specific Notes

### CUDA Backend
- Requires CUDA toolkit installation
- Supports compute capabilities 6.0+
- Use `CUDA_VISIBLE_DEVICES` for device selection
- Enable with `-DGGML_CUDA=ON`

### Metal Backend (Apple Silicon)
- Automatically enabled on macOS
- Optimized for Apple M1/M2/M3 processors
- Disable with `-DGGML_METAL=OFF`

### MLU Backend (Cambricon)
- Recently added support for Cambricon MLU devices
- Enable with `-DGGML_MLU=ON`
- Requires MLU SDK installation
- Has dedicated tests: `test-mlu-add.cpp`, `test-mlu-mul-mat.cpp`

### CPU Backend
- Default backend, always available
- Supports various optimizations (AVX, NEON, etc.)
- Multi-threading support via OpenMP

## Troubleshooting

### Build Issues
- Ensure all dependencies are installed for chosen backend
- Check CMake configuration output for missing dependencies
- Use `-DCMAKE_BUILD_TYPE=Debug` for debugging build issues

### Runtime Issues
- Verify model format is GGUF
- Check available memory for model size
- Use `--list-devices` to see available compute devices
- Monitor GPU memory usage for GPU backends

### Performance Issues
- Profile with appropriate backend-specific tools
- Check quantization settings
- Verify optimal batch sizes for hardware
- Consider using multiple GPUs if available

## Development Best Practices

### C++ Programming Guidelines
- @~/.claude/c++_programming_guide.md

### Code Style
- Follow existing code patterns and conventions
- Use consistent indentation and formatting
- Add appropriate error handling
- Include comprehensive tests for new features

### Testing
- Write unit tests for new components
- Add integration tests for complete workflows
- Test on multiple backends when applicable
- Verify memory management and resource cleanup

### Documentation
- Update relevant documentation for new features
- Add examples for new functionality
- Document any breaking changes
- Update build instructions if needed
