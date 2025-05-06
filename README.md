> [!IMPORTANT]
> ## Important notes
> 1. You can pass the memory limit using the third parameter at startup. Otherwise, it is equal to the length of the tape. (In the program, the value of the memory limit means how many numbers we can load from the tape.)
> 2. The configuration file is ```tape.config```. It specifies delays in milliseconds.
> 3. You must specify the ```input_file``` and the ```output_file``` in normal text mode. Then it is converted to binary, sorted, and converted to text again.
> 4. You don't need to create an output tape - it will be created automatically with the required size. (But you can also create it manually, with the number of numbers as in the input one)  
This is done to make the emulation as similar as possible, but at the same time to make the data easy to compare at the end.

# Build
> [!NOTE]
> The project requires support of ```C++17``` standart.

For a successful build create directory ```/build``` in the project root. Then use following commands:
```cmd
cd build
cmake ..
make
```
You can choose whether to build the tests or not using the option in CMakeLists.txt:
```cmake
option(FILE_TAPE_BUILD_TESTS "Build tests" ON)
```
By default, it's ```ON```.

Now, you can run the program:
```cmd
./main <input_file> <output_file> [memory limit]
```
...and tests:
```cmd
./tests/tests
```


