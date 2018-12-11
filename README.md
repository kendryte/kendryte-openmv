Kendryte-standalone-opemv build steps:
1. download V8.2.0 toolchain from https://kendryte.com/downloads/
2. export PATH=/opt/maix-toolchain/bin/:$PATH (riscv toolchain path for micropython build)
3. mkdir build
4. cd build
5. cmake .. -DPROJ=openmv -DTOOLCHAIN=/opt/maix-toolchain/bin (riscv toolchain path for kendryte sdk build)
6. make

The default example is face detect, you can change src/openmv/src/micropython/ports/k210-standalone/buildin-py/boot.py to implement other openmv functions.
