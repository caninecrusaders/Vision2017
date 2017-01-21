#!/bin/bash
cmake . -DCUDA_USE_STATIC_CUDA_RUNTIME=OFF
make
./vision "test.mp4"
