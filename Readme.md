# Build Instructions for Windows

In the `bin` folder, you'll find the prebuilt binary, ready for execution, along with the required DLL for the FFT algorithm.

**Important**:
- The host machine must be **64-bit**.
- All input CSV files should be placed **just outside the `src` folder**. 
- Make sure to run `WaveGen` **just outside the `src` folder**.

If you want to build the project from scratch, you can use either **MinGW** or **MSVC**. Follow these steps:

## 1. Create a build directory and run CMake

```
mkdir build
cd build
cmake ..
```


## Step 2: Build with Your Compiler  
### For MinGW:  
Run the following command to build the project:

```bash
make
```
The output will be placed in both the bin and build folders.

### For MSVC:  
Run the following command to build the project:

```bash
msbuild WaveAnalyzer.sln /p:Configuration=Release /p:Platform=x64
```
The output will be placed in the Release folder inside build.

## Step 3: Copy the Executable and DLL
Once the build is complete, follow these steps:

1. Copy WaveAnalyzer.exe from the Release folder
copy Release\WaveAnalyzer.exe to bin\

2. Copy libfftw3-3.dll from 3rdparty\fftw-3.3.5-dll64
copy 3rdparty\fftw-3.3.5-dll64\libfftw3-3.dll to bin\
After copying, both files should be placed in the bin folder.

# Running the Application
After placing the required files in the bin folder, you can run the application.

Note: Ensure that all input CSV files are placed just outside of the src folder.
